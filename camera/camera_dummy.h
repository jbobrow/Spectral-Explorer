#ifndef CAMERADUMMY_H
#define CAMERADUMMY_H

#include "camera/camera.h"

class CameraDummy : public Camera
{
public:
    CameraDummy();

    virtual void start();
    virtual bool isVirtual();
};

#endif // CAMERADUMMY_H
