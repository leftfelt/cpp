#include "GraphClass.h"

Graph::Graph(){//コンストラクタ
	InitGraph(&mgraph);
}
Graph::Graph(Graph &graph){//コピーコンストラクタ
	CopyGraph(&mgraph,&graph.mgraph,mfeature.Size());
}
Graph::~Graph(){
	DeleteGraph(&mgraph);
}


void Graph::SetFeature(Feature feature){
	mfeature = feature;
}
void Graph::AddPixel(int x, int y){//点を追加
	POINT pt = { x, y };
	AddPixelGraph(&mgraph,pt);
}
void Graph::Size(double rate){	//サイズを変更
	ChangeRateGraph(&mgraph, rate, &mgraph);
}
void Graph::Move(int x, int y){	//移動
	POINT pt = { x, y };
	PointMoveGraph(&mgraph,pt);
}

double Graph::operator*(biImage &image){//特徴量を取り出す
	int i;
	num = 0.0;
	for(i = 0 ; i < mgraph.count ; i++){
		mfeature.Pos((mgraph.pt_list+i)->x,(mgraph.pt_list+i)->y);
		num += mfeature * image;
	}
	return num;
}

void Graph::Clear(){//グラフの内容を削除
	DeleteGraph(&mgraph);
}