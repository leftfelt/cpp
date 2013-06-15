#include "CameraClass.h"

//����������
Camera::Camera(){
	data.status = 1;
	data.flag = 1;
	data.width = 640;
	data.height = 480;
}

//�L���v�`������T�C�Y��ݒ�i�Ή��T�C�Y�ɋ߂��T�C�Y�Ɏ����ݒ肳���j
void Camera::SetCaptureSize(int width, int height){
	data.width = width;
	data.height = height;
}

//�L���v�`�����J�n
void Camera::StartCapture(int id){
	//�J�����̏������i�J�����̑I���j
	data.capture = cvCaptureFromCAM(id);
	data.image = new biImage(data.width,data.height);
	data.status = true;
	//�捞�T�C�Y�̐ݒ�
	cvSetCaptureProperty (data.capture, CV_CAP_PROP_FRAME_WIDTH,  data.width);
	cvSetCaptureProperty (data.capture, CV_CAP_PROP_FRAME_HEIGHT, data.height);
	//�ʃX���b�h�Ŏ�荞�ݑ�����
	thread = (HANDLE)CreateThread(NULL,0,Capture,&this->data,0,&this->thid);
}

//�L���v�`�����I��
void Camera::StopCapture(){
	//�X���b�h�I��
	data.status = false;
	delete(data.image);
	CloseHandle(thread);
	//�J�������
	cvReleaseCapture (&data.capture);
}

//OpenCV�̉摜�`������ϊ�����
void Camera::getCvImage(IplImage *image, biImage *output){
	if(image->depth == IPL_DEPTH_8U && image->nChannels == 3 && image->imageData != NULL){
		for(int y = 0 ; y < image->height ; y++){
			for(int x = 0 ; x < image->width ; x++){
				output->Put(x,y,Pixel(
					(*(image->imageData+x*3+2+y*image->widthStep)),
					(*(image->imageData+x*3+1+y*image->widthStep)),
					(*(image->imageData+x*3+0+y*image->widthStep))
					));
			}
		}
	}
}

//================================================================================
DWORD WINAPI Capture(void* data){

	CaptureData *capture_data;
	capture_data = (CaptureData*)data;
	IplImage *image_data;
	while(capture_data->status){
		image_data = NULL;
		image_data = cvQueryFrame(capture_data->capture);
		if(image_data != NULL){
			Camera::getCvImage(image_data,capture_data->image);
		}
	}
	return 0;
}