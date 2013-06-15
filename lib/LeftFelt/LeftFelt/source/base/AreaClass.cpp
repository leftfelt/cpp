#include "base/AreaClass.h"

//�R���X�g���N�^
Area::Area(int x=0, int y=0, int width=0, int height=0){
	mx			= x;
	my			= y;
	mwidth		= width;
	mheight	= height;
}

Area::Area(){
	mx			= 0;
	my			= 0;
	mwidth		= 0;
	mheight	= 0;
}

//�ʒu��ݒ肷��
void Area::Pos(int x, int y){
	mx = x;
	my = y;
}

//�T�C�Y��ݒ肷��
void Area::Size(int width, int height){
	mwidth = width;
	mheight = height;
}

//X���W���擾
int Area::X(){
	return mx;
}

//Y���W���擾
int Area::Y(){
	return my;
}

//�����擾
int Area::Width(){
	return mwidth;
}

//Y���W���擾
int Area::Height(){
	return mheight;
}

//�����蔻��i�_�j
bool Area::HitCheck(int x,int y){
	if( mx <= x && x <= mx+mwidth && my <= y && y <= my+mheight){
		return true;
	}
	return false;
}
//�����蔻��i��`�j
bool Area::HitCheck(Area area){
	if( area.HitCheck(mx,my) ||
		area.HitCheck(my+mwidth,my) ||
		area.HitCheck(my,my+mheight) ||
		area.HitCheck(my+mwidth,my+mheight) 
		){
		return true;
	}
	return false;
}
