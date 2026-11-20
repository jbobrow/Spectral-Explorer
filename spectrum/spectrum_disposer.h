#ifndef SPECTRUMDISPOSER_H
#define SPECTRUMDISPOSER_H

#include <QList>
#include <QTimer>
#include "spectrum.h"

class SpectrumDisposer : public QObject
{
    Q_OBJECT
public:
    static SpectrumDisposer* getInstance();
    void addSpectrum(Spectrum* c, QString name);
    void renameSpectrum(Spectrum* c, QString newName);
    Spectrum* removeSpectrum(Spectrum* c);
    bool spectrumForCamAvailable(int dev);
    QString getAvailableName(QString suggestion);
    int getAvailableID();
    QList<Spectrum*> getSpectrums();
    Spectrum* getSpectrum(QString name);

public slots:
    void startHardwareUpdater();
    void updateHardwareSpectrums();
    void clearSpectrums();

signals:
    void spectrumsChanged();
    void initializeCamera(int devID);

private:
    SpectrumDisposer();
    ~SpectrumDisposer();
    SpectrumDisposer(SpectrumDisposer const& copy);
    SpectrumDisposer& operator=(SpectrumDisposer const& copy);
    QList<Spectrum*> mSpectrums;
    QTimer *mUpdateTimer = nullptr;
};

#endif // SPECTRUMDISPOSER_H
