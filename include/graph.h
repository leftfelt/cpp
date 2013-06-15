#ifndef _INCLUDE_GRAPH
#define _INCLUDE_GRAPH

#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "biImage.h"
#include "lib.h"
#include "wavelet.h"

//線の構造体
typedef struct Edge_tag{
	int start;
	int end;
}Edge;

//グラフ構造体
typedef struct Graph_tag{
	int name_len;//文字数
	char name[256];	//登録者名
	int count;//点の数
	int line_count;//線の数
	POINT *pt_list;//点
	Edge *edge_list;//線
	long double *feature;//特徴量
	RECT rc;//範囲
}GRAPH;

//グラフセッティング（読み取った登録データ）
typedef struct Graph_setting_tag {
	long double j_start;
	long double j_end;
	long double j_plus;
	long double rad_start;
	long double rad_end;
	long double rad_plus;
}Graph_set;

//グラフを保存・読み込み
void LoadGraphFile(char *filename, GRAPH *graph, Graph_set setting, Gw_arg arg, IMAGE *img, char *name);
void SaveGraphFile(char *filename, GRAPH *graph, Graph_set setting, Gw_arg arg, IMAGE *img, char *name);

void InitGraph(GRAPH *graph);
void AddPixelGraph(GRAPH *graph, POINT pt);//点を追加
void AddLineGraph(GRAPH *graph, int start, int end);
void ChangeRateGraph(GRAPH *graph, long double rate,GRAPH *def_graph);//def_graphをrate倍に変更してgraphにコピー
void CopyGraph(GRAPH *dstGraph,GRAPH *srcGraph,int feature_w);//dstGraphにsrcGraphをコピー
void MoveGraph(GRAPH *graph,GRAPH *def_graph);//def_graphの中心にgraphを移動
void PointMoveGraph(GRAPH *graph,POINT pt);//graphの中心をptに移動
void DeleteGraph(GRAPH *graph);//graphを削除
void GetGraphSize(GRAPH *graph);//graphのサイズを取得

#endif