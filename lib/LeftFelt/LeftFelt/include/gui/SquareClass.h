#ifndef SQUARECLASS_INCLUDE
#define SQUARECLASS_INCLUDE


#include <windows.h>
#include <gui/LineClass.h>

//�l�p�`
class Square{
private:
	Line line[4];
public:
	void Apex(POINT pt0,POINT pt1,POINT pt2,POINT pt3);//4�̒��_��ݒ�
	void Apex(int x, int y, int width, int height);
	void Apex(char num, int x, int y);//�w�肵�����_��ݒ�
	void Move(int x, int y);//���S���W���ړ�
	POINT getApex(char num);//0�`3�Ŏw�肵�����_�̍��W���擾
	POINT getMiddle();//���_�̍��W���擾
	int getArea();//�ʐς��擾�i���m�Ȓl�ł͂Ȃ��ߎ��l�E���m�ȏꍇ������j
	int getArea(biImage &image);//�ʐς��擾�i���m�Ȓl�ł͂Ȃ��ߎ��l�E���m�ȏꍇ������j
	int getPerimeter();//���͒����擾
	void Color(Pixel color);//�F���w��
	void Draw(biImage &image);
};

#endif