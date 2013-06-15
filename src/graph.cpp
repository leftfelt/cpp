
#include "graph.h"

//==============================================================================================
//�O���t�ɓ_��ǉ�
void AddPixelGraph(GRAPH *graph, POINT pt){
	graph->count++;
	POINT *pt_list = (POINT *)malloc(sizeof(POINT)*graph->count);
	if(graph->pt_list != NULL)*pt_list = *graph->pt_list;
	*(pt_list + graph->count-1) = pt;
	free(graph->pt_list);
	graph->pt_list = pt_list;
}
//�O���t�ɐ���ǉ�
void AddLineGraph(GRAPH *graph, int start, int end){
	graph->line_count++;
	Edge *edge_list = (Edge*)malloc(sizeof(Edge)*graph->line_count);
	*edge_list = *graph->edge_list;
	(edge_list + graph->line_count-1)->start = start;
	(edge_list + graph->line_count-1)->end = end;
	free(graph->pt_list);
	graph->edge_list = edge_list;
}
//�O���t���t�@�C���ɏ�������
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
		MessageBox(NULL,"�摜��������܂���","error",MB_OK);
		exit(1);
	}

	name_len = strlen(name);

	fwrite(&name_len,sizeof(int),1,fp);//���O�̕�����
	fwrite(name,sizeof(char),name_len,fp);//���O
	fwrite(&graph->count,sizeof(int),1,fp);		//�_�̐�������
	fwrite(&graph->line_count,sizeof(int),1,fp);	//���̐�������

	//�_������
	fwrite(graph->pt_list,sizeof(POINT), graph->count,fp);

	//��������
	fwrite(graph->edge_list,sizeof(Edge), graph->line_count,fp);

	//*
	for(i = 0; i < graph->count ; i++){
		//�����ʂ�ǉ�
		for(arg.j = setting.j_start ,k = 0 ; k < j_w ; k++ , arg.j += setting.j_plus){//�X�P�[����ύX
			for(arg.rad = setting.rad_start,l = 0 ; l < rad_w ; l++, arg.rad += setting.rad_plus){//�p�x��ύX
				num = convolution(img,(graph->pt_list+i)->x,(graph->pt_list+i)->y,arg);
				*(graph->feature+i*feature_w+k*rad_w+l) = num.Real;
			}
		}
	}//*/

	//�����ʂ�����
	for(i = 0 ; i < graph->count ; i++){
		fwrite(graph->feature+i*feature_w,sizeof(long double),feature_w,fp);
	}
	
	fclose(fp);

}
//�O���t���t�@�C������ǂݎ��
void LoadGraphFile(char *filename, GRAPH *graph, Graph_set setting, Gw_arg arg, IMAGE *img, char *name){
//void LoadGraphFile(Comm_Data *data, char *filename,Graph *graph){
	FILE *fp;

	int rad_w=(int)((setting.rad_end-setting.rad_start)/setting.rad_plus);
	int feature_w=(int)((setting.j_end-setting.j_start)/setting.j_plus*rad_w);

	fp = fopen(filename,"rb");
	if(fp == NULL){
		MessageBox(NULL,"�摜��������܂���","error",MB_OK);
		exit(1);
	}

	//������
	DeleteGraph(graph);
	InitGraph(graph);
	
	//*
	fread(&graph->name_len,sizeof(int),1,fp);//���O�̕�����
	fread(name,sizeof(char),graph->name_len,fp);//���O
	graph->name[graph->name_len] = 0;//*/
	fread(&graph->count,sizeof(int),1,fp);		//�_�̐���Ǎ�

	if(graph->pt_list == NULL)graph->pt_list = (POINT *)malloc(sizeof(POINT) * graph->count);
	if(graph->edge_list == NULL)graph->edge_list = (Edge *)malloc(sizeof(Edge) * graph->count * graph->count);
	if(graph->feature == NULL)graph->feature = (long double *)malloc(sizeof(long double) * graph->count * feature_w);

	fread(&graph->line_count,sizeof(int),1,fp);	//���̐���Ǎ�

	//�_��Ǎ�
	fread(graph->pt_list,sizeof(POINT),graph->count,fp);

	//����Ǎ�
	fread(graph->edge_list,sizeof(Edge),graph->line_count,fp);
	
	
	//�����ʂ�Ǎ�
	fread(graph->feature,sizeof(long double),graph->count * feature_w,fp);

	fclose(fp);
}
//�O���t��������
void InitGraph(GRAPH *graph){
	graph->line_count = 0;
	graph->count = 0;
	graph->name_len = 0;
	graph->pt_list = NULL;
	graph->edge_list = NULL;
	graph->feature = NULL;
}
//�O���t�̃T�C�Y��ύX
void ChangeRateGraph(GRAPH *graph, long double rate,GRAPH *def_graph){
	int i;
	for(i = 0 ; i < graph->count; i++){
		(graph->pt_list+i)->x = (LONG)((def_graph->pt_list+i)->x*rate);
		(graph->pt_list+i)->y = (LONG)((def_graph->pt_list+i)->y*rate);
	}
}
//�O���t���ړ�
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
//�O���t�̒��S���w�肵���_�ֈړ�
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
//�O���t���R�s�[
void CopyGraph(GRAPH *dstGraph,GRAPH *srcGraph,int feature_w){

	int i;
	
	//������
	DeleteGraph(dstGraph);
	InitGraph(dstGraph);

	*dstGraph = *srcGraph;

	//���O���R�s�[
	strcpy(dstGraph->name,srcGraph->name);

	dstGraph->pt_list = (POINT *)malloc(sizeof(POINT) *srcGraph->count);
	dstGraph->edge_list = (Edge *)malloc(sizeof(Edge) * srcGraph->count * srcGraph->count);
	dstGraph->feature = (long double *)malloc(sizeof(long double) * srcGraph->count * feature_w);

	//�_���R�s�[
	for(i = 0 ; i < srcGraph->count; i++){
		*(dstGraph->pt_list+i) = *(srcGraph->pt_list+i);
		*(dstGraph->feature+i) = *(srcGraph->feature+i);
	}
	//�����R�s�[
	for(i = 0 ; i< srcGraph->line_count;i++){
		*(dstGraph->edge_list+i) = *(srcGraph->edge_list+i);
	}

}
//�O���t�����
void DeleteGraph(GRAPH *graph){
	free(graph->pt_list);
	free(graph->edge_list);
	free(graph->feature);
	InitGraph(graph);
}
//�O���t�̃T�C�Y���擾
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
