#ifndef LIB_INCLUDE
#define LIB_INCLUDE

#pragma warning(disable : 4996)

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
#include "biImage.h"

//====================================================

//2値画像用
void expand(IMAGE *input, IMAGE *output);
void shrink(IMAGE *input, IMAGE *output);
int niti(int num, int threshold);
void auto_niti(IMAGE *image);
void sobelfilter(IMAGE *image);
int GetThreshold(IMAGE *image);
int limit(int num, int up, int down);
//----------------------------------------------------
void search(IMAGE *image, int num, RECT_I *rect);
void draw_frame(RECT_I *rect, IMAGE *image, int graylevel,IMAGE *output);
//====================================================

//YIQ用
void ChangeYIQ(IMAGE *image,IMAGE *outimage,int Ylow,int Yhigh,int Ilow,int Ihigh, int Qlow,int Qhigh);
void incline(IMAGE *image,IMAGE *outimage,double R, double G, double B);
//====================================================

//ヒストグラム系
void MakeHistogram(IMAGE *image,int *histogram);
void MakeDiagram(IMAGE *image, int x, int y, int width, int height, IMAGE *diagram, int roll);
void SaveHistogram(int *histogram);
void SaveHistogramImage(int *histogram,int pixelnum);
void ChangeContrast(IMAGE *image, IMAGE *outimage);
//====================================================

//ラベリング
int min_calc(int *p);
int  Lookup_update(int i,int * label);
int color_put(IMAGE *image,int max, RECT *rc);
int label(IMAGE *image);
void image_search(IMAGE *image,RECT_I *rect);
//====================================================

#endif
