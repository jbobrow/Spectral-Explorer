#include "spectrum_disposer.h"
#include "camera/camera_disposer.h"
#include "camera/camera_detector.h"
#include "gui/settings.h"
#include <QDebug>

SpectrumDisposer::SpectrumDisposer()
{
}

SpectrumDisposer::~SpectrumDisposer()
{
    while (!mSpectrums.isEmpty())
        delete mSpectrums.takeFirst();
    if (mUpdateTimer != nullptr)
        mUpdateTimer->stop();
}

void SpectrumDisposer::clearSpectrums()
{
    for (int i = 0; i < mSpectrums.size(); i++)
    {
        if (mSpectrums[i]->getCamera()->isVirtual())
        {
            Spectrum *s = mSpectrums[i];
            removeSpectrum(s);
            i--;
        }
    }
}

SpectrumDisposer *SpectrumDisposer::getInstance()
{
    static SpectrumDisposer instance;     // Guaranteed to be destroyed.
                                        // Instantiated on first use.
    return &instance;
}

void SpectrumDisposer::startHardwareUpdater()
{
    mUpdateTimer = new QTimer(this);
    connect(mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateHardwareSpectrums()));
    mUpdateTimer->start(5000);
    // TODO: Fix timing hack (check if onBindSpectrum is connected)
    QTimer::singleShot(1000,this, SLOT(updateHardwareSpectrums()));
}

void SpectrumDisposer::updateHardwareSpectrums()
{
    // TODO: Search for cameras on another thread to avoid micro freezes in ui
    qDebug() << "[SpecDisp] Update hardware spectrums.";
    for (int i = 0; i < mSpectrums.length(); i++)
    {
        Camera *c = mSpectrums[i]->getCamera();
        if (c->isTimedOut())
        {
            qDebug() << "[SpecDisp] Camera of spectrum timed out: " << mSpectrums[i]->mName;
            // Delete all cameras for the case that camera 0 was unplugged and camera 0 will
            // replace camera 0
            while (!mSpectrums.isEmpty())
            {
                delete removeSpectrum(mSpectrums.first());
            }
            break;
        }
    }

    QList<int> devs = CameraDisposer::getAvailableDeviceIDs();
    qDebug() << "[SpecDisp] Found following devices:" << devs;

    for (int i = 0; i < mSpectrums.length(); i++)
    {
        Camera *c = mSpectrums[i]->getCamera();
        if (!c->isVirtual() && !devs.contains(c->getCamID().toInt()))
        {
            qDebug() << "[SpecDisp] Camera of spectrum isn't detected anymore: " << mSpectrums[i]->mName;
            delete removeSpectrum(mSpectrums[i]);
            i--;
        }
    }

    Spectrum *s;
    for (int i = 0; i < devs.length(); i++)
    {
        if (!spectrumForCamAvailable(devs[i]))
        {
            qDebug() << "[SpecDisp] Initialize camera" << devs[i];
            emit initializeCamera(devs[i]);
            s = new Spectrum(devs[i]);
        }
    }
}

void SpectrumDisposer::addSpectrum(Spectrum *c, QString name)
{
    if (!mSpectrums.contains(c))
    {
        c->mName = getAvailableName(name);
        c->mID = getAvailableID();
        mSpectrums.append(c);
        emit spectrumsChanged();
    }
}

void SpectrumDisposer::renameSpectrum(Spectrum *c, QString newName)
{
    if (newName == c->mName)
        return;
    if (mSpectrums.contains(c))
    {
        c->mName = getAvailableName(newName);
        emit spectrumsChanged();
    }
}

Spectrum *SpectrumDisposer::removeSpectrum(Spectrum* c)
{
    Spectrum *s = nullptr;
    int idx = mSpectrums.indexOf(c);
    if (idx != -1) {
        s = mSpectrums[idx];
        mSpectrums.removeAt(idx);
    }

    emit spectrumsChanged();
    return s;
}

bool SpectrumDisposer::spectrumForCamAvailable(int dev)
{
    QList<Spectrum*> specs = getSpectrums();
    for (int j = 0; j < specs.length(); j++)
    {
        QVariant idVariant = specs[j]->getCamera()->getCamID();
        bool b;
        int id = idVariant.toInt(&b);
        if (b && id == dev)
            return true;
    }
    return false;
}

QList<Spectrum*> SpectrumDisposer::getSpectrums()
{
    return mSpectrums;
}

Spectrum* SpectrumDisposer::getSpectrum(QString name)
{
    foreach (Spectrum *s, mSpectrums)
        if (s->mName == name)
            return s;
    return nullptr;
}

QString SpectrumDisposer::getAvailableName(QString suggestion)
{
    bool occupied = false;
    for (int j = 0; j < mSpectrums.length(); j++)
        if (mSpectrums[j]->mName == suggestion)
            occupied = true;
    if (!occupied)
        return suggestion;

    for (int i = 1; i < 100; i++)
    {
        QString n = suggestion + "#" + QString::number(i);
        bool occupied = false;
        for (int j = 0; j < mSpectrums.length(); j++)
            if (mSpectrums[j]->mName == n)
                occupied = true;
        if (!occupied)
            return n;
    }
    return "";
}

int SpectrumDisposer::getAvailableID()
{
    for (int i = 0; i < 1000; i++)
    {
        bool occupied = false;
        for (int j = 0; j < mSpectrums.length(); j++)
            if (mSpectrums[j]->mID == i)
                occupied = true;
        if (!occupied)
            return i;
    }
    return -1;
}
