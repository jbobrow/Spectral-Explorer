#ifndef CAMERADETECTOR_H
#define CAMERADETECTOR_H

#include <QList>


class CameraDetector
{
public:
    CameraDetector();
    virtual ~CameraDetector() {}
    virtual QList<int> getDevices() = 0;
};

class CameraDetectorManual : public CameraDetector
{
    virtual QList<int> getDevices();
};


class CameraDetectorMaximum : public CameraDetector
{
    virtual QList<int> getDevices();

};

class CameraDetectorCV : public CameraDetector
{
    virtual QList<int> getDevices();
};

class CameraDetectorCVStupid : public CameraDetector
{
    virtual QList<int> getDevices();
};

class CameraDetectorQt : public CameraDetector
{
    virtual QList<int> getDevices();

};



#endif // CAMERADETECTOR_H
