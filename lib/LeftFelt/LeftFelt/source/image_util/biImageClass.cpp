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
//初期化
//コンストラクタ
biImage::biImage(){
	this->init();
}
//コンストラクタ
biImage::biImage(int width, int height){
	this->init();
	this->Create(width,height);
}
//コピーコンストラクタ
biImage::biImage(const biImage &image){
	if(this->flag == true)this->flag = false;
	mimage = image.mimage;
	this->clearColor = image.clearColor;
	this->width =image.width;
	this->height = image.height;
}
//デストラクタ
biImage::~biImage(){
	if(flag == true){//仮生成されていたら削除
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

//画像を保存
bool biImage::Save(String filename){
	throw new String("ImageUtil::Saveを使用してください");
	return true;
}
//画像を読み込み
bool biImage::Load(String filename){
	throw new String("ImageUtil::Loadを使用してください");
	return true;
}
//画像を貼り付け
void biImage::Paste(int x, int y,biImage &image, int mix_rate){
	biImage::for_each(image,[&](int i, int j){
		if(image.Get(i,j) == Pixel() || i%(int)(1.0/(mix_rate/100.0))) return;
		Pixel pixel = image.Get(i,j);
		this->Put((x+i),(y+j),pixel);
	});
}

//画像の一部を切り取り
biImage biImage::Cut(int x, int y, int width, int height){
	biImage ret(width,height);
	biImage::for_each(ret,[&](int i, int j){
		ret.Put(i,j,this->Get((x+i),(y+j)));
	});

	ret.flag = true;

	return ret;
}
//画像を消去
void biImage::Delete(){
	delete[] this->mimage;
	this->init();
}

//透明色を取得
Pixel biImage::Clear(){
	return this->clearColor;
}

//透明色を設定
void biImage::Clear(Pixel color){
	this->clearColor = color;
}

//幅を取得
int biImage::Width(){
	return this->width;
}
//高さを取得
int biImage::Height(){
	return this->height;
}

//幅を設定
void biImage::Width(int width){
	Size(width,this->Height());
}
//高さを設定
void biImage::Height(int height){
	Size(this->Width(),height);
}
//大きさを変更
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

biImage biImage::Rotate(int angle){//回転
	biImage image;
	int rx,ry;//回転後の座標
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

//演算子===========================================================================
biImage biImage::operator=(const biImage& image){
	//ここだけはconstがついているのでimageはWidth(),Height()が使えない
	if(this->Width() < image.width || this->Height() < image.height
		|| mimage == NULL){
		Delete();
		this->Create(image.width,image.height);
	}else{
		this->width = image.width;
		this->height = image.height;
	}

	this->clearColor = image.clearColor;

	//画素の内容をコピー
	biImage::for_each(*this,[&](int i, int j){
		*(mimage+i+j*this->Width()) = *(image.mimage+i+j*image.width);
	});

	return *this;
}

biImage biImage::operator=(Pixel pixel){
	
	//すべての画素をvalueにする
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

biImage biImage::operator+=(biImage &image){	//お互いの画素を足す
	int width,height;

	//サイズが小さいほうにあわせる
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) + *(image.mimage+i+j*image.Width());
	});


	return *this;
}


biImage biImage::operator-=(biImage &image){	//お互いの画素を引く
	int width,height;
	
	//サイズが小さいほうにあわせる
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) - *(image.mimage+i+j*image.Width());
	});
	return *this;
}

biImage biImage::operator*=(biImage &image){	//お互いの画素を掛ける
	int width,height;
	
	//サイズが小さいほうにあわせる
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) * *(image.mimage+i+j*image.Width());
	});
	return *this;
}


biImage biImage::operator/=(biImage &image){	//お互いの画素を割る
	int width,height;

	//サイズが小さいほうにあわせる
	if(this->Width() > image.Width()) width = image.Width();
	else width = this->Width();

	if(this->Height() > image.Height()) height = image.Height();
	else height = this->Height();

	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) / *(image.mimage+i+j*image.Width());
	});
	return *this;
}

biImage biImage::operator+=(Pixel pixel){	//お互いの画素をたす
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) + pixel;
	});
	return *this;
}

biImage biImage::operator-=(Pixel pixel){	//お互いの画素を引く
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) - pixel;
	});
	return *this;
}

biImage biImage::operator*=(Pixel pixel){	//お互いの画素をかける
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) * pixel;
	});
	return *this;
}

biImage biImage::operator/=(Pixel pixel){	//お互いの画素を割る
	biImage::for_each(*this,[&](int i, int j){
		*(this->mimage+i+j*this->Width()) = *(this->mimage+i+j*this->Width()) / pixel;
	});
	return *this;
}