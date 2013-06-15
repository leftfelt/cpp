#include "GraphClass.h"

Graph::Graph(){//�R���X�g���N�^
	InitGraph(&mgraph);
}
Graph::Graph(Graph &graph){//�R�s�[�R���X�g���N�^
	CopyGraph(&mgraph,&graph.mgraph,mfeature.Size());
}
Graph::~Graph(){
	DeleteGraph(&mgraph);
}


void Graph::SetFeature(Feature feature){
	mfeature = feature;
}
void Graph::AddPixel(int x, int y){//�_��ǉ�
	POINT pt = { x, y };
	AddPixelGraph(&mgraph,pt);
}
void Graph::Size(double rate){	//�T�C�Y��ύX
	ChangeRateGraph(&mgraph, rate, &mgraph);
}
void Graph::Move(int x, int y){	//�ړ�
	POINT pt = { x, y };
	PointMoveGraph(&mgraph,pt);
}

double Graph::operator*(biImage &image){//�����ʂ����o��
	int i;
	num = 0.0;
	for(i = 0 ; i < mgraph.count ; i++){
		mfeature.Pos((mgraph.pt_list+i)->x,(mgraph.pt_list+i)->y);
		num += mfeature * image;
	}
	return num;
}

void Graph::Clear(){//�O���t�̓��e���폜
	DeleteGraph(&mgraph);
}