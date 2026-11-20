#include "spectrum_combination_dock.h"
#include "ui_spectrum_combination_dock.h"
#include "spectrum/spectrum_disposer.h"
#include "spectrum/spectrum_intensity_drawing.h"
#include "spectrum/spectrum_rgb_drawing.h"

SpectrumCombinationDock::SpectrumCombinationDock(QWidget *parent) :
    Dock(parent),
    ui(new Ui::SpectrumCombinationDock)
{
    ui->setupUi(this);
    updateAvailableSpectrums();
    setSpectrum1(ui->spectrum1ComboBox->currentText());
    setSpectrum2(ui->spectrum2ComboBox->currentText());
    connect(SpectrumDisposer::getInstance(), SIGNAL(spectrumsChanged()), this, SLOT(updateAvailableSpectrums()));
    connect(ui->spectrum1ComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(setSpectrum1(QString)));
    connect(ui->spectrum2ComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(setSpectrum2(QString)));

    startTimer(1000/30);
}

SpectrumCombinationDock::~SpectrumCombinationDock()
{
    delete ui;
}

void SpectrumCombinationDock::updateAvailableSpectrums()
{
    ui->spectrum1ComboBox->clear();
    ui->spectrum2ComboBox->clear();
    foreach (Spectrum* s, SpectrumDisposer::getInstance()->getSpectrums())
    {
        ui->spectrum1ComboBox->addItem(s->mName);
        ui->spectrum2ComboBox->addItem(s->mName);
    }
}

void SpectrumCombinationDock::setSpectrum1(QString spec)
{
    if (mSpectrum1 != nullptr)
        disconnect(mSpectrum1, SIGNAL(newSpectrumData(SpectrumData)),this, SLOT(onNewSpectrum1Data(SpectrumData)));
    ui->spectrum1ComboBox->setCurrentText(spec);
    mSpectrum1 = SpectrumDisposer::getInstance()->getSpectrum(spec);
    if (mSpectrum1 != nullptr)
    {
        connect(mSpectrum1, SIGNAL(newSpectrumData(SpectrumData)),this, SLOT(onNewSpectrum1Data(SpectrumData)));
        mSpectrum1->getCamera()->requestFrame();
    }
}

void SpectrumCombinationDock::setSpectrum2(QString spec)
{
    if (mSpectrum2 != nullptr)
        disconnect(mSpectrum2, SIGNAL(newSpectrumData(SpectrumData)),this, SLOT(onNewSpectrum2Data(SpectrumData)));
    ui->spectrum2ComboBox->setCurrentText(spec);
    mSpectrum2 = SpectrumDisposer::getInstance()->getSpectrum(spec);
    if (mSpectrum2 != nullptr)
    {
        connect(mSpectrum2, SIGNAL(newSpectrumData(SpectrumData)),this, SLOT(onNewSpectrum2Data(SpectrumData)));
        mSpectrum2->getCamera()->requestFrame();
    }
}

void SpectrumCombinationDock::onNewSpectrum1Data(SpectrumData d)
{
    mData1 = d;
}

void SpectrumCombinationDock::onNewSpectrum2Data(SpectrumData d)
{
    mData2 = d;
}

void SpectrumCombinationDock::timerEvent(QTimerEvent *event)
{
    update();
}

void SpectrumCombinationDock::update()
{
    SpectrumData d = mData1 - mData2;
    d.mGraphColor = Qt::white;

    if (ui->intensityRadioButton->isChecked())
    {
        SpectrumIntensityDrawing* dr;
        dr = new SpectrumIntensityDrawing(d);
        dr->addSpectrumData(d);
        ui->graphOutput->showSpectrumGraph(dr);
    }
    if (ui->rgbRadioButton->isChecked())
    {
        SpectrumRGBDrawing* dr;
        dr = new SpectrumRGBDrawing(d);
        ui->graphOutput->showSpectrumGraph(dr);
    }
}
