#ifndef SPECTROMETERWINDOW_H
#define SPECTROMETERWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QList>
#include <QTimer>
#include "dock_container.h"

namespace Ui {
class SpectrometerWindow;
}

class SpectrometerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpectrometerWindow(QWidget *parent = nullptr);
    ~SpectrometerWindow();

public slots:
    void debug();
    void setupEmissionView();
    void setupAbsorptionView();
    void setupAbsorptionViewSmall();
    void clearDocks();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionEmissionMode_toggled(bool arg1);
    void on_actionAbsorptionMode_toggled(bool arg1);
    void on_actionAbsorptionModeSmall_toggled(bool arg1);
    void on_cameraTimeout();
    void on_cameraInit(int devID);
    void on_actionSettings_triggered();
    void on_actionAbout_Spectral_Explorer_triggered();

private:
    Ui::SpectrometerWindow *ui;
    QActionGroup *mViewModeGroup = nullptr;
    QList<QDockWidget*> mDocks;
    QTimer *mTimer = nullptr;
};

#endif // SPECTROMETERWINDOW_H
