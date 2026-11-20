#include "camera_detector.h"

#include <opencv2/highgui/highgui.hpp>
#include <QCameraInfo>
#include <QCoreApplication>
#include <QTime>
#include "camera_disposer.h"
#include "gui/settings.h"

CameraDetector::CameraDetector()
{

}

QList<int> CameraDetectorManual::getDevices()
{
    QList<int> devs;
    devs.append(Settings::getInstance().getManualCamera());
    return devs;
}

QList<int> CameraDetectorCV::getDevices()
{
    QList<int> devs;
    cv::VideoCapture *capture = new cv::VideoCapture;
    bool moreThanOneCam = false;

    devs.append(CameraDisposer::getInstance().getActiveDevices());

    // It's important to count down! Otherwise camera timeouts will occur
    for (int dev = 5; dev >= 0; dev--)
    {
        if( !CameraDisposer::getInstance().devAlreadyExists(dev) &&
                capture->open( dev ) )
        {
            // If you use prepend a single camera timeout will occur
            devs.append(dev);
            int exposure = static_cast<int>(capture->get(CV_CAP_PROP_EXPOSURE));
            qDebug() << "[CamDet] Exposure of device" << dev << "is" << exposure;

            // If the first camera is over and the second under an exposure of 6 the first camera
            // will have some timeouts. To prevent this, this code is executed everytime.
            // Old code:
            // if (exposure < -6)
            // {
            //     qDebug() << "[CamDet] Set exposure to -1 for device" << dev;
            //     capture->set(CV_CAP_PROP_EXPOSURE,-1);
            //     cv::Mat test;
            //     qDebug() << "[CamDet] Reading capture device" << dev << "returned" << capture->read(test);
            //     CameraDisposer::getInstance().createCVCamera(dev,capture)->start();
            //     capture = new cv::VideoCapture();
            // }
            // else capture->release();

            if (dev != 0) moreThanOneCam = true;
            if (moreThanOneCam)
            {
                qDebug() << "[CamDet] Set exposure to -1 for device" << dev;
                capture->set(CV_CAP_PROP_EXPOSURE,-1);
                cv::Mat test;
                qDebug() << "[CamDet] Reading capture device" << dev << "returned" << capture->read(test);

                // It's important to use start instead of setActive(true)
                CameraDisposer::getInstance().createCVCamera(dev,capture)->start();
                capture = new cv::VideoCapture();
            }
            // It is important that you don't release the capture device when only one
            // camera is detected. It's odd and don't know why it works.
            // else capture->release();
        }
    }
    // It is important that you don't delete the capture device when only one
    // camera is detected. It's odd and don't know why it works.
    if (moreThanOneCam) delete capture;
    return devs;
}

QList<int> CameraDetectorCVStupid::getDevices()
{
    QList<int> devs;
    cv::VideoCapture capture;

    devs.append(CameraDisposer::getInstance().getActiveDevices());

    for (int dev = 0; dev < 5; dev++)
    {
        if( !CameraDisposer::getInstance().devAlreadyExists(dev) &&
                capture.open( dev ) )
        {
            devs.append(dev);
            capture.release();
        }
    }
    return devs;
}

QList<int> CameraDetectorQt::getDevices()
{
    QList<int> devs; int i = 0;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        devs.append(i++);
        qDebug() << "[DetQt]" << cameraInfo.description();
    }
    return devs;
}

QList<int> CameraDetectorMaximum::getDevices()
{
    QList<int> devs;

    for (int dev = 0; dev < 3; dev++)
    {
        devs.append(dev);
    }
    return devs;
}
