//changed by tanakura 2011.12.02
#ifndef BIIMAGE_INCLUDE
#define BIIMAGE_INCLUDE

#include "cv.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <Windows.h>

#pragma warning(disable : 4996)

//画像ヘッダー情報
typedef struct image_header_data_tag{
	BITMAPFILEHEADER file;
	BITMAPINFOHEADER info;
}IMAGE_HEADER;

//画像データ用構造体
typedef struct image_data_tag{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
	unsigned char graylevel;
	unsigned char label;
}IMAGE_DATA;

//24・32ビット画像用
typedef struct image_tag{
	unsigned int file_size;
	unsigned int data_ofset;//ヘッダーの長さ
	long width;
	long height;
	unsigned short bitCount; //ビットの深さ
	unsigned char *head_data;
	IMAGE_DATA *image_data;
}IMAGE;

//四角形の左上・右下座標
typedef struct rect_i_tag{
	int top;
	int left;
	int right;
	int bottom;
}RECT_I;

void biInitImage(IMAGE *image);
void biCreateImage(IMAGE *image, int width, int height, int bitCount);
int biLoadImage(char *filename, IMAGE *image);
int biWriteImage(char *filename, IMAGE *image);
void biCopyImage(IMAGE *dstimage, IMAGE srcimage);
void biBreakImage(IMAGE *image);
void biPasteImage(IMAGE *base,int x, int y, IMAGE image);
void biCutImage(IMAGE *image,IMAGE *output,RECT_I *rect);
void biChangeCvImage(IplImage cvimg,IMAGE *img);
void biReverseImage(IMAGE *image);

#endif