#ifndef CAMERADISPOSER_H
#define CAMERADISPOSER_H

#include <QObject>
#include "camera.h"

class CameraDisposer : public QObject
{
    Q_OBJECT
    friend class Camera;
public:
    static CameraDisposer& getInstance();

    Camera* createCVCamera(int devID);
    Camera* createCVCamera(int devID, cv::VideoCapture *capture);
    Camera* createFileCamera(QString file);
    void deleteCamera(Camera* c);
    void deleteAllCameras();

    QList<Camera*> getCameras();
    static QList<int> getAvailableDeviceIDs();
    bool devAlreadyExists(int dev) const;
    QList<int> getActiveDevices() const;

signals:
    void cameraTimout();

protected:
    void addCamera(Camera* c);
    void removeCamera(Camera* c);

private:
    CameraDisposer();
    ~CameraDisposer();
    CameraDisposer(CameraDisposer const& copy);
    CameraDisposer& operator=(CameraDisposer const& copy);
    QList<Camera*> mCameras;
};

#endif // CAMERADISPOSER_H
