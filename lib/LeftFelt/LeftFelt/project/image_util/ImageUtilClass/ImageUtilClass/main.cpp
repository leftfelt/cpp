#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"guid.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"gui.lib")
#endif

#include <gui/WindowClass.h>
#include <gui/ImageClass.h>
#include <gui/Application.h>
#include <image_util/ImageUtilClass.h>

class ImageUtilApp : public Application{
private:
	Window *wnd;
	Image *img;
	biImage *background;
	biImage *cross;
	biImage *load;
	biImage *lenna;
	biImage *test;
	biImage *bulr;
	biImage *bulr2;
public:
	ImageUtilApp(){
		wnd = this->createWindow("main");
		load = new biImage;
		ImageUtil::Load(*load,"../../../data/matumae1.bmp");
		load->Size(load->Width()/3,load->Height()/3);
	
		cross = new biImage;
		ImageUtil::Load(*cross,"../../../data/cross.bmp");
		cross->Size(load->Width(),load->Height());
	
		lenna = new biImage;
		ImageUtil::Load(*lenna,"../../../data/lenna.bmp");
		lenna->Size(load->Width(),load->Height());
		
		bulr = new biImage;
		ImageUtil::Load(*bulr,"../../../data/bulr1.bmp");
		bulr->Size(load->Width(),load->Height());
		
		bulr2 = new biImage;
		ImageUtil::Load(*bulr2,"../../../data/bulr3.bmp");
		bulr2->Size(load->Width(),load->Height());
	
		img = new Image(load->Width()*6,load->Height()*5);
		wnd->Size(load->Width()*6,load->Height()*5);
		background = new biImage(load->Width()*6,load->Height()*5);
		*background = Pixel(255);
	
		test = new biImage;
		*test = *load;
	
		wnd->AddObject(img);
	}

	void main(){
		int label_num;
		Pixel pixel;

		int width = test->Width();
		int height = test->Height();
	
		*background = Pixel(255);
		*test = *load;
	
		//���邳����
		*test = *load;
		ImageUtil::Brightness(*test,150);
		background->Paste(0,0,*test);
	
		//�R���g���X�g�����i���m�N���j
		*test = *load;
		ImageUtil::Contrast(*test);
		background->Paste(width,0,*test);
		
		//��l���e�X�g
		*test = *load;
		ImageUtil::Binarize(*test);
		background->Paste(width*2,0,*test);
	
		//�m�C�Y�����e�X�g
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		background->Paste(width*3,0,*test);
	
		//���v���V�A���t�B���^(TODO �p�C��)
		*test = *load;
		ImageUtil::Laplacian(*test);
		//*test += Pixel(128);
		background->Paste(width*4,0,*test);
	
		//�]�[�x���t�B���^
		*test = *load;
		ImageUtil::Sobel(*test);
		background->Paste(width*5,0,*test);
		
		//=====================================================
		//�J���[�摜
		background->Paste(0,height,*lenna);
	
		//YIQ�͈͒��o
		*test = *lenna;
		ImageUtil::YIQRange(*test,20,80);
		background->Paste(width,height,*test);
		
		//HSV�͈͒��o
		*test = *lenna;
		ImageUtil::HSVRange(*test,0,30);
		background->Paste(width*2,height,*test);
	
		//�X����������
		*test = *lenna;
		ImageUtil::Incline(*test,2,0.5,0.5);
		background->Paste(width*3,height,*test);
	
		//�F�̈敪��
		*test = *lenna;
		ImageUtil::colorRegionSplit(*test);
		background->Paste(width*4,height,*test);
	
		//�א���
		*test = *cross;
		ImageUtil::Binarize(*test,100);
		ImageUtil::Sobel(*test);
		ImageUtil::Thinning(*test);
		background->Paste(width*5,height,*test);
		
		//=====================================================
		//���x�����O
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		label_num = ImageUtil::Labeling(*test);
		//���x�����Ƃɕʂ̐F��t����
		for(int count=0 ; count<label_num ; count++){
			for(int j=0 ; j<test->Height() ; j++){
				for(int i=0 ; i<test->Width() ; i++){
					pixel = test->Get(i,j);
					if(pixel.Label() == count){
						pixel.Blue(count*10);
						test->Put(i,j,pixel);
					}
				}
			}
		}
		background->Paste(0,height*2,*test);
		
		//�g�b�v�n�b�g
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::TopHat(*test,2);
		background->Paste(width,height*2,*test);
	
		//�u���b�N�n�b�g
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::BlackHat(*test,2);
		background->Paste(width*2,height*2,*test);
	
		//�N�s��
		*test = *lenna;
		ImageUtil::UnSharpMasking(*test,1);
		background->Paste(width*3,height*2,*test);
	
		//���x�����O��摜�̎w�肵�����x���̃I�u�W�F�N�g�������o
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		label_num = ImageUtil::Labeling(*test);
		ImageUtil::SamplingObject(*test,0);
		background->Paste(width*4,height*2,*test);
	
		//�m�[�h���o
		PointList point_list;
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		ImageUtil::Thinning(*test);
		point_list = ImageUtil::getNodeList(*test,1);
		for(int i = 0 ; i < (signed)point_list.size() ; i++){
			test->Put(point_list.at(i).X(),point_list.at(i).Y(),Pixel(255,0,0));
		}
		background->Paste(width*5,height*2,*test);
	
		//=====================================================
		
		//���x�����O��ʐύő�̃I�u�W�F�N�g�擾
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		ImageUtil::Labeling(*test);
		int area_id = ImageUtil::SamplingLargeObject(*test);
		background->Paste(0,height*3,*test);
		
		//for_each
		*test = *load;
		biImage::for_each(*test,[&](int x,int y){
			test->Put(x,y,Pixel(128));
		});
		background->Paste(width,height*3,*test);
	
		//�ڂ₯�x
		String str;
		double num;
		*test = *bulr;
		num = ImageUtil::getBulr(*test);
		str = (double)num;
		background->Paste(width*2,height*3,*test);
		//MessageBox(NULL,str.c_str(),"message",MB_OK);
	
		//�ڂ₯�x2
		*test = *bulr2;
		num = ImageUtil::getBulr(*test);
		str = (double)num;
		background->Paste(width*3,height*3,*test);
		//MessageBox(NULL,str.c_str(),"message",MB_OK);
	
		//�ڂ₯�x3
		*test = *bulr2;
		ImageUtil::UnSharpMasking(*test,20);
		num = ImageUtil::getBulr(*test);
		str = (double)num;
		background->Paste(width*4,height*3,*test);
		//MessageBox(NULL,str.c_str(),"message",MB_OK);
	
		//�ڂ₯�x4
		*test = *load;
		Image::for_each(*bulr2,[&](int x, int y){
			test->Put(x,y, Pixel(
				ImageUtil::getBulr(load->Cut(x-1,y-1,3,3))*255
			));
		});
		background->Paste(width*5,height*3,*test);
		//=====================================================
		
		//SURF������
		*test = *lenna;
		std::vector<SurfFeature> surf_list;
		surf_list = ImageUtil::getSurf(*test,100);
		std::for_each(surf_list.begin(), surf_list.end(),[&](SurfFeature surf){
			for(int j = -2 ; j <= 2 ; j++){
				for(int i = -2 ; i <= 2 ; i++){
					test->Put(surf.point.pt.x+i,surf.point.pt.y+j, Pixel(0,0,255));
				}
			}
		});
		background->Paste(0,height*4,*test);

		img->Paste(0,0,*background);
	}
};

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst, LPSTR lpsCmdLine, int nCmdShow){
	ImageUtilApp app;
	app.winMain(hCurInst);
}