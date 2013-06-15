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
	Graph(Graph &graph);//�R�s�[�R���X�g���N�^
	~Graph();//�f�X�g���N�^
	void SetFeature(Feature feature);//������ݒ�
	void AddPixel(int x, int y); //�_��ǉ�
	void Size(double rate);	//�T�C�Y��ύX
	void Move(int x, int y);	//�ړ�
	double Get(); //�S�̂̕��ς����
	double operator*(biImage &image);//�摜����O���t�̓��������o���B
	void Clear();
};

#endif