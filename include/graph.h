#ifndef _INCLUDE_GRAPH
#define _INCLUDE_GRAPH

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "biImage.h"
#include "lib.h"
#include "wavelet.h"

//���̍\����
typedef struct Edge_tag{
	int start;
	int end;
}Edge;

//�O���t�\����
typedef struct Graph_tag{
	int name_len;//������
	char name[256];	//�o�^�Җ�
	int count;//�_�̐�
	int line_count;//���̐�
	POINT *pt_list;//�_
	Edge *edge_list;//��
	long double *feature;//������
	RECT rc;//�͈�
}GRAPH;

//�O���t�Z�b�e�B���O�i�ǂݎ�����o�^�f�[�^�j
typedef struct Graph_setting_tag {
	long double j_start;
	long double j_end;
	long double j_plus;
	long double rad_start;
	long double rad_end;
	long double rad_plus;
}Graph_set;

//�O���t��ۑ��E�ǂݍ���
void LoadGraphFile(char *filename, GRAPH *graph, Graph_set setting, Gw_arg arg, IMAGE *img, char *name);
void SaveGraphFile(char *filename, GRAPH *graph, Graph_set setting, Gw_arg arg, IMAGE *img, char *name);

void InitGraph(GRAPH *graph);
void AddPixelGraph(GRAPH *graph, POINT pt);//�_��ǉ�
void AddLineGraph(GRAPH *graph, int start, int end);
void ChangeRateGraph(GRAPH *graph, long double rate,GRAPH *def_graph);//def_graph��rate�{�ɕύX����graph�ɃR�s�[
void CopyGraph(GRAPH *dstGraph,GRAPH *srcGraph,int feature_w);//dstGraph��srcGraph���R�s�[
void MoveGraph(GRAPH *graph,GRAPH *def_graph);//def_graph�̒��S��graph���ړ�
void PointMoveGraph(GRAPH *graph,POINT pt);//graph�̒��S��pt�Ɉړ�
void DeleteGraph(GRAPH *graph);//graph���폜
void GetGraphSize(GRAPH *graph);//graph�̃T�C�Y���擾

#endif