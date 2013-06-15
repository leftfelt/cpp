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
#include <gui/TextClass.h>
#include <image_util/biImageClass.h>
#include <gui/ImageClass.h>
#include <CameraClass.h>
#include <gui/KeyClass.h>
#include <gui/MouseClass.h>
#include <gui/LineClass.h>
#include <gui/SquareClass.h>

#define SIZE 1
#define PI 3.1415926535897932384 
#define r 15
#define SEARCH 15
#define WIDTH 640/2
#define HEIGHT 480/2

void CornerDetector(biImage &image, std::vector<POINT> &corner);//コーナー検出器
void CornerDetector(biImage &image, std::vector<POINT> &corner,int pos = 5);
//特徴量を取得する
int getFeature(biImage &image, int x, int y,int size);
int getCornerFeature(biImage &image,int x,int y,int size, int pos);

#define __DEBUG

typedef struct eximage_tag{
	int num;
	POINT pt;
}FPOINT;

//オブジェクトの宣言
Image *img;	//表示用
Camera *camera; //カメラ
Window wnd("main");
Key key;//キーボード
Mouse mouse(wnd);//マウス
Text *text;

Square template_area;

Square square;
Square square_temp;

biImage temp;
biImage output;
//biImage temp_img;
biImage point;
biImage cap(WIDTH,HEIGHT); //カメラからのキャプチャ用

std::vector<FPOINT> list;//特徴点リスト
std::vector<biImage> temp_imgs;//テンプレート画像

//初期化
void WindowInit(){
	//インスタンスの作成
	img = new Image(WIDTH*2,HEIGHT);
	text = new Text();
	camera = new Camera;
	wnd.Size(WIDTH*2,HEIGHT);
	//インスタンスの設定
	printf("test");
	text->Pos(0,0);
	text->Set("Test");

	camera->SetCaptureSize(WIDTH,HEIGHT);
	camera->StartCapture(1);

	square.Color(Pixel(0,0,250));
	template_area.Color(Pixel(255,0,0));

	template_area.Apex(WIDTH/2,HEIGHT/2,r,r);

	//point.Load("point.bmp");

	//インスタンスの登録
	wnd.AddObject(img);
	wnd.AddObject(text);
	wnd.Title("画像テスト");
	AddWindow(&wnd);
}

void WindowUpdate(){
	int i,j,k,l,m;
	char str[256];
	
	FPOINT fpt;

	static char threshold = -1;
	static int pos = 0;
	static int num = 0;
	static int lflag=0;
	
	//キーの状態の取得===================================
	key.Update();

	cap = camera->Image();	//カメラから取り込み
	//cap.Contrast();
	//cap.Laplacian();

	temp = cap;
	output = cap;
	output = temp;

	fpt.pt = mouse.Pos();

	if(mouse.Click(M_LEFT)){
		if(lflag < 2)lflag++;
		if(lflag == 1){//クリックした瞬間
			//マウスの座標
			fpt.pt = mouse.Pos();
			biImage *t = new biImage();
			*t = cap.Cut(fpt.pt.x-r/2,fpt.pt.y-r/2,r,r);
			list.push_back(fpt);
			temp_imgs.push_back(*t);//テンプレートを登録
		}
	}else{
		lflag = 0;
	}

	double primTemp;
	double prim;

	//探索
	for(i = 0 ; i < (int)list.size() ; i++){
		POINT pt = {-1,-1};

		pt = cap.TemplateMatching(list.at(i).pt.x,list.at(i).pt.y,2*SEARCH,2*SEARCH,temp_imgs.at(i),90);
		if(pt.x != 0 && pt.y != 0){
			list.at(i).pt.x = pt.x;
			list.at(i).pt.y = pt.y;
		}else{//見つからなかった
			pt = square.getMiddle();
			square_temp.Move(pt.x,pt.y);
			list.at(i).pt = square_temp.getApex(i%4);//list更新
		}
		//temp_imgs.at(i) = temp_img;//テンプレート画像を更新

		cap.Put(list.at(i).pt.x,list.at(i).pt.y,Pixel(0,255,0));

		square.Apex(i%4,list.at(i).pt.x,list.at(i).pt.y);
		if(i%4==3){//
			pt = square.getMiddle();
			cap.Put(pt.x,pt.y,Pixel(255,0,0));

			primTemp = (4*PI*square_temp.getArea())/pow(square_temp.getPerimeter(),2.0);
			prim = (4*PI*square.getArea())/pow(square.getPerimeter(),2.0);
			//円形度をテンプレートと比較
			//*
			if(abs(primTemp*100-prim*100) > 10){
				for(j=0 ; j<(int)list.size() ; j++){
					list.at(j).pt = square_temp.getApex(j);
					square = square_temp;
				}
			}else{
				//square_temp = square;
			}//*/
		}
		cap.Put(list.at(i).pt.x,list.at(i).pt.y,Pixel(255,0,0));
	}

	if(list.size() == 4 && lflag == 1){
		square_temp = square;
	}
	square_temp.Move(square.getMiddle().x,square.getMiddle().y);
	template_area.Move(mouse.Pos().x,mouse.Pos().y);
	
#ifdef __DEBUG
	//テキストを変更
	sprintf(str,"閾値:%d PX:%d PY:%d A:%d C:%f",threshold,fpt.pt.x,fpt.pt.y,square.getArea(cap),(4*PI*square.getArea())/pow(square.getPerimeter(),2.0));
	text->Set(str);
#endif

}


void WindowDraw(){
	//*
	square.Draw(cap);
	square.Draw(output);
	square_temp.Draw(output);
	template_area.Draw(cap);
	//*/
	//画面へ反映
	
	img->Paste(WIDTH,0,output);
	img->Paste(0,0,cap);
	//img->Save("test.bmp");
}

void WindowDelete(){
	camera->StopCapture();
}

//====================================================================



class CornerDetector{
private:

public:
	void Check(biImage &image);
	
};

//コーナー検出器
void CornerDetector(biImage &image, std::vector<POINT> &corner){
	int i,j,k,l;
	int count;
	int ope;//注目点の画素
	POINT pt;
	//コーナー検出
	for(j = 0 ; j < image.Height() ; j++){
		for(i = 0 ; i < image.Width() ; i++){
			count = 0;
			ope = image.Get(i,j).Lightness();//画素が白だったら
			if(ope == 255){
				for(k = -SIZE ; k <= SIZE; k++){
					for(l = -SIZE ; l <= SIZE; l++){
						if( i+k < 0 || i+k >= image.Width() || j+l < 0 || j+l >= image.Height() )continue;
						if(ope != image.Get(i+k,j+l).Lightness() && k!=0 && l!=0) count++;
					}
				}
			}
	
			if(count >= 1){
				pt.x = i;
				pt.y = j;
				corner.push_back(pt);
			}
		}
	}//*/
}

//コーナー検出器
void CornerDetector(biImage &image, std::vector<POINT> &corner, int pos){
	int i,j;
	int count = 0;
	int width = 10;
	POINT pt;
	//コーナー検出
	for(j = 0 ; j < image.Height() ; j++){
		for(i = 0 ; i < image.Width() ; i++){
			count = 0;
			count = getFeature(image,i,j,SIZE);//特徴量を取得

			if( pos-width/2 < count && count < pos+width/2){
			//if(count >= 80){
				pt.x = i;
				pt.y = j;
				corner.push_back(pt);
			}
		}
	}//*/
}

//
/*
最大値:255*3*pow(2*size+1,2)
最小値:0
*/
//コーナー検出器
int getCornerFeature(biImage &image,int x,int y,int size, int pos){
	int i,j;
	int feature = 0;
	int count = 0;
	int width = 10;
	POINT pt;

	//コーナー検出
	if( x-size < 0 || x+size >= image.Width() || y-size < 0 || y+size >= image.Height() )return 0;
	for(j = y-size ; j <= y+size ; j++){
		for(i = x+size ; i <= x+size ; i++){
			count = 0;
			count = getFeature(image,i,j,SIZE);//特徴量を取得

			if( pos-width/2 < count && count < pos+width/2){
				feature += image.Get(i,j).Red() + image.Get(i,j).Green() + image.Get(i,j).Blue();
			}
		}
	}//*/
	return feature;
}

//特徴量を取得する
/*
最大値(画素反転)：255*pow(2*size+1,2)
最小値(完全一致)：0
*/
int getFeature(biImage &image, int x, int y,int size){
	int i,j,count=0;

	for(j = -size ; j <= size ; j++){
		for(i = -size ; i <= size ; i++){
			if( x+i < 0 || x+i >= image.Width() || y+j < 0 || y+j >= image.Height() )continue;
			count += abs(image.Get(x,y).Lightness() - image.Get(x+i,y+j).Lightness());
			//if(255 != GRAY(image.Get(x+i,y+j)) && i!=0 && j!=0) count++;
		}
	}

	return count;
}
