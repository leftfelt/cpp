
//#include "CameraClass.h"
#include "biImageClass.h"

#define SIZE 1
#define YIQ_SIZE 256
#define YIQ_Y 128

int main(int argc, char* argv[])
{
	//ラベリング＋ゾーベルでオブジェクト検出
	RECT rc,maxrc;
	biImage test,all,img,yiq(YIQ_SIZE,YIQ_SIZE);
	int num,i,j,k,l,area,max;
	int count,ope;
	unsigned char r,g,b;
	/*
	test.Load("sample.bmp");
	img = test;
	//test.Incline(2.0,0.5,0.5);
	//test.NaturalColor(10,60);
	//test.Save("nature.bmp");
	test.Threshold(220);
	test.AntiNoise(5);
	//test.Sobel();
	test.Save("Sobel.bmp");
	num = test.Labeling();

	all = test;
	all = all * 0;

	max = 0;
	
	//肌色が多いオブジェクトだけ抽出
	for(i = 1 ; i < num ; i++){
		test.Object(i,&rc);
		//*
		printf("%d %d %d %d\n",rc.left,rc.top,rc.right,rc.bottom);

		area = 0;
		for(k =  rc.top  ; k < rc.bottom; k++){
			for(j = rc.left ; j < rc.right ; j++){
				if(GRAY(test.Get(j,k)) == 255){//白い画素
					if(10 < I(img.Get(j,k)) && I(img.Get(j,k)) < 60){
						area++;//肌色面積
					}
				}
			}
		}

		//面積の大きい
		if(max < area){
			max = area;
			maxrc = rc;
			all = test;
		}
	}

	rc = maxrc;
	//*/

	//*
	img.Load("sample.bmp");
	all = img;//*/

	//コーナー検出
	for(j = 0 ; j < img.Height() ; j++){
		for(i = 0 ; i < img.Width() ; i++){
			count = 0;
			ope = GRAY(all.Get(i,j));
			if(ope == 255){
				for(k = -SIZE ; k <= SIZE; k++){
					for(l = -SIZE ; l <= SIZE; l++){
						if( i+k < 0 || i+k >= img.Width() || j+l < 0 || j+l >= img.Height() )continue;
						if(ope != GRAY(all.Get(i+k,j+l)) && i!=0 && j!=0) count++;
					}
				}
			}

			if(count >= 5){
				img.Put(i,j,Pixel(255,0,0));
			}
		}
	}
	/*
	//yiq表色系
	for(j = 0 ; j < YIQ_SIZE ; j++){
		for(i = 0 ; i < YIQ_SIZE ; i++){
			r = YIQ_Y + 1.0/2*0.956 * (i-YIQ_SIZE/2) + 1.0/2*0.623 * (j-YIQ_SIZE/2);
			g = YIQ_Y - 1.0/2*0.272 * (i-YIQ_SIZE/2) - 1.0/2*0.648 * (j-YIQ_SIZE/2);
			b = YIQ_Y - 1.0/2*1.105 * (i-YIQ_SIZE/2) + 1.0/2*0.705 * (j-YIQ_SIZE/2);
			yiq.Put(i,j,Pixel( r,g,b));
		}
	}
	yiq.Save("yiq1.bmp");
	//yiq表色系
	for(j = 0 ; j < YIQ_SIZE ; j++){
		for(i = 0 ; i < YIQ_SIZE ; i++){
			r = YIQ_Y + 0.956 * (i-YIQ_SIZE/2) + 0.621 * (j-YIQ_SIZE/2);
			g = YIQ_Y - 0.272 * (i-YIQ_SIZE/2) - 0.647 * (j-YIQ_SIZE/2);
			b = YIQ_Y - 1.105 * (i-YIQ_SIZE/2) + 0.702 * (j-YIQ_SIZE/2);
			yiq.Put(i,j,Pixel( r,g,b));
		}
	}
	yiq.Save("yiq2.bmp");
	//yiq表色系
	for(j = 0 ; j < YIQ_SIZE ; j++){
		for(i = 0 ; i < YIQ_SIZE ; i++){
			r = YIQ_Y + 0.9489 * (i-YIQ_SIZE/2) + 0.6561 * (j-YIQ_SIZE/2);
			g = YIQ_Y - 0.2645 * (i-YIQ_SIZE/2) - 0.6847 * (j-YIQ_SIZE/2);
			b = YIQ_Y - 1.1127 * (i-YIQ_SIZE/2) + 1.8050 * (j-YIQ_SIZE/2);
			yiq.Put(i,j,Pixel( r,g,b));
		}
	}
	yiq.Save("yiq3.bmp");
	//yiq表色系
	for(j = 0 ; j < YIQ_SIZE ; j++){
		for(i = 0 ; i < YIQ_SIZE ; i++){
			r = YIQ_Y + 0.956 * (i-YIQ_SIZE/2) + 0.621 * (j-YIQ_SIZE/2);
			g = YIQ_Y - 0.272 * (i-YIQ_SIZE/2) - 0.647 * (j-YIQ_SIZE/2);
			b = YIQ_Y - 1.106 * (i-YIQ_SIZE/2) - 1.703 * (j-YIQ_SIZE/2);
			yiq.Put(i,j,Pixel( r,g,b));
		}
	}
	yiq.Save("yiq4.bmp");*/
	//---------------------------------
	/*
	//枠で囲う
	for(j = rc.left ; j <= rc.right ; j++){
		img.Put(j,rc.top,Pixel(0,255,0));
		img.Put(j,rc.bottom,Pixel(0,255,0));
	}
	for(j = rc.top ; j <= rc.bottom; j++){
		img.Put(rc.left,j,Pixel(0,255,0));
		img.Put(rc.right,j,Pixel(0,255,0));
	}//*/
	//----------------------------------

	all.Save("allSobel.bmp");
	img.Save("allframe.bmp");

	/*
	Camera camera;
	biImage test;
	int num,i;
	char str[256];

	camera.SetCaptureSize(1280,1024);

	camera.StartCapture(0);

	while (1) {
	     //キー入力待ち（Escキーで終了）
	     
	     if (GetAsyncKeyState(VK_SPACE)){//スペースで画像を取得
			
			 //test = camera.Image();
			 //test.Sobel();
			 test.Load("test1.bmp");
			 test.Threshold(160);
			 test.AntiNoise(10);
			 test.Save("testbe.bmp");
			 num = test.Labeling();
			 for(i = 0 ; i < num ; i++){
				 test.Object(i);
				 sprintf(str,"test%02d.bmp",i);
				 test.Save(str);
			 }
		 }

		 if (GetAsyncKeyState(VK_ESCAPE)) {
			 break;//エスケープで終了
		 }
	}
	

	camera.StopCapture();*/

	return 0;
}





