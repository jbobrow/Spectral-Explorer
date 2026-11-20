#ifndef GRAPHDOCK_H
#define GRAPHDOCK_H

#include <QWidget>
#include "dock.h"
#include "gui/spectrum_selector.h"
#include "spectrum/spectrum.h"
#include "spectrum/spectrum_intensity_drawing.h"
#include "spectrum/spectrum_rgb_drawing.h"
#include "spectrum/spectrum_intensity_colored_drawing.h"
#include "export_dialog.h"

namespace Ui {
class SpectrumGraphDock;
}

class SpectrumGraphDock : public Dock
{
    Q_OBJECT

public:
    explicit SpectrumGraphDock(QWidget *parent = nullptr);
    ~SpectrumGraphDock();

    QString getIdentifier() {
        return "Graph Dock";
    }
    int mWavelengthSteps = 50;

public slots:
    void addSpectrumSelector();
    void removeSpectrumSelector(SpectrumSelector* s);

private slots:
    void onNewSpectrumData(SpectrumData d);
    void onBindSpectrum(Spectrum* s);
    void onUnbindSpectrum(Spectrum* s);
    void on_exportButton_clicked();

private:
    Ui::SpectrumGraphDock *ui;
    QList<SpectrumSelector*> mSpecSelectors;
    SpectrumDrawingStrategy* mDrawing = nullptr;
    SpectrumIntensityDrawing* mDrawingIntensity = nullptr;
    SpectrumRGBDrawing* mDrawingRGB = nullptr;
    SpectrumIntensityColoredDrawing *mDrawingColored = nullptr;
    ExportDialog* mExportDialog = nullptr;
};

#endif // GRAPHDOCK_H
