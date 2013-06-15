#ifndef _AREA_INCLUDE
#define _AREA_INCLUDE

#include <math.h>

#define AreaList std::vector<Area>
/*
��`���͉~�`�̃G���A���쐬����
�G���A���m�̏Փ˔�����s��
*/
class Area{
private:
	int mx;			//X���W
	int my;			//Y���W
	int mheight;	//����
	int mwidth;		//��
public:
	Area();
	Area(int x, int y, int width, int height);		//�R���X�g���N�^
	void Pos(int x, int y);					//�ʒu��ݒ�
	void Size(int width, int height);		//�T�C�Y��ݒ�
	int X();							//X���W
	int Y();							//Y���W
	int Width();						//����Ԃ�
	int Height();						//������Ԃ�
	bool HitCheck(int x,int y);				//�����蔻��i�_�j
	bool HitCheck(Area area);				//�����蔻��i��`�j
};

#endif