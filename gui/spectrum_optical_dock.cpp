#include "spectrum_optical_dock.h"
#include "ui_spectrum_optical_dock.h"
#include <QStackedWidget>
#include <QDebug>
#include <QFileDialog>
#include <opencv2/highgui/highgui.hpp>
#include "gui/config_dialog.h"
#include "spectrum/spectrum_optical_drawing.h"
#include "spectrum/spectrum_disposer.h"
#include "settings.h"


SpectrumOpticalDock::SpectrumOpticalDock(QWidget *parent) :
    Dock(parent),
    ui(new Ui::SpectrumOpticalDock)
{
    ui->setupUi(this);

    mSpectrumSelector = new SpectrumSelector(this);
    ui->spectrumLayout->insertWidget(0,mSpectrumSelector);
    enableSpectrumUI(false);

    connect(mSpectrumSelector, SIGNAL(newSpectrumData(SpectrumData)), this, SLOT(onNewSpectrumData(SpectrumData)));
    connect(mSpectrumSelector, SIGNAL(bindSpectrum(Spectrum*)), this, SLOT(onBindSpectrum(Spectrum*)));
    connect(mSpectrumSelector, SIGNAL(unbindSpectrum(Spectrum*)), this, SLOT(onUnbindSpectrum(Spectrum*)));
    onBindSpectrum(mSpectrumSelector->getSpectrum());
    connect(ui->clearSpectrumsButton, SIGNAL(clicked()), SpectrumDisposer::getInstance(), SLOT(clearSpectrums()));

    //old version with stacked widget for config dialog
//    ui->stackedWidget->removeWidget(ui->page_2);
//    ui->stackedWidget->addWidget(mCameraConfig);

    //causes crash when maximizing
//    connect(ui->spectrumOutput, SIGNAL(sizeChanged()), mSpectrum->getCamera(), SLOT(requestFrame()));
}

SpectrumOpticalDock::~SpectrumOpticalDock()
{
    if (mConfigDialog != nullptr)
    {
        delete mConfigDialog;
        mConfigDialog = nullptr;
    }
    delete ui;
}

void SpectrumOpticalDock::resizeEvent(QResizeEvent *e)
{
    if (mSpectrumSelector != nullptr)
        mSpectrumSelector->requestFrame();
}

void SpectrumOpticalDock::onBindSpectrum(Spectrum *s)
{
    if (s == nullptr)
        return;
    connect(ui->freezeButton, SIGNAL(toggled(bool)), s, SLOT(setFreeze(bool)));
    if (mConfigDialog == nullptr) {
        mConfigDialog = new ConfigDialog(s);
        connect(mConfigDialog, SIGNAL(confirm()), this, SLOT(onConfigureConfirm()));
    }
    mSpectrumSelector->getSpectrum()->setActive(true);

    enableSpectrumUI(true);
}

void SpectrumOpticalDock::onUnbindSpectrum(Spectrum *s)
{
    enableSpectrumUI(false);

    disconnect(ui->freezeButton, SIGNAL(toggled(bool)), s, SLOT(setFreeze(bool)));
    delete mConfigDialog;
    mConfigDialog = nullptr;
    mSpectrumSelector->getSpectrum()->setActive(false);
}

void SpectrumOpticalDock::onNewSpectrumData(SpectrumData data)
{
    Spectrum *spec = mSpectrumSelector->getSpectrum();
    if (spec == nullptr)
        return;

    int maxRows = qMax(1, ui->spectrumOutput->height());
    if (!spec->getCamera()->isVirtual())
        mFloatingData.append(data);
    else
    {
        mFloatingData.clear();
        while (mFloatingData.length() < maxRows)
            mFloatingData.append(data);
    }
    while (mFloatingData.length() >= maxRows)
        mFloatingData.pop_front();
    SpectrumDrawingStrategy* dr;
    dr = new SpectrumOpticalDrawing(&mFloatingData);
    ui->spectrumOutput->showSpectrumGraph(dr);
}

void SpectrumOpticalDock::enableSpectrumUI(bool e)
{
    ui->configureButton->setEnabled(e);
    ui->freezeButton->setEnabled(e);
    ui->saveButton->setEnabled(e);
    ui->clearSpectrumsButton->setEnabled(e);
    mSpectrumSelector->setEnabled(e);
}

void SpectrumOpticalDock::on_configureButton_clicked()
{
    Spectrum *s = mSpectrumSelector->getSpectrum();
    if (mConfigDialog == nullptr || s == nullptr || s->getCamera() == nullptr)
        return;

    mConfigDialog->show();
    s->getCamera()->requestFrame();
}

void SpectrumOpticalDock::onConfigureConfirm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void SpectrumOpticalDock::on_saveButton_clicked()
{
    Spectrum *s = mSpectrumSelector->getSpectrum();
    if (s == nullptr || s->getCamera() == nullptr)
        return;

    QString file;
    QSettings settings;
    bool wasActive = s->isActive();
    s->setActive(false);
    file = QFileDialog::getSaveFileName(this,
                                        tr("Save File"),
                                        settings.value("GuiBehaviour/LastDirectory").toString(),
                                        tr("Images (*.png *.jpg)"));
    qDebug() << "save" << file;

    if (!file.isEmpty())
    {
        QDir dir;
        QString path = dir.absoluteFilePath(file);
        path.remove(path.split("/").last());
        settings.setValue("GuiBehaviour/LastDirectory",path);
        cv::Mat frame = s->getCamera()->getFrame();
        if (!frame.empty())
            cv::imwrite(file.toLocal8Bit().constData(), frame);
    }

    // save config
    if (!file.isEmpty() && Settings::getInstance().getAutoConfigFiles())
    {
        QString configFile = file;
        configFile.replace(".png",".config");
        configFile.replace(".jpg",".config");
        s->getConfig()->save(configFile);
    }

    s->setActive(wasActive);

    // load after saving
    bool load = Settings::getInstance().getLoadAfterSaving();
    if (load && !file.isEmpty())
    {
        Spectrum *loaded = new Spectrum(file);
        Q_UNUSED(loaded);
    }
}

void SpectrumOpticalDock::on_loadButton_clicked()
{
    QStringList files;
    QSettings settings;
    files = QFileDialog::getOpenFileNames(this,
                                        tr("Open File"),
                                        settings.value("GuiBehaviour/LastDirectory").toString(),
                                        tr("Images (*.png *.jpg)"));

    if (files.empty())
        return;

    QDir dir;
    QString path = dir.absoluteFilePath(files.last());
    path.remove(path.split("/").last());
    settings.setValue("GuiBehaviour/LastDirectory",path);

    for (QString file : files)
    {
        Spectrum *s = new Spectrum(file);
        mSpectrumSelector->setSpectrum(s->mName);
    }
}
