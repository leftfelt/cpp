#include "CameraClass.h"

//初期化処理
Camera::Camera(){
	data.status = 1;
	data.flag = 1;
	data.width = 640;
	data.height = 480;
}

//キャプチャするサイズを設定（対応サイズに近いサイズに自動設定される）
void Camera::SetCaptureSize(int width, int height){
	data.width = width;
	data.height = height;
}

//キャプチャを開始
void Camera::StartCapture(int id){
	//カメラの初期化（カメラの選択）
	data.capture = cvCaptureFromCAM(id);
	data.image = new biImage(data.width,data.height);
	data.status = true;
	//取込サイズの設定
	cvSetCaptureProperty (data.capture, CV_CAP_PROP_FRAME_WIDTH,  data.width);
	cvSetCaptureProperty (data.capture, CV_CAP_PROP_FRAME_HEIGHT, data.height);
	//別スレッドで取り込み続ける
	thread = (HANDLE)CreateThread(NULL,0,Capture,&this->data,0,&this->thid);
}

//キャプチャを終了
void Camera::StopCapture(){
	//スレッド終了
	data.status = false;
	delete(data.image);
	CloseHandle(thread);
	//カメラ解放
	cvReleaseCapture (&data.capture);
}

//OpenCVの画像形式から変換する
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