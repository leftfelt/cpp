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
	mimage = image.mimage;
	this->clearColor = image.clearColor;
	this->width =image.width;
	this->height = image.height;
}
//�f�X�g���N�^
biImage::~biImage(){
}

void biImage::init(){
	clearColor = Pixel();

	height = 0;
	width = 0;

	mimage.clear();
	std::vector<Pixel>(mimage).swap(mimage);
}

void biImage::Create(int width, int height){
	this->init();

	this->width = width;
	this->height = height;

	//--------------------------------------------------------------------------------------------
	mimage.resize(this->width * this->height);
}

//�摜��ۑ�
bool biImage::Save(String filename){
	throw std::exception("ImageUtil::Save���g�p���Ă�������");
	return true;
}
//�摜��ǂݍ���
bool biImage::Load(String filename){
	throw std::exception("ImageUtil::Load���g�p���Ă�������");
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

	return ret;
}
//�摜������
void biImage::Delete(){
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
//�傫����ύX(�v���t�@�N�^)
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
biImage biImage::operator=(Pixel pixel){
	
	//���ׂẲ�f��value�ɂ���
	std::fill(this->mimage.begin(), this->mimage.end(), pixel);
	
	return  *this;
}


biImage biImage::operator+(biImage &image){
	biImage temp;

	temp = *this;
	temp += image;
	
	return temp;
}

biImage biImage::operator-(biImage &image){
	biImage temp;

	temp = *this;
	temp -= image;
	
	return temp;
}
biImage biImage::operator*(biImage &image){
	biImage temp;

	temp = *this;
	temp *= image;
	
	return temp;
}
biImage biImage::operator/(biImage &image){
	biImage temp;

	temp = *this;
	temp /= image;
	
	return temp;
}

biImage biImage::operator+(Pixel pixel){
	biImage temp;

	temp = *this;
	temp += pixel;
	
	return temp;
}

biImage biImage::operator-(Pixel pixel){
	biImage temp;

	temp = *this;
	temp -= pixel;
	
	return temp;
}
biImage biImage::operator*(Pixel pixel){
	biImage temp;

	temp = *this;
	temp *= pixel;
	
	return temp;
}
biImage biImage::operator/(Pixel pixel){
	biImage temp;

	temp = *this;
	temp /= pixel;
	
	return temp;
}

biImage biImage::operator+=(biImage &image){	//���݂��̉�f�𑫂�
	int width,height;

	//�T�C�Y���������ق��ɂ��킹��
	width = (this->Width() > image.Width()) ? image.Width() : this->Width();
	height = (this->Height() > image.Height()) ? image.Height() : this->Height();
	
	for(int j = 0 ; j < height ; j++){
		for(int i = 0 ; i < width ; i++){
			this->mimage.at(i+j*width) = this->mimage.at(i+j*width) + image.mimage.at(i+j*width);
		}
	}

	return *this;
}


biImage biImage::operator-=(biImage &image){	//���݂��̉�f������
	int width,height;

	//�T�C�Y���������ق��ɂ��킹��
	width = (this->Width() > image.Width()) ? image.Width() : this->Width();
	height = (this->Height() > image.Height()) ? image.Height() : this->Height();

	for(int j = 0 ; j < height ; j++){
		for(int i = 0 ; i < width ; i++){
			this->mimage.at(i+j*width) = this->mimage.at(i+j*width) - image.mimage.at(i+j*width);
		}
	}

	return *this;
}

biImage biImage::operator*=(biImage &image){	//���݂��̉�f���|����
	int width,height;

	//�T�C�Y���������ق��ɂ��킹��
	width = (this->Width() > image.Width()) ? image.Width() : this->Width();
	height = (this->Height() > image.Height()) ? image.Height() : this->Height();

	for(int j = 0 ; j < height ; j++){
		for(int i = 0 ; i < width ; i++){
			this->mimage.at(i+j*width) = this->mimage.at(i+j*width) * image.mimage.at(i+j*width);
		}
	}

	return *this;
}


biImage biImage::operator/=(biImage &image){	//���݂��̉�f������
	int width,height;

	//�T�C�Y���������ق��ɂ��킹��
	width = (this->Width() > image.Width()) ? image.Width() : this->Width();
	height = (this->Height() > image.Height()) ? image.Height() : this->Height();

	for(int j = 0 ; j < height ; j++){
		for(int i = 0 ; i < width ; i++){
			this->mimage.at(i+j*width) = this->mimage.at(i+j*width) / image.mimage.at(i+j*width);
		}
	}

	return *this;
}

biImage biImage::operator+=(Pixel pixel){	//���݂��̉�f������
	biImage::for_each(*this,[&](int i, int j){
		this->mimage.at(i+j*this->Width()) = this->mimage.at(i+j*this->Width()) + pixel;
	});
	return *this;
}

biImage biImage::operator-=(Pixel pixel){	//���݂��̉�f������
	biImage::for_each(*this,[&](int i, int j){
		this->mimage.at(i+j*this->Width()) = this->mimage.at(i+j*this->Width()) - pixel;
	});
	return *this;
}

biImage biImage::operator*=(Pixel pixel){	//���݂��̉�f��������
	biImage::for_each(*this,[&](int i, int j){
		this->mimage.at(i+j*this->Width()) = this->mimage.at(i+j*this->Width()) * pixel;
	});
	return *this;
}

biImage biImage::operator/=(Pixel pixel){	//���݂��̉�f������
	biImage::for_each(*this,[&](int i, int j){
		this->mimage.at(i+j*this->Width()) = this->mimage.at(i+j*this->Width()) / pixel;
	});
	return *this;
}