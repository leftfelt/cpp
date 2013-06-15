#ifdef _DEBUG
	#pragma comment(lib,"Flibd.lib")
	#pragma comment(lib,"Flib_wind.lib")
#else
	#pragma comment(lib,"Flib.lib")
	#pragma comment(lib,"Flib_win.lib")
#endif

#include <WindowClass.h>
#include <CameraClass.h>
#include <ImageClass.h>
#include <ImageUtilClass.h>
#include <StringClass.h>
#include <KeyClass.h>

#define WIDTH 320
#define HEIGHT 240
#define THNUM 4

class GETPOINT {
public:
	POINT pt;
	int id;
	unsigned long frame;
};

Window wnd("main");
Image *img;
biImage *background;
biImage *myhand;
biImage *hand;
Camera camera;
HANDLE thread[THNUM];			//�X���b�h�n���h��
GETPOINT *pt;
POINT temp_point;
Key key;
bool is_set_template = false;//�e���v���[�g���ݒ肳��Ă��邩
bool update_temp;

void setTemplate(biImage image, POINT pt);
void Range(biImage *image);
DWORD WINAPI updateTemplate(void* data);
DWORD WINAPI getPoint(void* data);

double rate = (HEIGHT / 450.0);
//int x=(135*rate),y=(175*rate),width=(180*rate),height=(250*rate);
int x=0,y=0,width=(HEIGHT),height=(HEIGHT);

void WindowInit(){

	wnd.Size(WIDTH,HEIGHT);
	camera.SetCaptureSize(WIDTH,HEIGHT);
	camera.StartCapture(1);

	background = new Image(WIDTH,HEIGHT);
	*background = Pixel(255);
	
	myhand = new biImage;
	myhand->Load("../../../data/hand.bmp");
	myhand->Clear(Pixel(0));
	myhand->Size(HEIGHT,HEIGHT);
	//myhand->Width(-myhand->Width());

	hand = new biImage(myhand->Width(),myhand->Height());
	*hand = Pixel(0);

	//�X���b�h�X�^�[�g
	pt = new GETPOINT[THNUM];
	temp_point.x = WIDTH/2;
	temp_point.y = HEIGHT/2;
	for(int i=0 ; i<THNUM ; i++){
		(pt+i)->id = i+1;
		(pt+i)->frame = 0;
		thread[i] = (HANDLE)CreateThread(NULL,0,getPoint,(pt+i),0,NULL);
	}
	(HANDLE)CreateThread(NULL,0,updateTemplate,&update_temp,0,NULL);

	img = new Image(WIDTH,HEIGHT);
	wnd.AddObject(img);
	AddWindow(&wnd);
}

void WindowUpdate(){
	int base = 110;
	int average = 0;
	int cnt = 0;

	//�J���������荞��
	*background = camera.Image();
	
	//��̃e���v���[�g��ݒ肷��
	background->Paste(WIDTH/2-HEIGHT/2,0,*myhand,50);

	for(int j = 0 ; j < HEIGHT ; j++){
		for(int i = 0 ; i < HEIGHT ; i++){
			Pixel pixel = background->Get(WIDTH/2-HEIGHT/2+i,j);
			if(pixel.Lightness() != 255){
				average += pixel.Lightness();
				cnt++;
			}
		}
	}
	
	*background = camera.Image();
	average /= cnt;
	*background += (base - average);
	//*hand += (base - average);
	
	for(int i=0 ; i<THNUM ; i++){
		(pt+i)->frame++;
	}
	key.Update();
	if(key.State(0x5A)){
		background->Paste(temp_point.x-hand->Width()/2,temp_point.y-hand->Height()/2,*hand);
	}
	if(key.State(0x58)){
		background->Paste(WIDTH/2-HEIGHT/2,0,*myhand,33);
	}
	//*/
}

void WindowDraw(){
	
	img->Paste(0,0,*background);
	img->Put(temp_point.x,temp_point.y,Pixel(255,0,0));
}

void WindowDelete(){
	camera.StopCapture();
	for(int i=0 ; i<THNUM ; i++){
		CloseHandle(thread[i]);
	}
}

//HSV���F���o
void Range(biImage *image){
	int hsv_h,hsv_s,hsv_v;
	bool H_is_in_range = false;
	bool S_is_in_range = true;
	bool V_is_in_range = true;

	int Hlow = 25 , Hhigh = 210;
	int Slow = 3 , Shigh = 55;
	int Vlow = 30 , Vhigh = 65;

	for(int j=0 ; j<image->Height() ; j++){
		for(int i=0 ; i<image->Width() ; i++){
			hsv_h = H(image->Get(i,j));
			hsv_s = S(image->Get(i,j));
			hsv_v = V(image->Get(i,j));
			if( ( (Hlow <= hsv_h && hsv_h <= Hhigh) == H_is_in_range) &&
				( (Slow <= hsv_s && hsv_s <= Shigh) == S_is_in_range) && 
				( (Vlow <= hsv_v && hsv_v <= Vhigh) == V_is_in_range) 
				){
				image->Put(i,j,Pixel(255));
			}else{
				image->Put(i,j,Pixel(0));
			}
		}
	}
}

DWORD WINAPI updateTemplate(void* data){
	biImage image;
	bool *update = (bool*)data;
	//�ŏ��̃e���v���[�g�Ɣ�r�����Ă�����x�߂���΍X�V����
	biImage hand_temp(hand->Width(),hand->Height());
	biImage temp;
	hand->Clear(Pixel(0));
	hand_temp.Clear(Pixel(0));
	temp.Clear(Pixel(0));
	while(1){
		if(key.DownState(VK_SPACE) || *update){
			POINT pt = {temp_point.x-WIDTH/2,temp_point.y-HEIGHT/2};
			image = *background;
			Range(&image);
			ImageUtil::AntiNoise(image,1);
			image = *background * (image/255);
			
			for(int j = y ; j < y+height ; j++){
				for(int i = x ; i < x+width ; i++){
					Pixel pixel = myhand->Get(i,j);
					if(pixel.Lightness() != 255){
						pixel = image.Get(WIDTH/2-HEIGHT/2+pt.x+i,pt.y+j);
						hand_temp.Put(i,j,pixel);
					}
				}
			}
			hand_temp.Clear(Pixel(0));
			if(temp.Matching(0,0,hand_temp) > 98){
				*hand = hand_temp;//�e���v���[�g�X�V
				hand->Clear(Pixel(0));
			}
			if(!is_set_template){
				temp = hand_temp;//�ŏ��Ƀe���v���[�g��ݒ肳�ꂽ
				temp.Clear(Pixel(0));
				*hand = temp;
			}
			is_set_template = true;
			*update = false;
		}
	}
	return 0;
}

DWORD WINAPI getPoint(void* data){
	GETPOINT *pt = (GETPOINT*)data;
	biImage image;

	while(1){
		if(pt->frame%pt->id != 0) continue;
	
		if(is_set_template && update_temp != true){
			//�e���v���[�g��ݒ肷��
			image = *background;
			POINT point = temp_point;
			Range(&image);
			ImageUtil::AntiNoise(image,1);
			image = *background * (image/255);
			temp_point = image.TemplateMatching(temp_point.x,temp_point.y,10,10,*hand,80);
			if(point.x != temp_point.x || point.y != temp_point.y){
				update_temp = true;
			}
		}
	}
	return 0;
}