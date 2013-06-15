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
	
		//明るさ調整
		*test = *load;
		ImageUtil::Brightness(*test,150);
		background->Paste(0,0,*test);
	
		//コントラスト調整（モノクロ）
		*test = *load;
		ImageUtil::Contrast(*test);
		background->Paste(width,0,*test);
		
		//二値化テスト
		*test = *load;
		ImageUtil::Binarize(*test);
		background->Paste(width*2,0,*test);
	
		//ノイズ除去テスト
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		background->Paste(width*3,0,*test);
	
		//ラプラシアンフィルタ(TODO 用修正)
		*test = *load;
		ImageUtil::Laplacian(*test);
		//*test += Pixel(128);
		background->Paste(width*4,0,*test);
	
		//ゾーベルフィルタ
		*test = *load;
		ImageUtil::Sobel(*test);
		background->Paste(width*5,0,*test);
		
		//=====================================================
		//カラー画像
		background->Paste(0,height,*lenna);
	
		//YIQ範囲抽出
		*test = *lenna;
		ImageUtil::YIQRange(*test,20,80);
		background->Paste(width,height,*test);
		
		//HSV範囲抽出
		*test = *lenna;
		ImageUtil::HSVRange(*test,0,30);
		background->Paste(width*2,height,*test);
	
		//傾きをかける
		*test = *lenna;
		ImageUtil::Incline(*test,2,0.5,0.5);
		background->Paste(width*3,height,*test);
	
		//色領域分割
		*test = *lenna;
		ImageUtil::colorRegionSplit(*test);
		background->Paste(width*4,height,*test);
	
		//細線化
		*test = *cross;
		ImageUtil::Binarize(*test,100);
		ImageUtil::Sobel(*test);
		ImageUtil::Thinning(*test);
		background->Paste(width*5,height,*test);
		
		//=====================================================
		//ラベリング
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		label_num = ImageUtil::Labeling(*test);
		//ラベルごとに別の色を付ける
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
		
		//トップハット
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::TopHat(*test,2);
		background->Paste(width,height*2,*test);
	
		//ブラックハット
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::BlackHat(*test,2);
		background->Paste(width*2,height*2,*test);
	
		//鮮鋭化
		*test = *lenna;
		ImageUtil::UnSharpMasking(*test,1);
		background->Paste(width*3,height*2,*test);
	
		//ラベリング後画像の指定したラベルのオブジェクトだけ抽出
		*test = *load;
		ImageUtil::Binarize(*test);
		ImageUtil::AntiNoise(*test,5);
		label_num = ImageUtil::Labeling(*test);
		ImageUtil::SamplingObject(*test,0);
		background->Paste(width*4,height*2,*test);
	
		//ノード抽出
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
		
		//ラベリング後面積最大のオブジェクト取得
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
	
		//ぼやけ度
		String str;
		double num;
		*test = *bulr;
		num = ImageUtil::getBulr(*test);
		str = (double)num;
		background->Paste(width*2,height*3,*test);
		//MessageBox(NULL,str.c_str(),"message",MB_OK);
	
		//ぼやけ度2
		*test = *bulr2;
		num = ImageUtil::getBulr(*test);
		str = (double)num;
		background->Paste(width*3,height*3,*test);
		//MessageBox(NULL,str.c_str(),"message",MB_OK);
	
		//ぼやけ度3
		*test = *bulr2;
		ImageUtil::UnSharpMasking(*test,20);
		num = ImageUtil::getBulr(*test);
		str = (double)num;
		background->Paste(width*4,height*3,*test);
		//MessageBox(NULL,str.c_str(),"message",MB_OK);
	
		//ぼやけ度4
		*test = *load;
		Image::for_each(*bulr2,[&](int x, int y){
			test->Put(x,y, Pixel(
				ImageUtil::getBulr(load->Cut(x-1,y-1,3,3))*255
			));
		});
		background->Paste(width*5,height*3,*test);
		//=====================================================
		
		//SURF特徴量
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