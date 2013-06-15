#include "wavelet.h"

//*ガボールウェーブレット
Comp_Num gw(long double x,long double y,Gw_arg arg){
	Comp_Num num;
	long double pre,pre2,pre3;

	pre3 = ((2*arg.a * arg.j * arg.sigma)*(2 * arg.a * arg.j * arg.sigma));
	pre2 = (arg.u * (x*cos(arg.rad) + y*sin(arg.rad))) / (arg.a * arg.j);
	pre =  exp( - (x*x + y*y) / pre3) / (4*PI * arg.a * arg.a * arg.j * arg.j * arg.sigma);
	
	num.Real = pre * ( cos( pre2 ) - exp(-1.0*pow(arg.u*arg.sigma,2)) );//実数部
	num.Imaginary = pre * sin(pre2) ;//虚数部
	return num;
}//*/
//

//畳み込み
Comp_Num convolution(IMAGE *image, int x,int y,Gw_arg arg){
	Comp_Num num,sum;	//ウェーブレットの計算結果
	int pos = x+y*image->width;//基準点
	int myPos; //現在の位置
	long double i,j;
	int Max_Size = image->width*image->height;
	long double w_size = (15 * arg.j);//窓サイズ
	
	sum.Real = 0;
	sum.Imaginary = 0;
	//畳み込み計算----------------------------------
	for(j = -w_size ; j <= w_size ; j+=arg.j){			//
		for(i = -w_size ; i <= w_size ; i+=arg.j){		//
			myPos = (int)(i+j*image->width);
			if(pos+myPos < 0 || pos+myPos > Max_Size)continue;
			num = gw(i,j,arg);
			//real-imaginary 計算
			//f(t) * Ψ(t)
			sum.Real	  += ((image->image_data + pos + myPos)->graylevel * num.Real);
			sum.Imaginary += ((image->image_data + pos + myPos)->graylevel * num.Imaginary);
		}
	}//*/-------------------------------------------

	return sum;
}