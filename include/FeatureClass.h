#ifndef FEATURECLASS_INCLUDE
#define FEATURECLASS_INCLUDE

#include "biImageClass.h"
#include "WaveletClass.h"
#include <vector>
#include <windows.h>

using namespace std;

//さまざまな角度と大きさの特徴量を持つ
class Feature{
private:
	int mx;
	int my;
	double num;//特徴量
	vector<Wavelet> jet;
	unsigned int size;//特徴の数
public:
	Feature();
	void Pos(int x,int y);//座標を設定
	void AddFeature(double size, int angle, ColorType type = MONO);	//算出したいサイズと角度を追加
	double Get();//追加した角度と大きさの特徴量の平均を取得
	unsigned int Size();//特徴の数を取得
	double operator*(biImage image);
	void Delete();
};

#endif