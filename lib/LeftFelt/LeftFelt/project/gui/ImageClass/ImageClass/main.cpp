#include <gui/WindowClass.h>
#include <gui/ImageClass.h>
#include <image_util/ImageUtilClass.h>
#include <gui/Application.h>

class ImageClassApp : public Application{
private:
	Window *wnd;
	biImage *load;
	biImage base;
	biImage *background;
	Image *img;
	int frame;
	int angle;
public:
	ImageClassApp(){
		frame = 0;
		angle = 0;
		this->createWindow("main");
		wnd->Size(640,480);

		load = new biImage();
		ImageUtil::Load(*load,"../../../data/color.bmp");
		base = *load;

		background = new biImage(640,480);
		*background = Pixel(255);

		img = new Image(640,480);
		img->Paste(0,0,*background);
		wnd->AddObject(img);
	}
	void main(){

		//���ׂẲ�f������������
		*background = Pixel(255);
	
		
		//ImageClass#setPos�̃e�X�g
		if(frame == 100){
			img->setPos(0,100);
		}
	
		//biImageClass#Size�̃e�X�g
		if(frame == 200){
			load->Height(-load->Height());
		}
		if(frame == 250){
			load->Width(-load->Width());
		}
		//biImageClass#Rotate�̃e�X�g
		if(frame >= 300){
			angle += 10;
			*load = base.Rotate(angle);
		}
		//biImageClass#Clear�̃e�X�g
		if(frame == 500){
			load->Clear(Pixel(255,0,0));
		}
	
		//biImageClass#Paste�̃e�X�g
		background->Paste(0,0,*load);
	
		frame++;

		img->Paste(0,0,*background);
	}
};
