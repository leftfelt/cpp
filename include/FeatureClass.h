#ifndef FEATURECLASS_INCLUDE
#define FEATURECLASS_INCLUDE

#include "biImageClass.h"
#include "WaveletClass.h"
#include <vector>
#include <windows.h>

using namespace std;

//���܂��܂Ȋp�x�Ƒ傫���̓����ʂ�����
class Feature{
private:
	int mx;
	int my;
	double num;//������
	vector<Wavelet> jet;
	unsigned int size;//�����̐�
public:
	Feature();
	void Pos(int x,int y);//���W��ݒ�
	void AddFeature(double size, int angle, ColorType type = MONO);	//�Z�o�������T�C�Y�Ɗp�x��ǉ�
	double Get();//�ǉ������p�x�Ƒ傫���̓����ʂ̕��ς��擾
	unsigned int Size();//�����̐����擾
	double operator*(biImage image);
	void Delete();
};

#endif