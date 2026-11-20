#include "spectrum.h"
#include "spectrum_disposer.h"
#include <QSettings>
#include <QDebug>
#include "gui/settings.h"
#include "camera/camera_disposer.h"


Spectrum::Spectrum(QString file, QObject *parent) :
    QObject(parent)
{
    mCamera = CameraDisposer::getInstance().createFileCamera(file);
    initCam(file.split("/").last().split(".").first());

    if (Settings::getInstance().getAutoConfigFiles())
    {
        file.replace(".jpg",".config");
        file.replace(".png",".config");
        if (file != "")
        {
            getConfig()->load(file);
        }
        mCamera->requestFrame();
    }
}

Spectrum::Spectrum(int cameraDevice, QObject *parent) :
    QObject(parent)
{
    mCamera = CameraDisposer::getInstance().createCVCamera(cameraDevice);
    initCam(QString(tr("Device %1")).arg(cameraDevice));
}

void Spectrum::initCam(QString name)
{
    mConfig = new SpectrumConfig(mCamera);
    mFloatingSpectrum.create(0,800,CV_8UC3);
    connect(mCamera, SIGNAL(cameraSizeChanged(int,int)), this, SLOT(onCameraSizeChanged(int,int)));
    connect(mCamera, SIGNAL(newFrame(cv::Mat)), this, SLOT(onNewFrame(cv::Mat)));
    SpectrumDisposer::getInstance()->addSpectrum(this, name);
}

Spectrum::~Spectrum()
{
    qDebug() << "[Spec] Destroy spectrum" << mName;

    //Don't delete camera directly. This is the task of the CameraDisposer
    CameraDisposer::getInstance().deleteCamera(mCamera);
    mCamera = nullptr;
    delete mConfig;
    mConfig = nullptr;

    SpectrumDisposer::getInstance()->removeSpectrum(this);
}

void Spectrum::setActive(bool a)
{
    qDebug() << "[Spec]" << Q_FUNC_INFO << mName << a;
    mCamera->setActive(a);
}

bool Spectrum::isActive()
{
    return mCamera->isActive();
}

void Spectrum::onTargetSelected(MeasurementTarget targ, int mode)
{
    qDebug() << "[Spec] Spectrum" << mName << targ << "selected with mode" << mode;

    // Add target
    if (mode == 0)
        mTargets.append(targ);
    // Remove target
    if (mode == 1)
    {
        float wTol = 2.0f;
        float iTol = 0.01f;
        for (int t = 0; t < mTargets.length(); t++)
        {
            float w = targ.wavelength;
            float i = targ.intensity;
            if (mTargets[t].wavelength-wTol < w &&
                    mTargets[t].wavelength+wTol > w &&
                    mTargets[t].intensity-iTol < i &&
                    mTargets[t].intensity+iTol > i)
            {
                mTargets.removeAt(t);
            }
        }
    }

    mCamera->requestFrame();
}

void Spectrum::setFloatingSpecSize(int w, int h)
{
    mFloatingSpecW = w;
    mFloatingSpecH = h;
    mFloatingSpecR = (double)w/(double)h;
}

void Spectrum::setConfig(SpectrumConfig *config)
{
    delete mConfig;
    mConfig = config;
    mConfig->updateCameraProp();
}

void Spectrum::onCameraSizeChanged(int w,int h)
{
    qDebug() << "[Spec] Camera size changed.";
    mConfig->load();
    emit spectrumSizeChanged(w);

    mFloatingSpectrum.release();
    mFloatingSpectrum.create(0,w,CV_8UC3);
}

void Spectrum::onNewFrame(cv::Mat frame)
{
    if (mConfig == nullptr || frame.empty() || frame.cols <= 0 || frame.rows <= 0)
        return;

    int left = qMax(0, mConfig->getLeftOffset());
    int right = qMax(0, mConfig->getRightOffset());
    if (left + right >= frame.cols) {
        left = 0;
        right = 0;
    }

    int row = qBound(0, mConfig->getSpectrumRow(), frame.rows - 1);
    frame.row(row).copyTo(mSpectrumLine);

    int roiWidth = frame.cols - right - left;
    if (roiWidth <= 0)
        return;

    try {
        cv::Rect roi(left, row, roiWidth, 1);
        cv::Mat d(frame, roi);
        SpectrumData data(d, mID, mConfig->getCFLX1(), mConfig->getCFLX2(),
                          mConfig->getIntensityConfig());
        data.setTargets(mTargets);
        mSpectrumData = data;

        emit newSpectrumData(data);
    } catch (const cv::Exception &e) {
        qCritical() << "[Spec] OpenCV exception in onNewFrame:" << e.what();
    }
}

SpectrumData Spectrum::combineData(QList<SpectrumData> data)
{
    SpectrumData combination;
    combination = data[0];
    for (int i = 1; i < data.length(); i++)
        combination = combination + data[i];

    combination = combination * ((double) 1/data.length());

    return combination;
}
