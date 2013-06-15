#ifndef GRAPHCLASS_INCLUDE
#define GRAPHCLASS_INCLUDE

#include "biImageClass.h"
#include "graph.h"
#include "FeatureClass.h"
#include <windows.h>

class Graph{
protected:
	GRAPH mgraph;
private:
	Feature mfeature;
	double num;
public:
	Graph();
	Graph(Graph &graph);//コピーコンストラクタ
	~Graph();//デストラクタ
	void SetFeature(Feature feature);//特徴を設定
	void AddPixel(int x, int y); //点を追加
	void Size(double rate);	//サイズを変更
	void Move(int x, int y);	//移動
	double Get(); //全体の平均を取る
	double operator*(biImage &image);//画像からグラフの特徴を取り出す。
	void Clear();
};

#endif