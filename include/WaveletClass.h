#ifndef WAVELETCLASS_INCLUDE
#define WAVELETCLASS_INCLUDE

#include "biImageClass.h"
#include "wavelet.h"
#include <windows.h>

//���֐��̃N���X�����
//*wavelet�͋��L

//�ϊ��������v�f
typedef enum{
	MONO,
	RGB_R,
	RGB_G,
	RGB_B,
	YIQ_Y,
	YIQ_I,
	YIQ_Q,
}ColorType;

//�E�F�[�u���b�g�ϊ����ł���
class Wavelet{
private:
	int mx,my;
	Comp_Num num;		//��ݍ��݌���
	Gw_arg arg;			//����
	int w_size;			//���T�C�Y
	double size;
	Comp_Num *wave;	//
	int sampling;	//�T���v�����O��
	ColorType mtype;	//�ϊ�����v�f
	void Create();
public:
	Wavelet();
	Wavelet(const Wavelet &wavelet);//�R�s�[�R���X�g���N�^
	void Size(double value);			//�T�C�Y��ύX
	void Angle(int angle);				//�p�x��ύX
	void Type(ColorType type);			//�ϊ�����v�f
	void Pos(int x,int y);				//���ړ_�̈ʒu��ݒ�
	double Get();						//�����ʂ��擾
	double operator*(biImage image);	//���ړ_�̎��͂���ݍ���
	void Delete();
};

#endif