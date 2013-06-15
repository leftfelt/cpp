#ifdef _DEBUG
	#pragma comment(lib,"Based.lib")
	#pragma comment(lib,"Flibd.lib")
	#pragma comment(lib,"Flib_wind.lib")
#else
	#pragma comment(lib,"Base.lib")
	#pragma comment(lib,"Flib.lib")
	#pragma comment(lib,"Flib_win.lib")
#endif

#include <WindowClass.h>
#include <CameraClass.h>
#include <ImageClass.h>
#include <ImageUtilClass.h>
#include <StringClass.h>
#include <KeyClass.h>
#include <MouseClass.h>
#include <SquareClass.h>
#include <algorithm>
#include "SensorBarLogicClass.h"

#define WIDTH 320
#define HEIGHT 240

Window wnd("main");
Image *img;
biImage *background;
biImage *prev;
biImage *temp;
Camera camera;

Mouse mouse(wnd);

void WindowInit(){

	wnd.Size(WIDTH,HEIGHT);
	camera.SetCaptureSize(WIDTH,HEIGHT);
	camera.StartCapture(1);

	background = new Image(WIDTH,HEIGHT);
	*background = Pixel(255);
	
	prev = new Image(WIDTH,HEIGHT);
	temp = new Image(WIDTH,HEIGHT);

	img = new Image(WIDTH,HEIGHT);
	wnd.AddObject(img);
	AddWindow(&wnd);
}

void WindowUpdate(){
	int base = 130;
	SensorBarLogic::Hand hand;

	//�J���������荞��
	*background = camera.Image();
	*temp = *background;

	temp->Size(
		WIDTH*SensorBarLogic::Work_Image_Percent,
		HEIGHT*SensorBarLogic::Work_Image_Percent
	); //�S�̓I�ɏ������d���̂ŉ摜�T�C�Y������������

	//���邳����
	//ImageUtil::Brightness(*temp,base);
	//*background = *temp;
	SensorBarLogic::Range(*temp);//���F���o

	//*�א����������A�����_�𒊏o����
	hand = SensorBarLogic::SamplingHand(*temp);
	SensorBarLogic::DrawPointList(*temp,hand,*background);
	//*background = *temp;

	//�^�C�g���o�[�Ƀ}�E�X���W��\������*/
	String str = "";
	str += (String)"x=" + (Number)mouse.Pos().x;
	str += (String)" y=" + (Number)mouse.Pos().y;

	wnd.Title(str.c_str());

	//*/
}

void WindowDraw(){
	img->Paste(0,0,*background);
}

void WindowDelete(){
	camera.StopCapture();
}

