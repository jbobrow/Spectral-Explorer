#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QList>
#include "settings/setting.h"
#include "spectrum/spectrum.h"
#include "spectrum/spectrum_drawing_strategy.h"

namespace Ui {
class ExportDialog;
}

enum ExportMode {
    ExportCSV,
    ExportImage
};

class ExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExportDialog(QList<Spectrum*> spectrums, SpectrumDrawingStrategy *drStrategy, QWidget *parent = nullptr);
    ~ExportDialog();

    void exportImage();
    void exportCSV();

private:
    Ui::ExportDialog *ui;

    QSettings mSettings;
    QList<Spectrum*> mSpectrums;
    SpectrumDrawingStrategy *mDrawingStrategy;

    Setting mGraphImageResX;
    Setting mGraphImageResY;
    Setting mExportOptical;
    Setting mExportGraph;
    Setting mExportMode;

protected:
    void accept();
};

#endif // EXPORTDIALOG_H
