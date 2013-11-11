#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
#endif

#include <gui/Application.h>
#include <CameraClass.h>
#include <gui/KeyClass.h>
#include <gui/MouseClass.h>
#include <gui/SquareClass.h>
#include <algorithm>
#include "SensorBarLogicClass.h"

#define WIDTH 320
#define HEIGHT 240

class SensorbarApp : public Application{
private:
	Window *window;
	Image *img;
	biImage *background;
	biImage *prev;
	biImage *temp;
	Camera camera;
	Mouse *mouse;
public:
	SensorbarApp(){
		this->window = this->createWindow();
		
		this->window->setSize(WIDTH,HEIGHT);
		this->camera.SetCaptureSize(WIDTH,HEIGHT);
		this->camera.StartCapture(1);
		
		this->background = new Image(WIDTH,HEIGHT);
		*this->background = Pixel(255);
		
		this->prev = new Image(WIDTH,HEIGHT);
		this->temp = new Image(WIDTH,HEIGHT);

		this->img = new Image(WIDTH,HEIGHT);
		this->mouse = new Mouse(*this->window);
		this->window->AddObject(img);
	}

	~SensorbarApp(){
		this->camera.StopCapture();
	}


	void main(){
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
		str += (String)"x=" + (Number)mouse->Pos().x;
		str += (String)" y=" + (Number)mouse->Pos().y;

		this->window->setTitle(str.c_str());
		
		img->Paste(0,0,*background);
		//*/
	}
};

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow){
	SensorbarApp app;
	app.winMain(hCurInst);
}