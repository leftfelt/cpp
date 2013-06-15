#ifndef WAVELET_INCLUDE
#define WAVELET_INCLUDE

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "biImage.h"

#define PI 3.1415926535897932384 

//ガボールウェーブレットのパラメータ
typedef struct gw_tag{
	long double rad;		//角度
	long double j;		//スケール
	long double a;		//スケール
	long double u;		//
	long double sigma;	//
}Gw_arg;

//複素数
typedef struct Complex_Number{
	long double Real;
	long double Imaginary;
}Comp_Num;

//ガボールウェーブレット
Comp_Num gw(long double x,long double y,Gw_arg arg);//ガボール特徴量
Comp_Num convolution(IMAGE *image, int x, int y, Gw_arg arg);//畳み込み（特徴量抽出）

#endif