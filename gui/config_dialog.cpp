#include "config_dialog.h"
#include "ui_config_dialog.h"

#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QGuiApplication>
#include <QScreen>

#include "spectrum/spectrum_data.h"
#include "spectrum/spectrum_optical_drawing.h"
#include "spectrum/spectrum_intensity_colored_drawing.h"
#include "spectrum/spectrum_disposer.h"
#include "gui/settings.h"


ConfigDialog::ConfigDialog(Spectrum *spec, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);

    mSpectrum = spec;
    mCFLX1 = mSpectrum->getConfig()->getCFLX1();
    mCFLX2 = mSpectrum->getConfig()->getCFLX2();
    mCamera = spec->getCamera();

    ui->splitter->setStretchFactor(0,4);
    ui->splitter->setStretchFactor(1,1);
    connect(mCamera, SIGNAL(newFrame(cv::Mat)), this, SLOT(onNewFrame(cv::Mat)));
    connect(mSpectrum, SIGNAL(newSpectrumData(SpectrumData)), this, SLOT(onNewSpectrumData(SpectrumData)));
    connect(ui->cameraOutput, SIGNAL(cvImageClicked(int,int)), this, SLOT(onCVImageClicked(int,int)));
    connect(ui->cameraOutput, SIGNAL(sizeChanged()), mCamera, SLOT(requestFrame()));
    connect(ui->spectrumOutput, SIGNAL(newCFLX(int)), this, SLOT(onNewCFLX(int)));
    connect(ui->confirmButton, SIGNAL(clicked()), this, SIGNAL(confirm()));
    connect(ui->confirmButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->rotateButton, SIGNAL(clicked()), mCamera, SLOT(rotate90()));
//    connect(ui->sharpnessSpinBox, SIGNAL(valueChanged(int)), mCamera, SLOT(setSharpness(int)));
//    connect(ui->brightnessSpinBox, SIGNAL(valueChanged(int)), mCamera, SLOT(setBrightness(int)));

    mFirstCFLLine = ui->firstCFLLineButton->isChecked();
    mSecondCFLLine = ui->secondCFLLineButton->isChecked();

#ifndef DEVELOP
    ui->groupBox->setVisible(false);
    ui->intensityGroup->setVisible(false);
#endif
#ifndef DEVELOP
    ui->whiteBalanceSpinBox->hide();
    ui->label_7->hide();
    ui->brightnessSpinBox->hide();
    ui->label_2->hide();
    ui->sharpnessSpinBox->hide();
    ui->label_3->hide();
    ui->cameraConfigButton->hide();
#endif
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    QSettings settings;
    QByteArray savedGeometry = settings.value("GuiBehaviour/ConfigDialogGeometry").toByteArray();
    if (!savedGeometry.isEmpty()) {
        restoreGeometry(savedGeometry);
    } else if (QScreen *screen = QGuiApplication::primaryScreen()) {
        QRect avail = screen->availableGeometry();
        int w = qMax(800, static_cast<int>(avail.width() * 0.70));
        int h = qMax(600, static_cast<int>(avail.height() * 0.70));
        resize(w, h);
        move(avail.center() - rect().center());
    }

    mOldConfig = new SpectrumConfig(*mSpectrum->getConfig());
    mKeepConfig = false;
    mCamera->requestFrame();
    updateUi();
}

void ConfigDialog::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    QSettings settings;
    settings.setValue("GuiBehaviour/ConfigDialogGeometry", saveGeometry());

    if (!mKeepConfig)
    {
        mSpectrum->setConfig(mOldConfig);
        mCamera->requestFrame();
    }
    else
    {
        delete mOldConfig;
        mOldConfig = nullptr;
    }

    Settings::getInstance().setFilterEnabledConf(!ui->disableFilterCheckBox->isChecked());
    ui->disableFilterCheckBox->setChecked(false);
}

void ConfigDialog::updateUi()
{
    if (mCamera->isVirtual())
        ui->controlGroup->setEnabled(false);
    else
        ui->controlGroup->setEnabled(true);

    if (mCamera->exposureAvailable())
    {
        ui->exposureSpinBox->setEnabled(true);
    }
    if (mCamera->gainAvailable())
    {
        ui->gainSpinBox->setEnabled(true);
    }
    if (mCamera->brightnessAvailable())
    {
        ui->brightnessSpinBox->setEnabled(true);
    }
    if (mCamera->sharpnessAvailable())
    {
        ui->sharpnessSpinBox->setEnabled(true);
    }
    ui->exposureSpinBox->setValue(mSpectrum->getConfig()->getExposure());
    ui->gainSpinBox->setValue(mSpectrum->getConfig()->getGain());
    ui->temperatureSpinBox->setValue(mSpectrum->getConfig()->getBlackBodyTemp());
    ui->averageFramesSpinBox->setValue(mSpectrum->getConfig()->getRawAverageFrames());
    ui->disableFilterCheckBox->setChecked(!Settings::getInstance().getFilterEnabledConf());
    ui->offsetLeftSpin->setValue(mSpectrum->getConfig()->getLeftOffset());
    ui->offsetRightSpin->setValue(mSpectrum->getConfig()->getRightOffset());
}

//camera output
void ConfigDialog::onNewFrame(cv::Mat frame)
{
    if (!isVisible())
        return;

    int specRow = mSpectrum->getConfig()->getSpectrumRow();
    ui->cameraOutput->showImage(frame, specRow);
}

//spectrum output
void ConfigDialog::onNewSpectrumData(SpectrumData data)
{
    if (!isVisible())
        return;

    if (mSpectrum->getCamera()->isVirtual())
        mLastScale = data.normalizeIntensity();
    else
        mLastScale = data.normalizeIntensity(mLastScale,0.6);
    mSpectrumData.clear();
    // only needed for optical drawing strategy
//    while (mSpectrumData.length() < ui->spectrumOutput->height())
//        mSpectrumData.append(data);
    mSpectrumData.append(data);

    SpectrumIntensityColoredDrawing* dr;
    dr = new SpectrumIntensityColoredDrawing(mSpectrumData.last());
    dr->setOffset(0,0,0,0);
    dr->setBackgroundColor(Qt::black);
    dr->setMouseAxisEnabled(false);
    dr->setAxisEnabled(false);
    dr->setDrawTargets(false);
    dr->setEffectiveHeightScale(0.8);
    dr->setTitle(tr("Wavelength Config View"));
    dr->setConfigLinesEnabled(true);
    ui->spectrumOutput->showSpectrumGraph(dr);
}

void ConfigDialog::onCVImageClicked(int x, int y)
{
    mSpectrum->getConfig()->setSpectrumRow(y);
    mSpectrum->getCamera()->requestFrame();
}

void ConfigDialog::onNewCFLX(int x)
{
    if (mFirstCFLLine) mCFLX1 = x;
    if (mSecondCFLLine) mCFLX2 = x;
    mSpectrum->getConfig()->setCFLX(mCFLX1,mCFLX2);
    mSpectrum->getCamera()->requestFrame();
}

void ConfigDialog::on_firstCFLLineButton_toggled(bool checked)
{
    ui->secondCFLLineButton->setChecked(!checked);
    mFirstCFLLine = checked;
}

void ConfigDialog::on_secondCFLLineButton_toggled(bool checked)
{
    ui->firstCFLLineButton->setChecked(!checked);
    mSecondCFLLine = checked;
}

void ConfigDialog::on_configureIntensityButton_clicked()
{
    int temp = ui->temperatureSpinBox->value();
    mSpectrum->getConfig()->configureIntensity(mSpectrumData.last(),temp);
    mSpectrum->getCamera()->requestFrame();
}


void ConfigDialog::on_actionSave_Config_triggered()
{
    QString file;
    QSettings settings;
    file = QFileDialog::getSaveFileName(this,
                                        tr("Save File"),
                                        settings.value("GuiBehaviour/LastConfigDirectory").toString(),
                                        tr("Config File (*.config)"));
    if (file != "")
    {
        QDir dir;
        settings.setValue("GuiBehaviour/LastConfigDirectory",dir.absoluteFilePath(file));
        mSpectrum->getConfig()->save(file);
    }
}

void ConfigDialog::on_actionLoad_Config_triggered()
{
    QString file;
    QSettings settings;
    file = QFileDialog::getOpenFileName(this,
                                        tr("Open File"),
                                        settings.value("GuiBehaviour/LastConfigDirectory").toString(),
                                        tr("Config File (*.config)"));
    if (file != "")
    {
        QDir dir;
        settings.setValue("GuiBehaviour/LastConfigDirectory",dir.absoluteFilePath(file));
        mSpectrum->getConfig()->load(file);
    }
    delete mOldConfig;
    mOldConfig = new SpectrumConfig(*mSpectrum->getConfig());
    updateUi();
    mCamera->requestFrame();
}

void ConfigDialog::on_actionRestore_Default_triggered()
{
    mSpectrum->getConfig()->load();
    updateUi();
    mCamera->requestFrame();
}

void ConfigDialog::on_actionSet_As_Default_triggered()
{
    mSpectrum->getConfig()->save();
    delete mOldConfig;
    mOldConfig = new SpectrumConfig(*mSpectrum->getConfig());
}

void ConfigDialog::on_confirmButton_clicked()
{
    mKeepConfig = true;
}

void ConfigDialog::on_actionApply_to_All_triggered()
{
    QList<Spectrum*> spectrums;
    SpectrumConfig* config;

    spectrums = SpectrumDisposer::getInstance()->getSpectrums();
    //copy config because the original will be deleted in loop
    config = new SpectrumConfig(*mSpectrum->getConfig());
    for (int i = 0; i < spectrums.length(); i++)
    {
        spectrums[i]->setConfig(new SpectrumConfig(*config));
    }
    delete config;
    delete mOldConfig;
    mOldConfig = new SpectrumConfig(*mSpectrum->getConfig());
}

void ConfigDialog::on_cameraConfigButton_clicked()
{
    QString path = "C:/Windows/SysWOW64/LogiDPPApp.exe";
    QFile test(path);
    if (!test.exists())
    {
        QMessageBox::warning(this,QObject::tr("Warning"),
                             QObject::tr("Please install \"Logitech Webcam-Software\""));
        return;
    }

    if (mCameConfProcess != nullptr)
    {
        mCameConfProcess->kill();
        mCameConfProcess->waitForFinished();
        delete mCameConfProcess;
        mCameConfProcess = nullptr;
    }
    mCameConfProcess = new QProcess(this);
    mCameConfProcess->start(path);
    mCameConfProcess->waitForStarted();
}

void ConfigDialog::on_exposureSpinBox_valueChanged(int arg1)
{
    mSpectrum->getConfig()->setExposure(arg1);
}

void ConfigDialog::on_gainSpinBox_valueChanged(int arg1)
{
    mSpectrum->getConfig()->setGain(arg1);
}

void ConfigDialog::on_disableFilterCheckBox_toggled(bool checked)
{
    mSpectrum->getConfig()->setFilterEnabled(!checked);
}

void ConfigDialog::on_averageFramesSpinBox_valueChanged(int arg1)
{
    mSpectrum->getConfig()->setAverageFrames(arg1);
}

void ConfigDialog::on_offsetLeftSpin_valueChanged(int arg1)
{
    mSpectrum->getConfig()->setLeftOffset(arg1);
    SpectrumConfig *conf = mSpectrum->getConfig();
    int maxRight = conf->getCamera()->getWidth() - conf->getLeftOffset() - 1;
    if (maxRight >= 0)
        ui->offsetRightSpin->setMaximum(maxRight);
}

void ConfigDialog::on_offsetRightSpin_valueChanged(int arg1)
{
    mSpectrum->getConfig()->setRightOffset(arg1);
    SpectrumConfig *conf = mSpectrum->getConfig();
    int maxLeft = conf->getCamera()->getWidth() - conf->getRightOffset() - 1;
    if (maxLeft >= 0)
        ui->offsetLeftSpin->setMaximum(maxLeft);
}
