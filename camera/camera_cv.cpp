#include "camera_cv.h"

#include <QDebug>
#include <QThread>

#include "gui/settings.h"

CameraCV::CameraCV(int devID) : Camera(QVariant(devID))
{
    mDevID = devID;
    mCaptureWorker = nullptr;
    mCaptureThread = nullptr;
}

CameraCV::CameraCV(int devID, cv::VideoCapture *capture) : CameraCV(devID)
{
    mCapture = capture;
}

CameraCV::~CameraCV()
{
    qDebug() << "[CamCV]" << "Shut down CameraCV" << mDevID;
    qDebug() << "[CamCV]" << "Quit thread";
    if (mCaptureThread == nullptr)
        qCritical() << "[CamCV]" << "mCaptureThread is nullptr";
    else
    {
        if (mCaptureWorker != nullptr && mCaptureThread->isRunning())
            QMetaObject::invokeMethod(mCaptureWorker, "setActive", Qt::BlockingQueuedConnection, Q_ARG(bool, false));
        mCaptureThread->quit();
        qDebug() << "[CamCV]" << "Wait for thread termination";
        if (!mCaptureThread->wait(5000)) {
            qCritical() << "[CamCV]" << "Thread termination timeout. Terminating thread.";
            mCaptureThread->terminate();
            mCaptureThread->wait();
        }
        if (mCaptureWorker == nullptr)
            qCritical() << "[CamCV]" << "mCaptureWorker is nullptr";
        else
        {
            qDebug() << "[CamCV]" << "Delete CaptureWorker";
            delete mCaptureWorker;
            mCaptureWorker = nullptr;
        }
        qDebug() << "[CamCV]" << "Delete CaptureThread";
        delete mCaptureThread;
        mCaptureThread = nullptr;
    }
}

void CameraCV::start()
{
    if (mIsStarted) return;
    mIsStarted = true;

    qDebug() << "[CamCV]" << "Start CameraCV" << mDevID;

    if (mCapture == nullptr) {
        qDebug() << "[CamCV]" << "Create new capture device." << mDevID;

        mCapture = new cv::VideoCapture;

        if( !mCapture->isOpened() )
        {
            if( !mCapture->open( mDevID ) )
            {
                qCritical() << "[CamCV]" << "Can't open device" << mDevID;
                return;
            }
        }
    }
    else {
        qDebug() << "[CamCV]" << "Use existing capture device." << mDevID;
    }

    int exposure = static_cast<int>(mCapture->get(CV_CAP_PROP_EXPOSURE));
    qDebug() << "[CamCV] exposure" << exposure;

    mCapture->set(CV_CAP_PROP_FRAME_WIDTH,Settings::getInstance().getResX());
    mCapture->set(CV_CAP_PROP_FRAME_HEIGHT,Settings::getInstance().getResY());
    // DON'T SET FPS - It seems to contribute to camera timeout
    //capture->set(CV_CAP_PROP_FPS, 60);
    int width = static_cast<int>(mCapture->get(CV_CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(mCapture->get(CV_CAP_PROP_FRAME_HEIGHT));
    if (height == 0 || width == 0)
        qWarning() << "[CamCV]" << "mCapture->get(CV_CAP_PROP_FRAME_WIDTH/HEIGHT) returns 0";
    qDebug() << "[CamCV] resolution " << width << "x" << height;
    Settings::getInstance().setResX(width);
    Settings::getInstance().setResY(height);
    mCapture->set(CV_CAP_PROP_WHITE_BALANCE_U,3500);


    mCaptureWorker = new CaptureWorker(mCapture,mCamID.toInt());
    mCaptureThread = new QThread();
    mCaptureWorker->moveToThread(mCaptureThread);
    connect(this, &CameraCV::startCaptureThread, mCaptureWorker, &CaptureWorker::start);
    connect(this, &CameraCV::setCaptureThreadActive, mCaptureWorker, &CaptureWorker::setActive);
    connect(mCaptureWorker, &CaptureWorker::newFrame, this, &CameraCV::onNewWorkerFrame);
    connect(mCaptureWorker, &CaptureWorker::cameraTimeout, this, &CameraCV::cameraTimeout);

    qDebug() << "[CamCV]" << "Start device" << mDevID;
    mCaptureThread->start();
    emit startCaptureThread();
    emit setCaptureThreadActive(mActive);
}

void CameraCV::setActive(bool a)
{
    Camera::setActive(a);
    emit setCaptureThreadActive(a);
}

bool CameraCV::isVirtual()
{
    return false;
}

bool CameraCV::isTimedOut()
{
    return checkCaptureWorker() && mCaptureWorker->getTimoutCnt() >= 5;
}

void CameraCV::onNewWorkerFrame()
{
    if (!checkCaptureWorker()) return;
    updateFrame(mCaptureWorker->getFrame());
}

//-------------------------------------------------------------------------------------------------
// Camera Settings
//-------------------------------------------------------------------------------------------------
void CameraCV::setExposure(int exp)
{
    qDebug() << "[CamCV]" << "setExposure:" << exp;
    if (!checkCaptureWorker()) return;
    mCaptureWorker->setCvProp(CV_CAP_PROP_EXPOSURE,-exp);
}

void CameraCV::setGain(int gain)
{
    if (!checkCaptureWorker()) return;
    mCaptureWorker->setCvProp(CV_CAP_PROP_GAIN,gain);
}

void CameraCV::setBrightness(int br)
{
    if (!checkCaptureWorker()) return;
    mCaptureWorker->setCvProp(CV_CAP_PROP_BRIGHTNESS,br);
}

void CameraCV::setSharpness(int sh)
{
    if (!checkCaptureWorker()) return;
    mCaptureWorker->setCvProp(CV_CAP_PROP_SHARPNESS,sh);
}

void CameraCV::setAverageFrames(int num)
{
    if (!checkCaptureWorker()) return;
    mCaptureWorker->setAverageFrames(num);
}

bool CameraCV::exposureAvailable() const
{
    if (!checkCaptureWorker()) return false;
    return mCaptureWorker->cvPropAvailable(CV_CAP_PROP_EXPOSURE);
}

bool CameraCV::gainAvailable() const
{
    if (!checkCaptureWorker()) return false;
    return mCaptureWorker->cvPropAvailable(CV_CAP_PROP_GAIN);
}

bool CameraCV::sharpnessAvailable() const
{
    if (!checkCaptureWorker()) return false;
    return mCaptureWorker->cvPropAvailable(CV_CAP_PROP_SHARPNESS);
}

bool CameraCV::brightnessAvailable() const
{
    if (!checkCaptureWorker()) return false;
    return mCaptureWorker->cvPropAvailable(CV_CAP_PROP_BRIGHTNESS);
}

bool CameraCV::checkCaptureWorker() const
{
    if (mCaptureWorker == nullptr)
    {
        qWarning() << "[CamCV]" << "CaptureWorker is nullptr" << "ID: " << mCamID;
        return false;
    }
    return true;
}
