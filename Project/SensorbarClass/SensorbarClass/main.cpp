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
	PointList prev_nodes;
	Camera camera;
	Mouse *mouse;
public:
	SensorbarApp(){
		this->window = this->createWindow();
		
		this->window->setSize(WIDTH,HEIGHT);
		this->camera.SetCaptureSize(WIDTH,HEIGHT);
		this->camera.StartCapture(0);
		
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
		biImage display;

		//カメラから取り込み
		*prev = *background;
		*background = camera.Image();
		*temp = *background;
		display = *background;
		
		//特徴点抽出
		prev_nodes = ImageUtil::Harris(SensorBarLogic::getWorkImage(*prev), 0.5, 5);
		std::vector<Point> curr_nodes = ImageUtil::Harris(SensorBarLogic::getWorkImage(*background), 0.5, 5);

		std::vector<SensorBarLogic::Feature> optical_flow = SensorBarLogic::getOpticalFlow(SensorBarLogic::getWorkImage(*prev), SensorBarLogic::getWorkImage(*background), prev_nodes, curr_nodes, 5);
		SensorBarLogic::drawOpticalFlow(display, optical_flow);

		//ImageUtil::Harris(display, 0.5, 5);

		/*
		//明るさ調整
		ImageUtil::Brightness(*temp,base);

		SensorBarLogic::Range(*temp);//肌色抽出

		//細線化処理し、特徴点を抽出する
		hand = SensorBarLogic::SamplingHand(*temp);
		SensorBarLogic::DrawPointList(*temp,hand,display);
		//*/

		//display = *temp;
		img->Paste(0,0,display);
	}
};

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow){
	SensorbarApp app;
	app.winMain(hCurInst);
}