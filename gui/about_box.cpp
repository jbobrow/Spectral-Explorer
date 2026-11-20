#include "about_box.h"
#include "ui_about_box.h"

AboutBox::AboutBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutBox)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

//    QPalette pal;
//    pal.setColor(QPalette::Background, Qt::white);
//    setPalette(pal);
//    setStyleSheet("background-color:white;");

    ui->titleLabel->setText(QString("Spectral-Explorer ").append(APP_VERSION));
    ui->buildDateLabel->setText(QString(tr("Built on ") + __DATE__ + ' ' + __TIME__));
    ui->revisionLabel->setText("");
}

AboutBox::~AboutBox()
{
    delete ui;
}

void AboutBox::on_pushButton_clicked()
{
    close();
}
