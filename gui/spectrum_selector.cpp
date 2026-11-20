#include "spectrum_selector.h"
#include "spectrum/spectrum_disposer.h"
#include <QDebug>
#include <QColor>

int SpectrumSelector::mColorInstanceCount = 0;

SpectrumSelector::SpectrumSelector(QWidget *parent) : QWidget(parent)
{
    mLayout = new QHBoxLayout();
    mLayout->setMargin(0);
    mSpectrumComboBox = new QComboBox();
    mSpectrumComboBox->setToolTip("<html><head/><body><p>"+
                                  tr("Select the spectrum which shall be drawn.")+
                                  "</p></body></html>");

    mLayout->addWidget(mSpectrumComboBox);
    this->setLayout(mLayout);

    updateAvailableSpectrums();
    connect(SpectrumDisposer::getInstance(), SIGNAL(spectrumsChanged()), this, SLOT(updateAvailableSpectrums()));

    setSpectrum(mSpectrumComboBox->currentText());
    connect(mSpectrumComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(setSpectrum(QString)));
}

SpectrumSelector::SpectrumSelector(bool deletable, bool colorSelectable, QWidget *parent) : SpectrumSelector(parent)
{
    setDeletable(deletable);
    if (colorSelectable)
    {
        mColorComboBox = new QComboBox();
        mColorComboBox->setFixedWidth(40);
        mColorComboBox->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
        mColorComboBox->setToolTip("<html><head/><body><p>"+
                                   tr("Select the color which shall be used for drawing its graph.")+
                                   "</p></body></html>");
        mLayout->insertWidget(0,mColorComboBox);

        QStringList colors;
        colors << "white" << "aqua" << "blue" << "green" << "red" << "orange" << "purple" << "yellow";
        for (QString color : colors)
        {
            QColor c;
            c.setNamedColor(color);
            QPixmap p(20,20);
            p.fill(c);
            mColorComboBox->addItem(QIcon(p),color);
        }
        mColorComboBox->setCurrentIndex(mColorInstanceCount % mColorComboBox->count());
        // connect spectrum whith new color combo box
        setSpectrum(mSpectrumComboBox->currentText());

        mColorInstanceCount++;
    }
}

SpectrumSelector::~SpectrumSelector()
{

}

void SpectrumSelector::updateAvailableSpectrums()
{
    QString lastSpectrum = mSpectrumComboBox->currentText();
    mSpectrumComboBox->clear();
    for (Spectrum* s : SpectrumDisposer::getInstance()->getSpectrums())
    {
        mSpectrumComboBox->addItem(s->mName);
        if (s->mName == lastSpectrum && mSpectrum != nullptr)
            mSpectrumComboBox->setCurrentText(s->mName);
    }
}

void SpectrumSelector::setSpectrum(QString spec)
{
    qDebug() << "[SpecSelector] Set spectrum" << spec;
    if (mSpectrum != nullptr)
    {
        if (mColorComboBox != nullptr)
            disconnect(mColorComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(requestFrame()));
        disconnect(mSpectrum, SIGNAL(newSpectrumData(SpectrumData)),this, SLOT(onNewSpectrumData(SpectrumData)));
        emit unbindSpectrum(mSpectrum);
    }
    mSpectrumComboBox->setCurrentText(spec);
    mSpectrum = SpectrumDisposer::getInstance()->getSpectrum(spec);
    if (mSpectrum != nullptr)
    {
        emit bindSpectrum(mSpectrum);
        if (mColorComboBox != nullptr)
            connect(mColorComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(requestFrame()));
        connect(mSpectrum, SIGNAL(newSpectrumData(SpectrumData)),this, SLOT(onNewSpectrumData(SpectrumData)));
        requestFrame();
    }
}

void SpectrumSelector::onNewSpectrumData(SpectrumData d)
{
    if (mColorComboBox != nullptr && mColorComboBox->currentText() != "")
    {
        d.mGraphColor.setNamedColor(mColorComboBox->currentText());
    }
    emit newSpectrumData(d);
}

void SpectrumSelector::setDeletable(bool d)
{
    if (d && mDeleteButton == nullptr)
    {
        mDeleteButton = new QPushButton("X");
        mDeleteButton->setFixedSize(22,22);
        mDeleteButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        mLayout->addWidget(mDeleteButton);
        connect(mDeleteButton,SIGNAL(clicked()),this,SLOT(onDeleteClicked()));
    }
    else if (!d && mDeleteButton != nullptr)
    {
        disconnect(mDeleteButton,SIGNAL(clicked()),this,SLOT(onDeleteClicked()));
        mLayout->removeWidget(mDeleteButton);
        delete mDeleteButton;
        mDeleteButton = nullptr;
    }
}

void SpectrumSelector::onDeleteClicked()
{
    emit destroy(this);
    setSpectrum("");
    deleteLater();
}

void SpectrumSelector::requestFrame()
{
    // We have to request the frame of all cameras to ensure that the colors
    // are rendered correctly. Otherwise the same spectrum with two colors
    // can cause outdated results after the update of the spectrum.
    for (Spectrum* s : SpectrumDisposer::getInstance()->getSpectrums())
    {
        if (s != nullptr && s->getCamera() != nullptr)
            s->getCamera()->requestFrame();
    }
}

