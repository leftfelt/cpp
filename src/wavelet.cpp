#include "wavelet.h"

//*�K�{�[���E�F�[�u���b�g
Comp_Num gw(long double x,long double y,Gw_arg arg){
	Comp_Num num;
	long double pre,pre2,pre3;

	pre3 = ((2*arg.a * arg.j * arg.sigma)*(2 * arg.a * arg.j * arg.sigma));
	pre2 = (arg.u * (x*cos(arg.rad) + y*sin(arg.rad))) / (arg.a * arg.j);
	pre =  exp( - (x*x + y*y) / pre3) / (4*PI * arg.a * arg.a * arg.j * arg.j * arg.sigma);
	
	num.Real = pre * ( cos( pre2 ) - exp(-1.0*pow(arg.u*arg.sigma,2)) );//������
	num.Imaginary = pre * sin(pre2) ;//������
	return num;
}//*/
//

//��ݍ���
Comp_Num convolution(IMAGE *image, int x,int y,Gw_arg arg){
	Comp_Num num,sum;	//�E�F�[�u���b�g�̌v�Z����
	int pos = x+y*image->width;//��_
	int myPos; //���݂̈ʒu
	long double i,j;
	int Max_Size = image->width*image->height;
	long double w_size = (15 * arg.j);//���T�C�Y
	
	sum.Real = 0;
	sum.Imaginary = 0;
	//��ݍ��݌v�Z----------------------------------
	for(j = -w_size ; j <= w_size ; j+=arg.j){			//
		for(i = -w_size ; i <= w_size ; i+=arg.j){		//
			myPos = (int)(i+j*image->width);
			if(pos+myPos < 0 || pos+myPos > Max_Size)continue;
			num = gw(i,j,arg);
			//real-imaginary �v�Z
			//f(t) * ��(t)
			sum.Real	  += ((image->image_data + pos + myPos)->graylevel * num.Real);
			sum.Imaginary += ((image->image_data + pos + myPos)->graylevel * num.Imaginary);
		}
	}//*/-------------------------------------------

	return sum;
}