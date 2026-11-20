#ifndef CAMERAFILE_H
#define CAMERAFILE_H

#include "camera/camera.h"

class CameraFile : public Camera
{
public:
    CameraFile(QString file);
    virtual void start();
    virtual bool isVirtual();

private:
    QString mFile;
};

#endif // CAMERAFILE_H
