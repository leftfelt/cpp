#include "image_util/biImageClass.h"

void biImage::for_each(biImage &image, std::function<void(int x,int y)> func){
	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			func(i,j);
		}
	}
}

void biImage::around_each(biImage &image, Point point, int around, std::function<void(int x, int y)> func){
	for(int j = point.Y()-around ; j <= point.Y()+around ; j++){
		for(int i = point.X()-around ; i <= point.X()+around ; i++){
			func(i,j);
		}
	}
}
//------------------------------------------------------------------------
//������
//�R���X�g���N�^
biImage::biImage(){
	this->init();
}
//�R���X�g���N�^
biImage::biImage(int width, int height){
	this->init();
	this->Create(width,height);
}
//�R�s�[�R���X�g���N�^
biImage::biImage(const biImage &image){
	if(this->flag == true)this->flag = false;
	mimage = image.mimage;
	this->clearColor = image.clearColor;
	this->width =image.width;
	this->height = image.height;
}
//�f�X�g���N�^
biImage::~biImage(){
	if(flag == true){//����������Ă�����폜
		Delete();
	}
}

void biImage::init(){
	clearColor = Pixel();
	flag = false;

	height = 0;
	width = 0;

	mimage = NULL;
}

void biImage::Create(int width, int height){
	this->init();

	this->width = width;
	this->height = height;

	//--------------------------------------------------------------------------------------------
	mimage = new Pixel[this->width*this->height];
}

//�摜��ۑ�
bool biImage::Save(String filename){
	throw new String("ImageUtil::Save���g�p���Ă�������");
	return true;
}
//�摜��ǂݍ���
bool biImage::Load(String filename){
	throw new String("ImageUtil::Load���g�p���Ă�������");
	return true;
}
//�摜��\��t��
void biImage::Paste(int x, int y,biImage &image, int mix_rate){
	biImage::for_each(image,[&](int i, int j){
		if(image.Get(i,j) == Pixel() || i%(int)(1.0/(mix_rate/100.0))) return;
		Pixel pixel = image.Get(i,j);
		this->Put((x+i),(y+j),pixel);
	});
}

//�摜�̈ꕔ��؂���
biImage biImage::Cut(int x, int y, int width, int height){
	biImage ret(width,height);
	biImage::for_each(ret,[&](int i, int j){
		ret.Put(i,j,this->Get((x+i),(y+j)));
	});

	ret.flag = true;

	return ret;
}
//�摜������
void biImage::Delete(){
	delete[] this->mimage;
	this->init();
}

//�����F���擾
Pixel biImage::Clear(){
	return this->clearColor;
}

//�����F��ݒ�
void biImage::Clear(Pixel color){
	this->clearColor = color;
}

//�����擾
int biImage::Width(){
	return this->width;
}
//�������擾
int biImage::Height(){
	return this->height;
}

//����ݒ�
void biImage::Width(int width){
	Size(width,this->Height());
}
//������ݒ�
void biImage::Height(int height){
	Size(this->Width(),height);
}
//�傫����ύX
void biImage::Size(int width ,int height ){
	biImage image;

	double wRate = (double)this->Width() / width;
	double hRate = (double)this->Height() / height;

	width = abs(width);
	height = abs(height);

	image.Create(width,height);

	biImage::for_each(image,[&](int i,int j){
		image.Put(i,j,this->Get((int)(i*wRate),(int)(j*hRate)));
	});
	image.Clear(this->clearColor);
	*this = image;
	image.Delete();
}

biImage biImage::Rotate(int angle){//��]
	biImage image;
	int rx,ry;//��]��̍��W
	double rad = angle * (3.14159265358979323846 / 180.0);
	
	image.Create(Width(),Height());
	image = this->clearColor;
	image.Clear(this->clearColor);

	for(int j = -this->Height()/2 ; j <= this->Height()/2 ; j++){
		for(int i = -this->Width()/2 ; i <= this->Width()/2 ; i++){
			rx = (int)(this->Width()/2 + i * cos(rad) - j * sin(rad));
			ry = (int)(this->Height()/2 + i * sin(rad) + j * cos(rad));
			if(rx < 0 || this->Width() <= rx || ry < 0 || this->Height() <= ry)continue;
			image.Put(i+this->Width()/2,j+this->Height()/2,Get(rx,ry));
		}
	}
	return image;
}

//���Z�q===========================================================================
biImage biImage::operator=(const biImage& image){
	//����������const�����Ă���̂�image��Width(),Height()���g���Ȃ�
	if(this->Width() < image.width || this->Height() < image.height
		|| mimage == NULL){
		Delete();
		this->Create(image.width,image.height);
	}else{
		this->width = image.width;
		this->height = image.height;
	}

	this->clearColor = image.clearColor;

	//��f�̓��e���R�s�[
	biImage::for_each(*this,[&](int i, int j){
		*(mimage+i+j*this->Width()) = *(image.mimage+i+j*image.width);
	});

	return *this;
}

biImage biImage::operator=(Pixel pixel){
	
	//���ׂẲ�f��value�ɂ���
	biImage::for_each(*this,[&](int i, int j){
		this->Put(i,j,pixel);
	});
	
	return  *this;
}


biImage biImage::operator+(biImage &image){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp += image;
	
	return temp;
}

biImage biImage::operator-(biImage &image){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp -= image;
	
	return temp;
}
biImage biImage::operator*(biImage &image){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp *= image;
	
	return temp;
}
biImage biImage::operator/(biImage &image){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp /= image;
	
	return temp;
}

biImage biImage::operator+(Pixel pixel){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp += pixel;
	
	return temp;
}

biImage biImage::operator-(Pixel pixel){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp -= pixel;
	
	return temp;
}
biImage biImage::operator*(Pixel pixel){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp *= pixel;
	
	return temp;
}
biImage biImage::operator/(Pixel pixel){
	biImage temp;

	temp.flag = true;
	temp = *this;
	temp /= pixel;
	
	return temp;
}

biImage biImage::operator+=(biImage &image){	//���݂��̉�f�𑫂�
	int width,height;

	//�T�C�Y���������ق��ɂ��킹��
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) + *(image.mimage+i+j*image.Width());
	});


	return *this;
}


biImage biImage::operator-=(biImage &image){	//���݂��̉�f������
	int width,height;
	
	//�T�C�Y���������ق��ɂ��킹��
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) - *(image.mimage+i+j*image.Width());
	});
	return *this;
}

biImage biImage::operator*=(biImage &image){	//���݂��̉�f���|����
	int width,height;
	
	//�T�C�Y���������ق��ɂ��킹��
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) * *(image.mimage+i+j*image.Width());
	});
	return *this;
}


biImage biImage::operator/=(biImage &image){	//���݂��̉�f������
	int width,height;

	//�T�C�Y���������ق��ɂ��킹��
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) / *(image.mimage+i+j*image.Width());
	});
	return *this;
}

biImage biImage::operator+=(Pixel pixel){	//���݂��̉�f������
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) + pixel;
	});
	return *this;
}

biImage biImage::operator-=(Pixel pixel){	//���݂��̉�f������
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) - pixel;
	});
	return *this;
}

biImage biImage::operator*=(Pixel pixel){	//���݂��̉�f��������
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) * pixel;
	});
	return *this;
}

biImage biImage::operator/=(Pixel pixel){	//���݂��̉�f������
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) / pixel;
	});
	return *this;
}