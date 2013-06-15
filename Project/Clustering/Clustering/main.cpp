#ifdef _DEBUG
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
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
#include "Testament.h"
#include "Converter.h"
#include "ClusterRepository.h"
#include "RelationDataRepository.h"
#include <base/MathClass.h>
#include <CameraClass.h>
#include <boost/thread.hpp>
#include <sstream>

#define WIDTH 320
#define HEIGHT 240

class Clustering : public Application{
private:
	ClassifierSetting setting;
	Image *background;
	biImage img;
	Window *window;
	Window *input_window;
	HWND input_data;
	Testament *testament;
	ClusterRepository *cluster_repository;
	RelationDataRepository *relation_data_repository;
	Camera camera;
	Mouse *mouse;
	Key key;
	int count;
	int range;
	int label;
public:
	Clustering(){

		ImageUtil::Load(img,"../../../data/lenna.bmp");

		img.Size(200,200);
		background = new Image(WIDTH,HEIGHT);
		
		//ウィンドウ生成
		window = this->createWindow();
		window->setSize(WIDTH,HEIGHT);
		window->AddObject(this->background);
		
		input_window = this->createWindow("input_window");
		input_window->setSize(200,50);
		input_window->hide();

		input_data = CreateWindow(
				"EDIT","",
				WS_CHILD|WS_VISIBLE|WS_BORDER,
				10,10,180,30,
				input_window->hWnd,
				(HMENU)1,
				this->getInstanceHandle(),
				NULL
				);

		//マウス
		mouse = new Mouse(*window);
		
		//分類器設定
		setting.cluster_size = 10;
		setting.dimension_num = 128;
		setting.learn_num = 10;
		setting.data_min = -2;
		setting.data_max = 2;
	
		//学習装置
		this->relation_data_repository = new RelationDataRepository;
		this->relation_data_repository->setup();
		this->cluster_repository = new ClusterRepository("image");
		testament = new Testament(setting,this->cluster_repository);

		//カメラ設定
		camera.SetCaptureSize(320,240);
		camera.StartCapture(1);

		count = 0;
		range = 50;
		
		//別スレッドで学習
		boost::thread learn([&](){
			while(1){
				try{
					testament->accept(this->camera.Image());
				}catch(std::exception e){
					//nice catch!
				}
				testament->learn(0.005);
				count++;
			}
		});

	}

	~Clustering(){
		camera.StopCapture();
	}

	void main(){
		if(window->isActive()){
			this->window_main();
		}else if(input_window->isActive()){
			this->input_window_main();
		}
	}

	void window_main(){
		std::ostringstream title;
		int label;
		std::vector<SurfFeature> surf_list;
		Square square;

		square.Color(Pixel(255,0,0));
		square.Apex(this->mouse->Pos().x-range/2, this->mouse->Pos().y-range/2,range,range);
				
		background->Paste(0,0,camera.Image());
		square.Draw(*background);
		
		//範囲内特徴のラベルを取ってくる
		surf_list = ImageUtil::getSurf(camera.Image(), 1);
		surf_list = this->getInAreaFeaturePoint(surf_list,this->range);

		if(mouse->Click(M_BUTTON::M_LEFT)){
			try{
				this->label = this->getLabelFromFeatureList(surf_list);
			}catch(std::exception e){
			}

			//ラベルからラベルの対応するデータを取ってくる
			try{
				RelationData data = this->relation_data_repository->search(this->label);
				MessageBox(NULL,data.getData().c_str(),data.getData().c_str(),MB_OK);
			}catch(std::exception e){
				//not found
				//見つからなかったら関連データを入力
				this->input_window->show();
				SetWindowText(this->input_data,"");
				title << this->label;
				this->input_window->setTitle(title.str());
				title.clear();
			}
		}
		
		//特徴点を表示
		this->displayFeaturePoint(surf_list);

		title << "label:[" << this->label << "]: count["<< this->count <<"]";
		this->window->setTitle(title.str());
	}

	void input_window_main(){
		this->key.Update();
		if(this->key.State(VK_RETURN)){
			char *data = new char(255);
			if(GetWindowTextLength(this->input_data) > 0){
				GetWindowText(this->input_data,data,GetWindowTextLength(this->input_data));
				std::string str(data);
				try{
					this->relation_data_repository->commit(RelationData(this->label,str));
				}catch(std::exception e){
					MessageBox(NULL,e.what(),e.what(),MB_OK);
				}
			}
			delete data;
			this->input_window->hide();
		}
	}

	//一番多いラベルを取得する
	unsigned int getLabelFromFeatureList(std::vector<SurfFeature> surf_list){
		Converter<biImage> converter;
		Converter<std::vector<float>> pattern_converter;
		std::map<std::string, int> count_map;
		
		std::for_each(surf_list.begin(), surf_list.end(), [&](SurfFeature surf){
			Row row;
			Pattern pattern = pattern_converter.convert(surf.features);
			try{
				row = this->cluster_repository->search(pattern,0.005);
				if(row.empty()) return;				count_map[row["label"]]++;
			}catch(std::exception e){
			}
		});

		if(count_map.empty()) throw std::exception("label not found");

		std::map<std::string,int>::iterator itr;
		std::map<std::string,int>::iterator itr_max;
		int max = 0;
		for( itr = count_map.begin() ; itr != count_map.end() ; itr++){
			if(itr->second > max) {
				max = itr->second;
				itr_max = itr;
			}
		}
		std::istringstream iss;
		iss.str(itr_max->first);
		iss >> max;

		return max;
	}

	//範囲内の特徴点を取ってくる
	std::vector<SurfFeature> getInAreaFeaturePoint(std::vector<SurfFeature> surf_list, int range){
		std::vector<SurfFeature> result;

		std::for_each(surf_list.begin(), surf_list.end(), [&](SurfFeature surf){
			if(this->mouse->Pos().x-range/2 <= surf.point.pt.x && surf.point.pt.x <= this->mouse->Pos().x+range/2){
				if(this->mouse->Pos().y-range/2 <= surf.point.pt.y && surf.point.pt.y <= this->mouse->Pos().y+range/2){
					result.push_back(surf);
				}
			}
		});

		return result;
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
