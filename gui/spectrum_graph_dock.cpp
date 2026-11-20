#include "spectrum_graph_dock.h"
#include "ui_spectrum_graph_dock.h"
#include "spectrum/spectrum_disposer.h"
#include "settings.h"

#include <QFileDialog>
#include <QDir>
#include <QDebug>
#include <QTextStream>

SpectrumGraphDock::SpectrumGraphDock(QWidget *parent) :
    Dock(parent),
    ui(new Ui::SpectrumGraphDock)
{
    ui->setupUi(this);

    mDrawingIntensity = new SpectrumIntensityDrawing(SpectrumData());
    addSpectrumSelector();
    mSpecSelectors.last()->setDeletable(false);
    connect(ui->addSpectrumSelectorButton,SIGNAL(clicked()),this,SLOT(addSpectrumSelector()));
}

SpectrumGraphDock::~SpectrumGraphDock()
{
    if (mExportDialog != nullptr) delete mExportDialog;
    mDrawing = nullptr;
    delete mDrawingIntensity;
    delete mDrawingRGB;
    delete mDrawingColored;
    delete ui;
}

void SpectrumGraphDock::addSpectrumSelector()
{
    SpectrumSelector* s = new SpectrumSelector(true,true,this);
    mSpecSelectors.append(s);
    ui->spectrumGroupBox->layout()->addWidget(s);
    connect(s, SIGNAL(newSpectrumData(SpectrumData)), this, SLOT(onNewSpectrumData(SpectrumData)));
    connect(s, SIGNAL(bindSpectrum(Spectrum*)), this, SLOT(onBindSpectrum(Spectrum*)));
    connect(s, SIGNAL(unbindSpectrum(Spectrum*)), this, SLOT(onUnbindSpectrum(Spectrum*)));
    connect(s, SIGNAL(destroy(SpectrumSelector*)), this, SLOT(removeSpectrumSelector(SpectrumSelector*)));
    // request frame so that new spectrums are shown when they are added
    s->requestFrame();
}

void SpectrumGraphDock::removeSpectrumSelector(SpectrumSelector *s)
{
    mSpecSelectors.removeOne(s);
    ui->spectrumGroupBox->layout()->removeWidget(s);
}

void SpectrumGraphDock::onBindSpectrum(Spectrum *s)
{
    qDebug() << "[SpectrumGraphDock] Bind spectrum" << s->mName;
    connect(ui->rgbRadioButton, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    connect(ui->intensityRadioButton, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    connect(ui->coloredRadioButton, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    connect(ui->cflLinesCheckBox, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    connect(ui->fraunhoferCheckBox, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    connect(ui->graphOutput, SIGNAL(targetSelected(MeasurementTarget,int)), s, SLOT(onTargetSelected(MeasurementTarget,int)));
}

void SpectrumGraphDock::onUnbindSpectrum(Spectrum *s)
{
    int uses = 0;
    for (SpectrumSelector* sel : mSpecSelectors)
        if (sel->getID() == s->mID)
            uses++;
    // mSpectrum of SpectrumSelector isn't already unregistered --> "<=1"
    if (uses <= 1) mDrawingIntensity->removeSpectrumData(s->mID);

    disconnect(ui->rgbRadioButton, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    disconnect(ui->intensityRadioButton, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    disconnect(ui->coloredRadioButton, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    disconnect(ui->cflLinesCheckBox, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    disconnect(ui->fraunhoferCheckBox, SIGNAL(clicked()), s->getCamera(), SLOT(requestFrame()));
    disconnect(ui->graphOutput, SIGNAL(targetSelected(MeasurementTarget,int)), s, SLOT(onTargetSelected(MeasurementTarget,int)));
}

void SpectrumGraphDock::onNewSpectrumData(SpectrumData d)
{
    if (mExportDialog != nullptr && mExportDialog->isActiveWindow())
        return;
    if (ui->intensityRadioButton->isChecked())
    {
        mDrawingIntensity->addSpectrumData(d);
        mDrawing = mDrawingIntensity;

    }
    else if (ui->coloredRadioButton->isChecked())
    {
        if (mDrawingColored != nullptr) delete mDrawingColored;
        mDrawingColored = new SpectrumIntensityColoredDrawing(d);
        mDrawingColored->setColorOpacity(0.4);
        mDrawing = mDrawingColored;

    }
    else if (ui->rgbRadioButton->isChecked())
    {
        if (mDrawingRGB != nullptr) delete mDrawingRGB;
        mDrawingRGB = new SpectrumRGBDrawing(d);
        mDrawing = mDrawingRGB;
    }
    else
    {
       qWarning() << "[SpecGra]" << Q_FUNC_INFO << "No radio button is selected. Abort.";
       return;
    }

    mDrawing->setWavelengthSteps(mWavelengthSteps);
    mDrawing->setCFLLinesEnabled(ui->cflLinesCheckBox->isChecked());
    mDrawing->setFraunhoferLinesEnabled(ui->fraunhoferCheckBox->isChecked());
    ui->graphOutput->showSpectrumGraph(mDrawing,false);
}

void SpectrumGraphDock::on_exportButton_clicked()
{
    if (mDrawing == nullptr)
    {
        qWarning() << "[SpecGra]" << Q_FUNC_INFO << "mDrawing is nullptr. Abort.";
        return;
    }
    QList<Spectrum*> spectrums;
    for (int i = 0; i < mSpecSelectors.length(); i++)
    {
        if (mSpecSelectors[i]->getSpectrum() == nullptr)
        {
            qWarning() << "Export CSV failed: Spectrum is nullptr";
            continue;
        }
        spectrums.append(mSpecSelectors[i]->getSpectrum());
    }
    if (mExportDialog != nullptr)
        delete mExportDialog;
    mExportDialog = new ExportDialog(spectrums,mDrawing,this);
    mExportDialog->setWindowModality(Qt::ApplicationModal);
    mExportDialog->show();
}
