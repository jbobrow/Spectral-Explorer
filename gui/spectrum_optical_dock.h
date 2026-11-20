#ifndef CAMERADOCK_H
#define CAMERADOCK_H

#include <QWidget>
#include "dock.h"
#include "config_dialog.h"
#include "spectrum/spectrum.h"
#include "gui/spectrum_selector.h"

namespace Ui {
class SpectrumOpticalDock;
}

class SpectrumOpticalDock : public Dock
{
    Q_OBJECT

public:
    explicit SpectrumOpticalDock(QWidget *parent = nullptr);
    ~SpectrumOpticalDock();

    QString getIdentifier() {
        return "Camera Dock";
    }

private slots:
    void onNewSpectrumData(SpectrumData data);
    void onConfigureConfirm();
    void onBindSpectrum(Spectrum* s);
    void onUnbindSpectrum(Spectrum* s);
    void enableSpectrumUI(bool e);
    void on_configureButton_clicked();
    void on_saveButton_clicked();
    void on_loadButton_clicked();

private:
    Ui::SpectrumOpticalDock *ui;
    ConfigDialog *mConfigDialog = nullptr;
    SpectrumSelector *mSpectrumSelector;
    QList<SpectrumData> mFloatingData;

protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // CAMERADOCK_H
