#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>
#include "settings/setting.h"

namespace Ui {
class Settings;
}

//! This class includes all settings which concern the user interface
class Settings : public QDialog
{
    Q_OBJECT

public:
    static Settings& getInstance();

    bool getLoadAfterSaving();
    bool getAutoConfigFiles();
    int getResX();
    int getResY();
    bool getFilterEnabledConf();
    int getCameraDetectionMode();
    int getManualCamera();

public slots:
    void setLoadAfterSaving(bool e);
    void setAutoConfigFiles(bool e);
    void setResX(int x);
    void setResY(int y);
    void setFilterEnabledConf(bool e);
    void enableManualCameraCombo(int i);


private slots:
    void apply();
    void cancel();

private:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    void loadAll();

    Ui::Settings *ui;
    QSettings* mSettings = nullptr;

    bool mLoadAfterSaving = false;
    bool mAutoConfigFiles = true;
    int mResX = 1280;
    int mResY = 960;
    bool mFilterEnabledConf = false;
    bool mRestartRequired = false;
    Setting mCameraDetectionMode;
    Setting mManualCamera;
};

#endif // SETTINGS_H
