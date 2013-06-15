#ifndef BIIMAGECLASS_INCLUDE
#define BIIMAGECLASS_INCLUDE

#include <base/MathClass.h>
#include <image_util/PixelClass.h>
#include <base/PointClass.h>
#include <base/StringClass.h>
#include <functional>

//biImage�N���X
class biImage{
protected:
	Pixel *mimage;//�摜
	bool flag;	//�I�y���[�^�p
private:
	Pixel clearColor; //�����F
	int width;
	int height;
	void init();
public:
	void static for_each(biImage &image, std::function<void(int x,int y)> func);
	void static around_each(biImage &image, Point point, int around, std::function<void(int x, int y)> func);
	biImage();
	biImage(int width, int height);
	biImage(const biImage &image);//�R�s�[�R���X�g���N�^
	~biImage();
	void Create(int width, int height);
	void Delete();
	bool Save(String filename);//�ۑ�
	bool Load(String filename);//�ǂݍ���
	void Paste(int x, int y,biImage &image, int mix_rate = 100);//�\��t��
	biImage Cut(int x, int y, int width, int height);//�����R�s�[
	biImage Rotate(int angle);//��]
	//=========================================================================
	Pixel Clear();//�����F���擾
	void Clear(Pixel coloer);//�����F��ݒ�
	int Width();//�����擾
	void Width(int width);//����ݒ�
	int Height();//�������擾
	void Height(int height);//������ݒ�
	void Size(int width,int height);
	void Put(int x, int y, Pixel pixel);//�h�b�g��`��
	Pixel Get(int x,int y);//�s�N�Z���̐F���擾(1�o�C�g��:R 2�o�C�g��:G 3�o�C�g��:B 4�o�C�g��:LABEL)
	//=========================================================================
	//=���Z�q(Copy)
	biImage operator=(const biImage &image);
	biImage operator=(Pixel pixel);
	biImage operator+(biImage &image);
	biImage operator-(biImage &image);
	biImage operator*(biImage &image);
	biImage operator/(biImage &image);
	biImage operator+(Pixel pixel);
	biImage operator-(Pixel pixel);
	biImage operator*(Pixel pixel);
	biImage operator/(Pixel pixel);
	biImage operator+=(biImage &image);	//���݂��̉�f�𑫂�
	biImage operator-=(biImage &image);	//���݂��̉�f������
	biImage operator*=(biImage &image);	//���݂��̉�f���|����
	biImage operator/=(biImage &image);	//���݂��̉�f������
	biImage operator+=(Pixel pixel);		//���݂��̉�f�𑫂�
	biImage operator-=(Pixel pixel);		//���݂��̉�f������
	biImage operator*=(Pixel pixel);		//���݂��̉�f���|����
	biImage operator/=(Pixel pixel);		//���݂��̉�f������
};

//�h�b�g��`��=========================================================================
inline void biImage::Put(int x, int y, Pixel pixel){
	x = ((x < 0)? this->Width()+x : x)%this->Width();
	y = ((y < 0)? this->Height()+y : y)%this->Height();
	*(mimage+x+y*this->Width()) = pixel;
}
//�h�b�g���擾=========================================================================
inline Pixel biImage::Get(int x,int y){
	x = ((x < 0)? this->Width()+x : x)%this->Width();
	y = ((y < 0)? this->Height()+y : y)%this->Height();
	if( this->clearColor == *(mimage+x+y*this->Width()) ) return Pixel();
	return *(mimage+x+y*this->Width());
}

#endif