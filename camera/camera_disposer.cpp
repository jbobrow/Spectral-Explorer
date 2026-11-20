#include "camera_disposer.h"
#include "camera_file.h"
#include "camera_cv.h"
#include "camera_dummy.h"
#include "camera_detector.h"
#include "gui/settings.h"

#include <QDebug>

CameraDisposer::CameraDisposer()
{
}

CameraDisposer::~CameraDisposer()
{
    deleteAllCameras();
}

CameraDisposer& CameraDisposer::getInstance()
{
    static CameraDisposer instance;     // Guaranteed to be destroyed.
                                        // Instantiated on first use.
    return instance;
}

Camera *CameraDisposer::createCVCamera(int devID)
{
    for (int i = 0; i < mCameras.length(); i++)
    {
        if (mCameras[i]->getCamID() == QVariant(devID)) {
            qDebug() << "[CamDisp]" << Q_FUNC_INFO << "Camera already exists. Return the existing camera:" << mCameras[i]->getCamID();
            return mCameras[i];
        }
    }
    Camera *c = new CameraCV(devID);
    return c;
}

Camera *CameraDisposer::createCVCamera(int devID, cv::VideoCapture *capture)
{
    Camera *c = new CameraCV(devID,capture);
    return c;
}

Camera *CameraDisposer::createFileCamera(QString file)
{
    Camera *c = new CameraFile(file);
    int count = 0;
    for (int i = 0; i < mCameras.length(); i++)
    {
        if (mCameras[i] != c && mCameras[i]->getCamID().toString().startsWith(file))
            count++;
    }
    QString newID = file + QString::number(count);
    c->setCamID(QVariant(newID));
    return c;
}

void CameraDisposer::deleteCamera(Camera *c)
{
    qDebug() << "[CamDisp] CameraDisposer delete camera";
    int idx = mCameras.indexOf(c);
    if (idx != -1) {
        delete mCameras[idx];
    }
    else
        qDebug() << "[CamDisp]" << Q_FUNC_INFO << "Camera is already destroyed.";
}

void CameraDisposer::addCamera(Camera* c)
{
    if (c == nullptr)
    {
        qWarning() << "[CamDisp] CameraDisposer::addCamera: c is nullptr.";
        return;
    }
    if (!mCameras.contains(c)) {
        mCameras.append(c);
        connect(c, SIGNAL(cameraTimeout()), this, SIGNAL(cameraTimout()));
    }
}

void CameraDisposer::removeCamera(Camera* c)
{
    qDebug() << "[CamDisp] CameraDisposer remove camera: " << c->getCamID();
    int idx = mCameras.indexOf(c);
    if (idx != -1)
        mCameras.removeAt(idx);
}

QList<Camera*> CameraDisposer::getCameras()
{
    return mCameras;
}

QList<int> CameraDisposer::getAvailableDeviceIDs()
{
    int detectionMode = Settings::getInstance().getCameraDetectionMode();
    CameraDetector *detector;
    switch (detectionMode) {
    case 0:
        detector =  new CameraDetectorManual(); break;
    case 1:
        detector = new CameraDetectorCV(); break;
    case 2:
        detector = new CameraDetectorQt(); break;
    case 3:
        detector = new CameraDetectorMaximum(); break;
    default:
        detector = new CameraDetectorManual();
        qCritical() << Q_FUNC_INFO << "[CamDisp] Unknown CameraDetector" << detectionMode;
        break;
    }
    QList<int> devs = detector->getDevices();
    delete detector;
    return devs;
}

void CameraDisposer::deleteAllCameras()
{
    qDebug() << "[CamDisp] Delete all cameras.";
    qDebug() << "[CamDisp] Cameras to destroy:" << mCameras;
    for (int i = 0; i < mCameras.length(); i++)
    {
        qDebug() << "[CamDisp] Destroy" << mCameras[i]->getCamID();
        delete mCameras[i];
        // The destructor of Camera calls removeCam so that
        // it removed from mCameras
        i--;
    }
    qDebug() << "[CamDisp]" << "Clear mCameras";
    mCameras.clear();
}

//-----------------------------------------------------------------------------
// Helpers
//-----------------------------------------------------------------------------
bool CameraDisposer::devAlreadyExists(int dev) const
{
    for (int i = 0; i < mCameras.length(); i++)
    {
        if (mCameras[i]->getCamID() == QVariant(dev))
            return true;
    }
    return false;
}

QList<int> CameraDisposer::getActiveDevices() const
{
    QList<int> devs;

    for (int i = 0; i < mCameras.length(); i++)
    {
        bool isInt;
        int devID = mCameras[i]->getCamID().toInt(&isInt);
        if (isInt)
            devs.append(devID);
    }

    return devs;
}
