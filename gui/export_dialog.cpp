#include "export_dialog.h"
#include "ui_export_dialog.h"

#include <QFileDialog>
#include <QTextStream>
#include <QPainter>
#include <QSettings>
#include <QDebug>

#include "spectrum/spectrum_optical_drawing.h"

ExportDialog::ExportDialog(QList<Spectrum*> spectrums,
                           SpectrumDrawingStrategy *drStrategy,
                           QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog),
    mGraphImageResX("Export/GraphImageResX",1280),
    mGraphImageResY("Export/GraphImageResY",720),
    mExportOptical("Export/ExportOptical",true),
    mExportGraph("Export/ExportGraph",true),
    mExportMode("Export/ExportMode",ExportCSV)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    mSpectrums = spectrums;
    mDrawingStrategy = drStrategy;
    mGraphImageResX.load(mSettings);
    mGraphImageResY.load(mSettings);
    mExportGraph.load(mSettings);
    mExportOptical.load(mSettings);
    mExportMode.load(mSettings);

    mGraphImageResX.addWidget(ui->resXSpin);
    mGraphImageResY.addWidget(ui->resYSpin);
    mExportGraph.addWidget(ui->graphCheckBox);
    mExportOptical.addWidget(ui->opticalCheckBox);

    switch (mExportMode.value().toInt()) {
    case ExportCSV:
        ui->csvMode->setChecked(true);
        break;
    case ExportImage:
        ui->imageMode->setChecked(true);
        break;
    default:
        qWarning() << "ExportDialog: Unhandled export mode.";
        break;
    }
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::accept() {
    hide();

    if (ui->csvMode->isChecked()) mExportMode.setValue(QVariant(ExportCSV));
    if (ui->imageMode->isChecked()) mExportMode.setValue(QVariant(ExportImage));

    mGraphImageResX.save(mSettings);
    mGraphImageResY.save(mSettings);
    mExportGraph.save(mSettings);
    mExportOptical.save(mSettings);
    mExportMode.save(mSettings);

    switch (mExportMode.value().toInt()) {
    case ExportCSV:
        exportCSV();
        break;
    case ExportImage:
        exportImage();
        break;
    default:
        qWarning() << "ExportDialog::accept(): Unhandled export mode.";
        break;
    }

    QDialog::accept();
}

void ExportDialog::exportCSV()
{
    // Get file name
    QSettings settings;
    QString filename = QFileDialog::getSaveFileName(this,
                                        tr("Export Raw Data"),
                                        settings.value("GuiBehaviour/LastExportDirectory").toString(),
                                        tr("CSV Files (*.csv)"));
    if (filename.isEmpty() || mSpectrums.isEmpty())
        return;

    // Create CSV StringList table
    QStringList csvList;
    for (int dataCol = 0; dataCol < mSpectrums.length(); dataCol++)
    {

        SpectrumData data = mSpectrums[dataCol]->getSpectrumData();
        QStringList newCSV = data.exportCSV();
        for (int line = 0; line < newCSV.length(); line++)
        {
            if (line >= csvList.length())
            {
                QString initLine = "";
                for (int s = 0; s < dataCol; s++) initLine += ';';
                initLine += newCSV[line];
                csvList.append(initLine);
            }
            else
            {
                csvList[line].append(";" + newCSV[line]);
            }
        }
    }

    // Add head
    csvList.insert(0,"");
    csvList.insert(1,"");
    for (int dataCol = 0; dataCol < mSpectrums.length(); dataCol++)
    {
        csvList[0] += mSpectrums[dataCol]->mName + ";;";
        csvList[1] += tr("Wavelength in nm") + ";" + tr("Intensity in %") + ";";
    }

    // Join and modify csv
    QString csv = csvList.join('\n');
    csv.replace('.',',');

    // Save file
    QFile file(filename);
    if(filename != "" && file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QDir dir;
        QString path = dir.absoluteFilePath(filename);
        path.remove(path.split("/").last());
        settings.setValue("GuiBehaviour/LastExportDirectory",path);

        QTextStream streamOut(&file);
        streamOut << csv;
        file.close();
    }
}

void ExportDialog::exportImage()
{
    int width = mGraphImageResX.value().toInt();
    int height = mGraphImageResY.value().toInt();
    QImage img(width,height,QImage::Format_ARGB32_Premultiplied);
    QSettings settings;

    QString filename = QFileDialog::getSaveFileName(this,
                                        tr("Save Graph as Image"),
                                        settings.value("GuiBehaviour/LastExportDirectory").toString(),
                                        tr("Image File (*.png)"));
    if (filename.isEmpty() || mSpectrums.isEmpty())
        return;

    QDir dir;
    QString path = dir.absoluteFilePath(filename);
    path.remove(path.split("/").last());
    settings.setValue("GuiBehaviour/LastExportDirectory",path);

    QPainter painter;
    painter.begin(&img);
    if (mExportGraph.value().toBool() && mExportOptical.value().toBool())
    {
        int opticalHeight = height/5;

        QList<SpectrumData> opticalData;
        while (opticalData.length() < opticalHeight)
            opticalData.append(mSpectrums[0]->getSpectrumData());
        SpectrumOpticalDrawing dr(&opticalData);
        dr.draw(&painter,QRect(0,0,width,opticalHeight));

        painter.translate(0,opticalHeight);
        mDrawingStrategy->draw(&painter,QRect(0,0,width,height-opticalHeight));
    }
    else if (mExportGraph.value().toBool())
    {
        mDrawingStrategy->draw(&painter,QRect(0,0,width,height));
    }
    else if (mExportOptical.value().toBool())
    {
        QList<SpectrumData> opticalData;
        while (opticalData.length() < height)
            opticalData.append(mSpectrums[0]->getSpectrumData());
        SpectrumOpticalDrawing dr(&opticalData);
        dr.draw(&painter,QRect(0,0,width,height));
    }

    painter.end();
    img.save(filename,"PNG");
}
