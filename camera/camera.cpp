#include "camera.h"
#include "camera_disposer.h"

Camera::Camera(QVariant camID)
{
    mCamID = camID;
    CameraDisposer::getInstance().addCamera(this);
}

Camera::~Camera()
{
    CameraDisposer::getInstance().removeCamera(this);
}

void Camera::setActive(bool a)
{
    mActive = a;
    if (!mIsStarted) start();
}

bool Camera::isTimedOut()
{
    return false;
}

bool Camera::exposureAvailable() const
{
    return false;
}

bool Camera::gainAvailable() const
{
    return false;
}

bool Camera::brightnessAvailable() const
{
    return false;
}

bool Camera::sharpnessAvailable() const
{
    return false;
}

bool Camera::rowInRange(int r)
{
    return (r >= 0 && r < getHeight());
}
int Camera::getHeight()
{
    return mHeight;
}
int Camera::getWidth()
{
    return mWidth;
}
cv::Mat Camera::getFrame() {
    return mFrame;
}

void Camera::requestFrame()
{
    if (mFrame.rows != 0)
        updateFrame(mFrame);
}

void Camera::updateFrame(cv::Mat frame)
{
    mFrame = frame;
    mHeight = mFrame.rows;
    mWidth = mFrame.cols;
    rotate_image_90n(mFrame, mFrame, mAngle);
    if (mFrame.rows!=mOldH || mFrame.cols!=mOldW)
    {
        emit cameraSizeChanged(mFrame.cols, mFrame.rows);
        mOldH = mFrame.rows;
        mOldW = mFrame.cols;
    }

    emit newFrame(mFrame);
}

QVariant Camera::getCamID() const
{
    return mCamID;
}

void Camera::setCamID(const QVariant &camID)
{
    mCamID = camID;
}

//-----------------------------------------------------------------------------
// Camera rotation
//-----------------------------------------------------------------------------
void Camera::rotate90()
{
    mAngle += 90;
    if (mAngle >= 360)
        mAngle -= 360;
    requestFrame();
}

void Camera::resetRotation()
{
    mAngle = 0;
    requestFrame();
}

void Camera::rotate_image_90n(cv::Mat &src, cv::Mat &dst, int angle)
{
   if(src.data != dst.data){
       src.copyTo(dst);
   }

   angle = ((angle / 90) % 4) * 90;

   //0 : flip vertical; 1 flip horizontal
   bool const flip_horizontal_or_vertical = angle > 0 ? 1 : 0;
   int const number = std::abs(angle / 90);

   for(int i = 0; i != number; ++i){
       cv::transpose(dst, dst);
       cv::flip(dst, dst, flip_horizontal_or_vertical);
   }
}

