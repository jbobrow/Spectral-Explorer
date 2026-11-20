#include "camera_dummy.h"

CameraDummy::CameraDummy() : Camera(QVariant(0))
{
}

void CameraDummy::start()
{
    mIsStarted = true;
}

bool CameraDummy::isVirtual()
{
    return false;
}

