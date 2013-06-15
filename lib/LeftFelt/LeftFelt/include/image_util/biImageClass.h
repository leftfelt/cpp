#ifndef BIIMAGECLASS_INCLUDE
#define BIIMAGECLASS_INCLUDE

#include <base/MathClass.h>
#include <image_util/PixelClass.h>
#include <base/PointClass.h>
#include <base/StringClass.h>
#include <functional>

//biImageクラス
class biImage{
protected:
	Pixel *mimage;//画像
	bool flag;	//オペレータ用
private:
	Pixel clearColor; //透明色
	int width;
	int height;
	void init();
public:
	void static for_each(biImage &image, std::function<void(int x,int y)> func);
	void static around_each(biImage &image, Point point, int around, std::function<void(int x, int y)> func);
	biImage();
	biImage(int width, int height);
	biImage(const biImage &image);//コピーコンストラクタ
	~biImage();
	void Create(int width, int height);
	void Delete();
	bool Save(String filename);//保存
	bool Load(String filename);//読み込み
	void Paste(int x, int y,biImage &image, int mix_rate = 100);//貼り付け
	biImage Cut(int x, int y, int width, int height);//部分コピー
	biImage Rotate(int angle);//回転
	//=========================================================================
	Pixel Clear();//透明色を取得
	void Clear(Pixel coloer);//透明色を設定
	int Width();//幅を取得
	void Width(int width);//幅を設定
	int Height();//高さを取得
	void Height(int height);//高さを設定
	void Size(int width,int height);
	void Put(int x, int y, Pixel pixel);//ドットを描く
	Pixel Get(int x,int y);//ピクセルの色を取得(1バイト目:R 2バイト目:G 3バイト目:B 4バイト目:LABEL)
	//=========================================================================
	//=演算子(Copy)
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
	biImage operator+=(biImage &image);	//お互いの画素を足す
	biImage operator-=(biImage &image);	//お互いの画素を引く
	biImage operator*=(biImage &image);	//お互いの画素を掛ける
	biImage operator/=(biImage &image);	//お互いの画素を割る
	biImage operator+=(Pixel pixel);		//お互いの画素を足す
	biImage operator-=(Pixel pixel);		//お互いの画素を引く
	biImage operator*=(Pixel pixel);		//お互いの画素を掛ける
	biImage operator/=(Pixel pixel);		//お互いの画素を割る
};

//ドットを描く=========================================================================
inline void biImage::Put(int x, int y, Pixel pixel){
	x = ((x < 0)? this->Width()+x : x)%this->Width();
	y = ((y < 0)? this->Height()+y : y)%this->Height();
	*(mimage+x+y*this->Width()) = pixel;
}
//ドットを取得=========================================================================
inline Pixel biImage::Get(int x,int y){
	x = ((x < 0)? this->Width()+x : x)%this->Width();
	y = ((y < 0)? this->Height()+y : y)%this->Height();
	if( this->clearColor == *(mimage+x+y*this->Width()) ) return Pixel();
	return *(mimage+x+y*this->Width());
}

#endif