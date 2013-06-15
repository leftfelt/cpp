#ifndef WAVELET_INCLUDE
#define WAVELET_INCLUDE

#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include "biImage.h"

#define PI 3.1415926535897932384 

//�K�{�[���E�F�[�u���b�g�̃p�����[�^
typedef struct gw_tag{
	long double rad;		//�p�x
	long double j;		//�X�P�[��
	long double a;		//�X�P�[��
	long double u;		//
	long double sigma;	//
}Gw_arg;

//���f��
typedef struct Complex_Number{
	long double Real;
	long double Imaginary;
}Comp_Num;

//�K�{�[���E�F�[�u���b�g
Comp_Num gw(long double x,long double y,Gw_arg arg);//�K�{�[��������
Comp_Num convolution(IMAGE *image, int x, int y, Gw_arg arg);//��ݍ��݁i�����ʒ��o�j

#endif