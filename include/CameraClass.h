#ifndef CAMERA_INCLUDE
#define CAMERA_INCLUDE

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_util/biImageClass.h>
#include <image_util/ImageUtilClass.h>
#include <windows.h>

#define THREADMODE
#define CV242

#ifdef CV210
#ifdef _DEBUG
//Debug���[�h�̏ꍇ
#pragma comment(lib,"cv210d.lib")
#pragma comment(lib,"cxcore210d.lib")
#pragma comment(lib,"cvaux210d.lib")
#pragma comment(lib,"highgui210d.lib")
#else
//Release���[�h�̏ꍇ
#pragma comment(lib,"cv210.lib")
#pragma comment(lib,"cxcore210.lib")
#pragma comment(lib,"cvaux210.lib")
#pragma comment(lib,"highgui210.lib")
#endif
#elif defined CV220
#ifdef _DEBUG
#pragma comment(lib,"opencv_core220d.lib")
#pragma comment(lib,"opencv_highgui220d.lib")
#pragma comment(lib,"opencv_imgproc220d.lib")
#pragma comment(lib,"opencv_objdetect220d.lib")
#else
//Release���[�h�̏ꍇ
#pragma comment(lib,"opencv_core220.lib")
#pragma comment(lib,"opencv_highgui220.lib")
#pragma comment(lib,"opencv_imgproc220.lib")
#pragma comment(lib,"opencv_objdetect220.lib")
#endif
#elif defined CV242
#ifdef _DEBUG
#pragma comment(lib,"opencv_core242d.lib")
#pragma comment(lib,"opencv_highgui242d.lib")
#pragma comment(lib,"opencv_imgproc242d.lib")
#pragma comment(lib,"opencv_objdetect242d.lib")
#else
//Release���[�h�̏ꍇ
#pragma comment(lib,"opencv_core242.lib")
#pragma comment(lib,"opencv_highgui242.lib")
#pragma comment(lib,"opencv_imgproc242.lib")
#pragma comment(lib,"opencv_objdetect242.lib")
#endif
#endif

typedef struct capture_data_tag{
	biImage *image;
	char flag;
	int width;
	int height;
	bool status;	//false=��~�@true=�N��
	CvCapture *capture;
}CaptureData;

DWORD WINAPI Capture(void* data);

class Camera{
private:
	biImage img;				//�摜
	HANDLE thread;			//�X���b�h�n���h��
	DWORD thid;				//�߂�l
	CaptureData data;
	IplImage *frame;
public:
	Camera();
	void SetCaptureSize(int width, int height);
	void StartCapture(int id);
	biImage Image();
	void StopCapture();
	void static getCvImage(IplImage *image, biImage *output);
};

//�L���v�`���摜���擾
inline biImage Camera::Image(){
	//�J���������荞��
	return *data.image;
}

#endif