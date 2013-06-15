#include "WaveletClass.h"
Wavelet::Wavelet(){
	arg.a = 2.5;
	arg.j = 1;
	arg.rad = 0;
	arg.sigma = 1.0;
	arg.u = PI;

	sampling = (w_size*2+1);//サンプリング数

	w_size = 15;		//窓サイズ(GW)
	wave = NULL;

	mtype = MONO;

	mx = 0;
	my = 0;

	Size(arg.j);
}

Wavelet::Wavelet(const Wavelet &wavelet){//コピーコンストラクタ
	if(wave != NULL)free(wave);
	*this = wavelet;
	Create();
}

void Wavelet::Create(){
	double i,j;
	int k,l;
	//大きさに合わせてウェーブレットを変える
	wave = (Comp_Num*)malloc(sizeof(Comp_Num)*sampling*sampling );
	
	for(l = 0,j = -size ; l<=w_size*2 ;l++,j+=arg.j){		//
		for(k = 0,i = -size ; k<=w_size*2 ;k++,i+=arg.j){		//
			*(wave+k+l*sampling) = gw(i,j,arg);
		}
	}
}

//位置を変更
void Wavelet::Pos(int x,int y){
	mx = x;
	my = y;
}

//変換する要素
void Wavelet::Type(ColorType type){
	mtype = type;
}

//角度を変更
void Wavelet::Angle(int angle){
	arg.rad = PI / 180 * angle;

	sampling = w_size*2+1;	//サンプリング数

	//角度に合わせてウェーブレットを変える
	if(wave != NULL)free(wave);
	Create();
}

//サイズを変更
void Wavelet::Size(double value){
	
	arg.j = value;
	size = w_size*arg.j;

	sampling = w_size*2+1;	//サンプリング数

	//大きさに合わせてウェーブレットを変える
	if(wave != NULL)free(wave);
	Create();
	
}

//特徴量を取得
double Wavelet::Get(){
	return num.Real;
}

//Waveletを削除
void Wavelet::Delete(){
	free(wave);
}

double Wavelet::operator*(biImage image){
	int i,j;
	Comp_Num temp;
	int count = 0;

	temp.Real = 0;
	temp.Imaginary = 0;
	num.Real = 0;
	num.Imaginary = 0;

	//畳み込み----------------------------------
	for(j = -w_size ; j <= w_size ; j+=1){		//
		for(i = -w_size ; i <= w_size ; i+=1){		//
			count++;
			if(mx+i<0 || my+j<0 || mx+i>=image.Width() || my+j>=image.Height())continue;
			
			temp = *(wave+((int)w_size+i)+((int)w_size+j)*sampling);
			//*
			switch(mtype){
				case MONO:
					num.Real += GRAY(image.Get(mx+i,my+j)) * temp.Real;
					num.Imaginary += GRAY(image.Get(mx+i,my+j)) * temp.Imaginary;
					break;
				case RGB_R:
					num.Real += R(image.Get(mx+i,my+j)) * temp.Real;
					num.Imaginary += R(image.Get(mx+i,my+j)) * temp.Imaginary;
					break;
				case RGB_G:
					num.Real += G(image.Get(mx+i,my+j)) * temp.Real;
					num.Imaginary += G(image.Get(mx+i,my+j)) * temp.Imaginary;
					break;
				case RGB_B:
					num.Real += B(image.Get(mx+i,my+j)) * temp.Real;
					num.Imaginary += B(image.Get(mx+i,my+j)) * temp.Imaginary;
					break;
				case YIQ_Y:
					num.Real += Y(image.Get(mx+i,my+j)) * temp.Real;
					num.Imaginary += Y(image.Get(mx+i,my+j)) * temp.Imaginary;
					break;
				case YIQ_I:
					num.Real += I(image.Get(mx+i,my+j)) * temp.Real;
					num.Imaginary += I(image.Get(mx+i,my+j)) * temp.Imaginary;
					break;
				case YIQ_Q:
					num.Real += Q(image.Get(mx+i,my+j)) * temp.Real;
					num.Imaginary += Q(image.Get(mx+i,my+j)) * temp.Imaginary;
					break;
				default:
					break;

			}//*/

		}
	}
	//------------------------------------------

	return num.Real;
}
