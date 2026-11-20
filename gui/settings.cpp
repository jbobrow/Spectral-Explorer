#include "settings.h"
#include "ui_settings.h"
#include <QSettings>
#include <QProcess>
#include <QDebug>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings),
    mCameraDetectionMode("Camera/DetectionMode",1),
    mManualCamera("Camera/ManualCamera",0)
{
    ui->setupUi(this);

    ui->cameraDetectionCombo->addItem(tr("Manual Camera Detection"));
    ui->cameraDetectionCombo->addItem(tr("OpenCV Camera Detection (recommended)"));
    ui->cameraDetectionCombo->addItem(tr("Qt Camera Detection"));
    ui->cameraDetectionCombo->addItem(tr("Maximum Camera Detection"));
    for (int i = 0; i < 4; i++)
        ui->manualCameraCombo->addItem(QString(tr("Camera %1")).arg(i));
    connect(ui->cameraDetectionCombo, SIGNAL(currentIndexChanged(int)),
            this, SLOT(enableManualCameraCombo(int)));

    mCameraDetectionMode.addWidget(ui->cameraDetectionCombo);
    mManualCamera.addWidget(ui->manualCameraCombo);

    mSettings = new QSettings;
    loadAll();
    connect(this,SIGNAL(accepted()),this,SLOT(apply()));
    connect(this,SIGNAL(rejected()),this,SLOT(cancel()));
}

Settings::~Settings()
{
    delete ui;
    delete mSettings;
}

Settings& Settings::getInstance()
{
    static Settings instance;   // Guaranteed to be destroyed.
                                // Instantiated on first use.
                                // This approach is thread-safe with C++11
    return instance;
}

void Settings::loadAll()
{
    qDebug() << "Load all Settings.";
    mLoadAfterSaving = mSettings->value("GuiBehaviour/LoadAfterSaving",mLoadAfterSaving).toBool();
    mAutoConfigFiles = mSettings->value("GuiBehaviour/AutoConfigFiles",mAutoConfigFiles).toBool();
    mResX = mSettings->value("Camera/ResX",mResX).toInt();
    mResY = mSettings->value("Camera/ResY",mResY).toInt();
    mFilterEnabledConf = mSettings->value("GuiBehaviour/FilterEnabledConf",mFilterEnabledConf).toBool();
    ui->loadAfterSavingCheckBox->setChecked(mLoadAfterSaving);
    ui->autoConfigFilesCheckBox->setChecked(mAutoConfigFiles);
    ui->resXSpin->setValue(mResX);
    ui->resYSpin->setValue(mResY);

    mCameraDetectionMode.load(*mSettings);
    mManualCamera.load(*mSettings);
}

void Settings::apply()
{
    setLoadAfterSaving(ui->loadAfterSavingCheckBox->isChecked());
    setAutoConfigFiles(ui->autoConfigFilesCheckBox->isChecked());
    if (mResX != ui->resXSpin->value() || mResY != ui->resYSpin->value())
        mRestartRequired = true;

    setResX(ui->resXSpin->value());
    setResY(ui->resYSpin->value());

    mCameraDetectionMode.save(*mSettings);
    mManualCamera.save(*mSettings);

    if (mRestartRequired)
    {
        // restart:
        qApp->quit();
        QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
    }
}

void Settings::cancel()
{
    mCameraDetectionMode.reset();
    mManualCamera.reset();
}

void Settings::setLoadAfterSaving(bool e)
{
    mLoadAfterSaving = e;
    mSettings->setValue("GuiBehaviour/LoadAfterSaving",e);
    ui->loadAfterSavingCheckBox->setChecked(e);
}

void Settings::setAutoConfigFiles(bool e)
{
    mAutoConfigFiles = e;
    mSettings->setValue("GuiBehaviour/AutoConfigFiles",e);
    ui->autoConfigFilesCheckBox->setChecked(e);
}

void Settings::setResX(int x)
{
    mResX = x;
    mSettings->setValue("Camera/ResX",x);
    ui->resXSpin->setValue(x);
}

void Settings::setResY(int y)
{
    mResY = y;
    mSettings->setValue("Camera/ResY",y);
    ui->resYSpin->setValue(y);
}

void Settings::setFilterEnabledConf(bool e)
{
    mFilterEnabledConf = e;
    mSettings->setValue("GuiBehaviour/FilterEnabledConf",e);
}

void Settings::enableManualCameraCombo(int i)
{
    ui->manualCameraCombo->setEnabled(i == 0);
}

bool Settings::getLoadAfterSaving() { return mLoadAfterSaving; }
bool Settings::getAutoConfigFiles() { return mAutoConfigFiles; }
int Settings::getResX() { return mResX; }
int Settings::getResY() { return mResY; }
bool Settings::getFilterEnabledConf() { return mFilterEnabledConf; }
int Settings::getCameraDetectionMode() { return mCameraDetectionMode.value().toInt(); }
int Settings::getManualCamera() { return mManualCamera.value().toInt(); }
