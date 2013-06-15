#include "FeatureClass.h"
//�R���X�g���N�^
Feature::Feature(){
	size = 0;
	mx = 0;
	my = 0;
	num = 0.0;
}

void Feature::Pos(int x,int y){//���W��ݒ�
	int i;

	mx = x;
	my = y;

	for(i = 0 ; i < (int)jet.size() ; i++){
		jet.at(i).Pos(x,y);
	}
}

void Feature::AddFeature(double size, int angle, ColorType type){	//�Z�o�������T�C�Y�Ɗp�x��ǉ�
	Wavelet wavelet;
	wavelet.Size(size);
	wavelet.Angle(angle);
	wavelet.Type(type);
	wavelet.Pos(mx,my);

	jet.push_back(wavelet);
	size++;
}


double Feature::Get(){//�ǉ������p�x�Ƒ傫���̓����ʂ̕��ς��擾
	return num;
}

unsigned int Feature::Size(){//�����̐����擾
	return size;
}


double Feature::operator*(biImage image){//�����ʂ��v�Z
	int i,cnt = 0;

	num = 0.0;

	for(i = 0 ; i < (int)jet.size() ; i++){
		num += jet.at(i) * image;
		cnt++;
	}

	return (num /= cnt);
}
//�폜
void Feature::Delete(){
	int i;

	for(i = 0 ; i < (int)jet.size() ; i++){
		jet.at(i).Delete();//Wavelet���폜
	}

	jet.clear();	//���e���폜
}