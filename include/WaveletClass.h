#ifndef WAVELETCLASS_INCLUDE
#define WAVELETCLASS_INCLUDE

#include "biImageClass.h"
#include "wavelet.h"
#include <windows.h>

//基底関数のクラスを作る
//*waveletは共有

//変換したい要素
typedef enum{
	MONO,
	RGB_R,
	RGB_G,
	RGB_B,
	YIQ_Y,
	YIQ_I,
	YIQ_Q,
}ColorType;

//ウェーブレット変換ができる
class Wavelet{
private:
	int mx,my;
	Comp_Num num;		//畳み込み結果
	Gw_arg arg;			//引数
	int w_size;			//窓サイズ
	double size;
	Comp_Num *wave;	//
	int sampling;	//サンプリング数
	ColorType mtype;	//変換する要素
	void Create();
public:
	Wavelet();
	Wavelet(const Wavelet &wavelet);//コピーコンストラクタ
	void Size(double value);			//サイズを変更
	void Angle(int angle);				//角度を変更
	void Type(ColorType type);			//変換する要素
	void Pos(int x,int y);				//注目点の位置を設定
	double Get();						//特徴量を取得
	double operator*(biImage image);	//注目点の周囲を畳み込む
	void Delete();
};

#endif