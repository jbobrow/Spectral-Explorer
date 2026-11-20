#ifndef SPECTRUMCOMBINATIONDOCK_H
#define SPECTRUMCOMBINATIONDOCK_H

#include <QWidget>
#include "dock.h"
#include "spectrum/spectrum.h"
#include "spectrum/spectrum_data.h"

namespace Ui {
class SpectrumCombinationDock;
}



class SpectrumCombinationDock : public Dock
{
    Q_OBJECT

public:
    explicit SpectrumCombinationDock(QWidget *parent = nullptr);
    ~SpectrumCombinationDock();

    QString getIdentifier() {
        return "Combination Dock";
    }

public slots:
    void setSpectrum1(QString spec);
    void setSpectrum2(QString spec);

protected:
    void timerEvent(QTimerEvent * event);

private slots:
    void onNewSpectrum1Data(SpectrumData d);
    void onNewSpectrum2Data(SpectrumData d);
    void updateAvailableSpectrums();

private:
    void update();

private:
    Ui::SpectrumCombinationDock *ui;
    Spectrum* mSpectrum1 = nullptr;
    Spectrum* mSpectrum2 = nullptr;
    SpectrumData mData1;
    SpectrumData mData2;

};

#endif // SPECTRUMCOMBINATIONDOCK_H
