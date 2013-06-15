
#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
#endif

#include <gui/WindowClass.h>
#include <gui/ImageClass.h>
#include <image_util/ImageUtilClass.h>
#include <gui/KeyClass.h>
#include <gui/MouseClass.h>
#include <gui/SquareClass.h>
#include <gui/TextClass.h>
#include <gui/Application.h>
#include <ThinkGearClass.h>

#define WIDTH 500
#define HEIGHT 300

//”]”gƒQ[ƒW
class BrainWaveGage{
private:
	double threshold;
	double value;
	int gage_max;
	Area gage;
	Pixel color;
public:
	BrainWaveGage(int width,int height){
		threshold = 0.000;
		gage_max = 10;
		gage.Size(width,(int)(height/gage_max));
		value = 0;
	}
	//F
	void setColor(Pixel color){
		this->color = color;
	}
	//ƒQ[ƒW‚ÌˆÊ’u
	void setPos(int x, int y){
		gage.Pos(x,y);
	}
	void setThreshold(double threshold){
		this->threshold = threshold;
	}
	//’l‚ðÝ’è‚·‚é
	void setValue(double value){
		if(this->threshold < fabs(value)){
			this->threshold = fabs(value);
		}
		this->value = value / this->threshold * 100;
	}
	//ƒQ[ƒW‚ð•\Ž¦
	void Draw(biImage &image){
		int num = this->value / this->gage_max;
		Square square;
		square.Color(this->color);
		for(int i = 0 ; i < abs(num) ; i++){
			if(0 < num){
				square.Apex(
					gage.X(),
					gage.Y()-(i+1)-i*this->gage.Height(),
					gage.Width(),
					gage.Height()
				);
			}else if(num < 0){
				square.Apex(
					gage.X(),
					gage.Y()+(i+1)+(i+1)*this->gage.Height(),
					gage.Width(),
					gage.Height()
				);
			}
			square.Draw(image);
		}
	}
};

//”]”gƒ‚ƒjƒ^
class BrainMonitor{
private:
	Area area;
	int gage_width;
	int gage_height;
	int gage_num;
	int param_num;
	double threshold;
	std::vector<BrainWaveGage> wave_gage;
	std::vector<Text*> wave_text;
public:
	BrainMonitor(Window &wnd,int x, int y){
		gage_width = 15; //ƒQ[ƒWˆêŒÂ•ª‚Ì•
		gage_height = 100; //ƒQ[ƒWˆêŒÂ•ª‚Ì‚‚³
		param_num = 14; //ƒpƒ‰ƒ[ƒ^‚Ì”
		this->setPos(x,y);
		this->area.Size(
			param_num*this->gage_width+1+200,
			15*param_num
		);

		for(int i = 0 ; i < this->param_num ; i++){
			Text *text = new Text;
			text->Size(10,15);
			text->Pos(area.X()+gage_width*param_num+10,area.Y()+15*i);
			text->Color(0,0,0);
			text->Font("MS–¾’©");
			text->AreaSize(200,14);
			wave_text.push_back(text);
			wnd.AddObject(wave_text.at(i));
		}
	}

	void setPos(int x, int y){
		area.Pos(x,y);
	}

	void Update(ThinkGear *thinkgear){
		BrainWaves wave = thinkgear->getWaves();
		std::map<String,double> waves = wave.toMap();
		std::vector<String> parametors = wave.getParameters();
		this->wave_gage.clear();
		std::for_each(waves.begin(),waves.end(),[&](std::pair<String,double> itr){
			BrainWaveGage gage(gage_width,gage_height);
			gage.setPos(area.X()+wave_gage.size()*gage_width,area.Y()+gage_height);
			gage.setThreshold(10000);
			gage.setColor(Pixel(255,0,0));
			gage.setValue(itr.second);
			
			wave_text.at(wave_gage.size())->Set(String(itr.first + " : ") + itr.second);
			wave_gage.push_back(gage);
		});
	}
	void Draw(biImage &image){
		Square square;
		square.Color(Pixel(0,0,255));
		square.Apex(area.X(),area.Y(),area.Width(),area.Height());
		square.Draw(image);
		for(int i = 0 ; i < wave_gage.size() ; i++){
			wave_gage.at(i).Draw(image);
		}
	}
};

class BrainMonitorApp : public Application{
private:
	Window *wnd;
	Image *display;
	biImage *background;
	ThinkGear thinkgear;
	BrainMonitor *monitor;
public:
	BrainMonitorApp(){
		wnd = this->createWindow("main");
		wnd->Size(WIDTH,HEIGHT);

		display = new Image(WIDTH,HEIGHT);
		wnd->AddObject(display);

		background = new biImage(WIDTH,HEIGHT);
		
		try{
			thinkgear.Connect("COM4");
		}catch(char* message){
			MessageBox(NULL,message,"error",MB_OK);
		}

		monitor = new BrainMonitor(*wnd,0,50);

		thinkgear.StartCapture();
	}

	~BrainMonitorApp(){
		thinkgear.StopCapture();
		thinkgear.DisConnect();
	}

	void main(){
		
		monitor->Update(&thinkgear);
		*background = Pixel(0);
		monitor->Draw(*background);
		display->Paste(0,0,*background);
	}
};