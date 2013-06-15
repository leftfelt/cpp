#ifdef _DEBUG
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
	#pragma comment(lib,"testamentd.lib")
#else
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
	#pragma comment(lib,"testament.lib")
#endif

#include <gui/Application.h>
#include <gui/WindowClass.h>
#include <gui/ImageClass.h>
#include <gui/SquareClass.h>
#include <gui/MouseClass.h>
#include <gui/KeyClass.h>
#include <image_util/ImageUtilClass.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <testament/Testament.h>
#include <testament/Converter.h>
#include <testament/ClusterRepository.h>
#include <testament/RelationDataRepository.h>
#include <base/MathClass.h>
#include <CameraClass.h>
#include <boost/thread.hpp>
#include <sstream>

#define WIDTH 320
#define HEIGHT 240

class Clustering : public Application{
private:
	ClassifierSetting classifier_setting;
	TestamentSetting setting;
	Image *background;
	biImage img;
	Window *window;
	Testament *testament;
	Camera camera;
public:
	Clustering(){

		ImageUtil::Load(img,"../../../data/lenna.bmp");

		img.Size(200,200);
		background = new Image(WIDTH,HEIGHT);
		
		//ウィンドウ生成
		window = this->createWindow();
		window->setSize(WIDTH,HEIGHT);
		window->AddObject(this->background);
		
		//分類器設定
		classifier_setting.cluster_size = 10;
		classifier_setting.dimension_num = 128;
		classifier_setting.classification_num = 10;
		classifier_setting.data_min = -1;
		classifier_setting.data_max = 100;

		//変換機設定
		setting.converter_setting.convert_image_threshold = 500;
		setting.data_type = "image";
		setting.db_name = "knowledge.db";
		setting.equivocation = 10;
		setting.learn_num = 50;
		setting.relation_range = 10;
	
		//学習装置
		testament = new Testament(setting, new KMeansClassifier(classifier_setting));

		//カメラ設定
		camera.SetCaptureSize(320,240);
		camera.StartCapture(1);
		
		testament->truncate();

		//別スレッドで学習
		boost::thread learn([&](){
			while(1){
				try{
					testament->accept(this->camera.Image());
				}catch(std::exception e){
					//nice catch!
				}
				testament->learn();
			}
		});
	}

	~Clustering(){
		camera.StopCapture();
	}

	void main(){
		//差分をとって動いた部分の特徴点を学習
		background->Paste(0,0,camera.Image());
	}


	//特徴点を表示
	void displayFeaturePoint(std::vector<SurfFeature> surf_list){
		std::for_each(surf_list.begin(), surf_list.end(), [&](SurfFeature surf){
			for(int j = -1 ; j <= 1 ; j++)
				for(int i = -1 ; i <= 1 ; i++)
					this->background->Put(surf.point.pt.x+i, surf.point.pt.y+j, Pixel(0,0,255));
		});
	}
};

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow){
	Clustering clustering;
	clustering.winMain(hCurInst);
}
