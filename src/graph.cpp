
#include "graph.h"

//==============================================================================================
//グラフに点を追加
void AddPixelGraph(GRAPH *graph, POINT pt){
	graph->count++;
	POINT *pt_list = (POINT *)malloc(sizeof(POINT)*graph->count);
	if(graph->pt_list != NULL)*pt_list = *graph->pt_list;
	*(pt_list + graph->count-1) = pt;
	free(graph->pt_list);
	graph->pt_list = pt_list;
}
//グラフに線を追加
void AddLineGraph(GRAPH *graph, int start, int end){
	graph->line_count++;
	Edge *edge_list = (Edge*)malloc(sizeof(Edge)*graph->line_count);
	*edge_list = *graph->edge_list;
	(edge_list + graph->line_count-1)->start = start;
	(edge_list + graph->line_count-1)->end = end;
	free(graph->pt_list);
	graph->edge_list = edge_list;
}
//グラフをファイルに書き込み
void SaveGraphFile(char *filename, GRAPH *graph, Graph_set setting, Gw_arg arg, IMAGE *img, char *name){
//void SaveGraphFile(Comm_Data *data, char *filename,Graph *graph){
	FILE *fp;
	int i,k,l;
	int name_len;

	Comp_Num num;

	int rad_w = (int)((setting.rad_end-setting.rad_start)/setting.rad_plus);
	int j_w = (int)((setting.j_end-setting.j_start)/setting.j_plus);
	int feature_w = j_w * rad_w;


	fp = fopen(filename,"wb");
	if(fp == NULL){
		MessageBox(NULL,"画像が見つかりません","error",MB_OK);
		exit(1);
	}

	name_len = strlen(name);

	fwrite(&name_len,sizeof(int),1,fp);//名前の文字数
	fwrite(name,sizeof(char),name_len,fp);//名前
	fwrite(&graph->count,sizeof(int),1,fp);		//点の数を書込
	fwrite(&graph->line_count,sizeof(int),1,fp);	//線の数を書込

	//点を書込
	fwrite(graph->pt_list,sizeof(POINT), graph->count,fp);

	//線を書込
	fwrite(graph->edge_list,sizeof(Edge), graph->line_count,fp);

	//*
	for(i = 0; i < graph->count ; i++){
		//特徴量を追加
		for(arg.j = setting.j_start ,k = 0 ; k < j_w ; k++ , arg.j += setting.j_plus){//スケールを変更
			for(arg.rad = setting.rad_start,l = 0 ; l < rad_w ; l++, arg.rad += setting.rad_plus){//角度を変更
				num = convolution(img,(graph->pt_list+i)->x,(graph->pt_list+i)->y,arg);
				*(graph->feature+i*feature_w+k*rad_w+l) = num.Real;
			}
		}
	}//*/

	//特徴量を書込
	for(i = 0 ; i < graph->count ; i++){
		fwrite(graph->feature+i*feature_w,sizeof(long double),feature_w,fp);
	}
	
	fclose(fp);

}
//グラフをファイルから読み取り
void LoadGraphFile(char *filename, GRAPH *graph, Graph_set setting, Gw_arg arg, IMAGE *img, char *name){
//void LoadGraphFile(Comm_Data *data, char *filename,Graph *graph){
	FILE *fp;

	int rad_w=(int)((setting.rad_end-setting.rad_start)/setting.rad_plus);
	int feature_w=(int)((setting.j_end-setting.j_start)/setting.j_plus*rad_w);

	fp = fopen(filename,"rb");
	if(fp == NULL){
		MessageBox(NULL,"画像が見つかりません","error",MB_OK);
		exit(1);
	}

	//初期化
	DeleteGraph(graph);
	InitGraph(graph);
	
	//*
	fread(&graph->name_len,sizeof(int),1,fp);//名前の文字数
	fread(name,sizeof(char),graph->name_len,fp);//名前
	graph->name[graph->name_len] = 0;//*/
	fread(&graph->count,sizeof(int),1,fp);		//点の数を読込

	if(graph->pt_list == NULL)graph->pt_list = (POINT *)malloc(sizeof(POINT) * graph->count);
	if(graph->edge_list == NULL)graph->edge_list = (Edge *)malloc(sizeof(Edge) * graph->count * graph->count);
	if(graph->feature == NULL)graph->feature = (long double *)malloc(sizeof(long double) * graph->count * feature_w);

	fread(&graph->line_count,sizeof(int),1,fp);	//線の数を読込

	//点を読込
	fread(graph->pt_list,sizeof(POINT),graph->count,fp);

	//線を読込
	fread(graph->edge_list,sizeof(Edge),graph->line_count,fp);
	
	
	//特徴量を読込
	fread(graph->feature,sizeof(long double),graph->count * feature_w,fp);

	fclose(fp);
}
//グラフを初期化
void InitGraph(GRAPH *graph){
	graph->line_count = 0;
	graph->count = 0;
	graph->name_len = 0;
	graph->pt_list = NULL;
	graph->edge_list = NULL;
	graph->feature = NULL;
}
//グラフのサイズを変更
void ChangeRateGraph(GRAPH *graph, long double rate,GRAPH *def_graph){
	int i;
	for(i = 0 ; i < graph->count; i++){
		(graph->pt_list+i)->x = (LONG)((def_graph->pt_list+i)->x*rate);
		(graph->pt_list+i)->y = (LONG)((def_graph->pt_list+i)->y*rate);
	}
}
//グラフを移動
void MoveGraph(GRAPH *graph,GRAPH *def_graph){
	int i;
	POINT move;

	move.x = (def_graph->pt_list+15)->x - (graph->pt_list+15)->x;
	move.y = (def_graph->pt_list+15)->y - (graph->pt_list+15)->y;

	for(i = 0 ; i < graph->count; i++){
		(graph->pt_list+i)->x = (graph->pt_list+i)->x+move.x;
		(graph->pt_list+i)->y = (graph->pt_list+i)->y+move.y;
	}
}
//グラフの中心を指定した点へ移動
void PointMoveGraph(GRAPH *graph,POINT pt){
	int i;
	POINT move;

	move.x = pt.x - (graph->pt_list)->x;
	move.y = pt.y - (graph->pt_list)->y;

	for(i = 0 ; i < graph->count; i++){
		(graph->pt_list+i)->x = (graph->pt_list+i)->x+move.x;
		(graph->pt_list+i)->y = (graph->pt_list+i)->y+move.y;
	}
}
//グラフをコピー
void CopyGraph(GRAPH *dstGraph,GRAPH *srcGraph,int feature_w){

	int i;
	
	//初期化
	DeleteGraph(dstGraph);
	InitGraph(dstGraph);

	*dstGraph = *srcGraph;

	//名前をコピー
	strcpy(dstGraph->name,srcGraph->name);

	dstGraph->pt_list = (POINT *)malloc(sizeof(POINT) *srcGraph->count);
	dstGraph->edge_list = (Edge *)malloc(sizeof(Edge) * srcGraph->count * srcGraph->count);
	dstGraph->feature = (long double *)malloc(sizeof(long double) * srcGraph->count * feature_w);

	//点をコピー
	for(i = 0 ; i < srcGraph->count; i++){
		*(dstGraph->pt_list+i) = *(srcGraph->pt_list+i);
		*(dstGraph->feature+i) = *(srcGraph->feature+i);
	}
	//線をコピー
	for(i = 0 ; i< srcGraph->line_count;i++){
		*(dstGraph->edge_list+i) = *(srcGraph->edge_list+i);
	}

}
//グラフを解放
void DeleteGraph(GRAPH *graph){
	free(graph->pt_list);
	free(graph->edge_list);
	free(graph->feature);
	InitGraph(graph);
}
//グラフのサイズを取得
void GetGraphSize(GRAPH *graph){
	int i;
	RECT rc;

	rc.top = -1;
	rc.right = -1;
	rc.left = -1;
	rc.bottom = -1;

	for(i = 0 ; i < graph->count ; i++){
		if((graph->pt_list+i)->x > rc.right || rc.right == -1){
			rc.right = (graph->pt_list+i)->x;
		}
		if((graph->pt_list+i)->x < rc.left || rc.left == -1){
			rc.left = (graph->pt_list+i)->x;
		}

		if((graph->pt_list+i)->y > rc.bottom || rc.bottom == -1){
			rc.bottom = (graph->pt_list+i)->y;
		}
		if((graph->pt_list+i)->y < rc.top || rc.top == -1){
			rc.top = (graph->pt_list+i)->y;
		}
	}

	graph->rc = rc;
	
}
