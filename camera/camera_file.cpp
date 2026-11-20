#include "camera_file.h"

CameraFile::CameraFile(QString file) : Camera(QVariant(file))
{
    mFile = file;
}

void CameraFile::start()
{
    mIsStarted = true;
    cv::Mat loaded = cv::imread(mFile.toLocal8Bit().constData(), CV_LOAD_IMAGE_COLOR);
    if (!loaded.empty())
    {
        loaded.copyTo(mFrame);
        updateFrame(mFrame);
    }
}

bool CameraFile::isVirtual()
{
    return true;
}
