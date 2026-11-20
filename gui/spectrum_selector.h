#ifndef SPECTRUMSELECTOR_H
#define SPECTRUMSELECTOR_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include "spectrum/spectrum.h"
#include "spectrum/spectrum_data.h"

class SpectrumSelector : public QWidget
{
    Q_OBJECT
public:
    explicit SpectrumSelector(QWidget *parent = nullptr);
    SpectrumSelector(bool deletable, bool colorChosable, QWidget *parent = nullptr);
    ~SpectrumSelector();
    int getID() {return mSpectrum->mID;}
    Spectrum* getSpectrum() {return mSpectrum;}

signals:
    void newSpectrumData(SpectrumData d);
    void bindSpectrum(Spectrum* s);
    void unbindSpectrum(Spectrum* s);
    void destroy(SpectrumSelector* s);

public slots:
    void setSpectrum(QString spec);
    void updateAvailableSpectrums();
    void requestFrame();
    void setDeletable(bool d);

private slots:
    void onNewSpectrumData(SpectrumData d);
    void onDeleteClicked();


private:
    QHBoxLayout* mLayout;
    QComboBox* mColorComboBox = nullptr;
    QComboBox* mSpectrumComboBox = nullptr;
    QPushButton* mDeleteButton = nullptr;
    QColor mColor;
    Spectrum* mSpectrum = nullptr;
    static int mColorInstanceCount;
};

#endif // SPECTRUMSELECTOR_H
