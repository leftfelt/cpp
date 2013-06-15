#ifndef LINECLASS_INCLUDE
#define LINECLASS_INCLUDE


#include <windows.h>
#include <math.h>
#include <image_util/biImageClass.h>

//����
class Line{
private:
	POINT pt[2];//0:�n�_ 1:�I�_
	Pixel mcolor; //�F
public:
	Line();
	void Color(Pixel color);	//�F���w��
	void Start(int x , int y); //�n�_��ݒ�
	void End(int x, int y);	   //�I�_��ݒ�
	void Move(int x, int y);
	POINT getMiddle();//���_���擾
	POINT getStart();		//�n�_���擾
	POINT getEnd();		//�I�_���擾
	int getDistance();//��_�Ԃ̋������擾
	void Draw(biImage &image);
	bool isClossing(Line line); //��������
};

#endif