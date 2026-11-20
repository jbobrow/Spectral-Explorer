#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QMainWindow>
#include <QProcess>
#include "spectrum/spectrum.h"

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigDialog(Spectrum *spec, QWidget *parent = nullptr);
    ~ConfigDialog();

signals:
    void confirm();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent * event);

private slots:
    void onNewFrame(cv::Mat frame);
    void onNewSpectrumData(SpectrumData data);
    void onCVImageClicked(int x, int y);

    void on_confirmButton_clicked();
    void on_firstCFLLineButton_toggled(bool checked);
    void on_secondCFLLineButton_toggled(bool checked);
    void onNewCFLX(int x);

    void updateUi();

    void on_configureIntensityButton_clicked();
    void on_actionSave_Config_triggered();
    void on_actionLoad_Config_triggered();
    void on_actionRestore_Default_triggered();
    void on_actionSet_As_Default_triggered();
    void on_actionApply_to_All_triggered();
    void on_cameraConfigButton_clicked();
    void on_disableFilterCheckBox_toggled(bool checked);
    void on_exposureSpinBox_valueChanged(int arg1);
    void on_gainSpinBox_valueChanged(int arg1);
    void on_averageFramesSpinBox_valueChanged(int arg1);
    void on_offsetLeftSpin_valueChanged(int arg1);
    void on_offsetRightSpin_valueChanged(int arg1);

private:
    Ui::ConfigDialog *ui;
    QProcess *mCameConfProcess = nullptr;
    Spectrum *mSpectrum = nullptr;
    SpectrumConfig *mOldConfig = nullptr;
    QList<SpectrumData> mSpectrumData;
    Camera *mCamera = nullptr;
    bool mFirstCFLLine;
    bool mSecondCFLLine;
    int mCFLX1=0;
    int mCFLX2=0;
    double mLastScale = 1;
    bool mKeepConfig;
};

#endif // CONFIGDIALOG_H
