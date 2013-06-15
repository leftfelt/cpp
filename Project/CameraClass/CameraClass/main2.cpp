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

void CornerDetector(biImage &image, std::vector<POINT> &corner);//�R�[�i�[���o��
void CornerDetector(biImage &image, std::vector<POINT> &corner,int pos = 5);
//�����ʂ��擾����
int getFeature(biImage &image, int x, int y,int size);
int getCornerFeature(biImage &image,int x,int y,int size, int pos);

#define __DEBUG

typedef struct eximage_tag{
	int num;
	POINT pt;
}FPOINT;

//�I�u�W�F�N�g�̐錾
Image *img;	//�\���p
Camera *camera; //�J����
Window wnd("main");
Key key;//�L�[�{�[�h
Mouse mouse(wnd);//�}�E�X
Text *text;

Square template_area;

Square square;
Square square_temp;

biImage temp;
biImage output;
//biImage temp_img;
biImage point;
biImage cap(WIDTH,HEIGHT); //�J��������̃L���v�`���p

std::vector<FPOINT> list;//�����_���X�g
std::vector<biImage> temp_imgs;//�e���v���[�g�摜

//������
void WindowInit(){
	//�C���X�^���X�̍쐬
	img = new Image(WIDTH*2,HEIGHT);
	text = new Text();
	camera = new Camera;
	wnd.Size(WIDTH*2,HEIGHT);
	//�C���X�^���X�̐ݒ�
	printf("test");
	text->Pos(0,0);
	text->Set("Test");

	camera->SetCaptureSize(WIDTH,HEIGHT);
	camera->StartCapture(1);

	square.Color(Pixel(0,0,250));
	template_area.Color(Pixel(255,0,0));

	template_area.Apex(WIDTH/2,HEIGHT/2,r,r);

	//point.Load("point.bmp");

	//�C���X�^���X�̓o�^
	wnd.AddObject(img);
	wnd.AddObject(text);
	wnd.Title("�摜�e�X�g");
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
	
	//�L�[�̏�Ԃ̎擾===================================
	key.Update();

	cap = camera->Image();	//�J���������荞��
	//cap.Contrast();
	//cap.Laplacian();

	temp = cap;
	output = cap;
	output = temp;

	fpt.pt = mouse.Pos();

	if(mouse.Click(M_LEFT)){
		if(lflag < 2)lflag++;
		if(lflag == 1){//�N���b�N�����u��
			//�}�E�X�̍��W
			fpt.pt = mouse.Pos();
			biImage *t = new biImage();
			*t = cap.Cut(fpt.pt.x-r/2,fpt.pt.y-r/2,r,r);
			list.push_back(fpt);
			temp_imgs.push_back(*t);//�e���v���[�g��o�^
		}
	}else{
		lflag = 0;
	}

	double primTemp;
	double prim;

	//�T��
	for(i = 0 ; i < (int)list.size() ; i++){
		POINT pt = {-1,-1};

		pt = cap.TemplateMatching(list.at(i).pt.x,list.at(i).pt.y,2*SEARCH,2*SEARCH,temp_imgs.at(i),90);
		if(pt.x != 0 && pt.y != 0){
			list.at(i).pt.x = pt.x;
			list.at(i).pt.y = pt.y;
		}else{//������Ȃ�����
			pt = square.getMiddle();
			square_temp.Move(pt.x,pt.y);
			list.at(i).pt = square_temp.getApex(i%4);//list�X�V
		}
		//temp_imgs.at(i) = temp_img;//�e���v���[�g�摜���X�V

		cap.Put(list.at(i).pt.x,list.at(i).pt.y,Pixel(0,255,0));

		square.Apex(i%4,list.at(i).pt.x,list.at(i).pt.y);
		if(i%4==3){//
			pt = square.getMiddle();
			cap.Put(pt.x,pt.y,Pixel(255,0,0));

			primTemp = (4*PI*square_temp.getArea())/pow(square_temp.getPerimeter(),2.0);
			prim = (4*PI*square.getArea())/pow(square.getPerimeter(),2.0);
			//�~�`�x���e���v���[�g�Ɣ�r
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
	//�e�L�X�g��ύX
	sprintf(str,"臒l:%d PX:%d PY:%d A:%d C:%f",threshold,fpt.pt.x,fpt.pt.y,square.getArea(cap),(4*PI*square.getArea())/pow(square.getPerimeter(),2.0));
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
	//��ʂ֔��f
	
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

//�R�[�i�[���o��
void CornerDetector(biImage &image, std::vector<POINT> &corner){
	int i,j,k,l;
	int count;
	int ope;//���ړ_�̉�f
	POINT pt;
	//�R�[�i�[���o
	for(j = 0 ; j < image.Height() ; j++){
		for(i = 0 ; i < image.Width() ; i++){
			count = 0;
			ope = image.Get(i,j).Lightness();//��f������������
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

//�R�[�i�[���o��
void CornerDetector(biImage &image, std::vector<POINT> &corner, int pos){
	int i,j;
	int count = 0;
	int width = 10;
	POINT pt;
	//�R�[�i�[���o
	for(j = 0 ; j < image.Height() ; j++){
		for(i = 0 ; i < image.Width() ; i++){
			count = 0;
			count = getFeature(image,i,j,SIZE);//�����ʂ��擾

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
�ő�l:255*3*pow(2*size+1,2)
�ŏ��l:0
*/
//�R�[�i�[���o��
int getCornerFeature(biImage &image,int x,int y,int size, int pos){
	int i,j;
	int feature = 0;
	int count = 0;
	int width = 10;
	POINT pt;

	//�R�[�i�[���o
	if( x-size < 0 || x+size >= image.Width() || y-size < 0 || y+size >= image.Height() )return 0;
	for(j = y-size ; j <= y+size ; j++){
		for(i = x+size ; i <= x+size ; i++){
			count = 0;
			count = getFeature(image,i,j,SIZE);//�����ʂ��擾

			if( pos-width/2 < count && count < pos+width/2){
				feature += image.Get(i,j).Red() + image.Get(i,j).Green() + image.Get(i,j).Blue();
			}
		}
	}//*/
	return feature;
}

//�����ʂ��擾����
/*
�ő�l(��f���])�F255*pow(2*size+1,2)
�ŏ��l(���S��v)�F0
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
