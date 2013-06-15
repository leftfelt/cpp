#include "FeatureClass.h"
//コンストラクタ
Feature::Feature(){
	size = 0;
	mx = 0;
	my = 0;
	num = 0.0;
}

void Feature::Pos(int x,int y){//座標を設定
	int i;

	mx = x;
	my = y;

	for(i = 0 ; i < (int)jet.size() ; i++){
		jet.at(i).Pos(x,y);
	}
}

void Feature::AddFeature(double size, int angle, ColorType type){	//算出したいサイズと角度を追加
	Wavelet wavelet;
	wavelet.Size(size);
	wavelet.Angle(angle);
	wavelet.Type(type);
	wavelet.Pos(mx,my);

	jet.push_back(wavelet);
	size++;
}


double Feature::Get(){//追加した角度と大きさの特徴量の平均を取得
	return num;
}

unsigned int Feature::Size(){//特徴の数を取得
	return size;
}


double Feature::operator*(biImage image){//特徴量を計算
	int i,cnt = 0;

	num = 0.0;

	for(i = 0 ; i < (int)jet.size() ; i++){
		num += jet.at(i) * image;
		cnt++;
	}

	return (num /= cnt);
}
//削除
void Feature::Delete(){
	int i;

	for(i = 0 ; i < (int)jet.size() ; i++){
		jet.at(i).Delete();//Waveletを削除
	}

	jet.clear();	//内容を削除
}