#include "biImage.h"

//===========================================================================================================================
//画像を初期化
void biInitImage(IMAGE *image){

	image->bitCount = 0;
	image->data_ofset = 0;
	image->file_size = 0;
	image->height = 0;
	image->width = 0;

	image->image_data = NULL;
	image->head_data = NULL;
}

//画像データ領域を取得
void biCreateImage(IMAGE *image, int width, int height, int bitCount){
	IMAGE_HEADER header;
	int i;
	biInitImage(image);

	//ヘッダーを設定
	//BITMAPFILEHEADER
	strcpy((char *)&header.file.bfType,"BM");
	header.file.bfSize =  0;
	header.file.bfReserved1 = 0;
	header.file.bfReserved2 = 0;
	header.file.bfOffBits = 54;

	//BITMAPINFOHEADER
	header.info.biSize = 40;
	header.info.biWidth = width;
	header.info.biHeight = height;
	header.info.biPlanes = 1;
	header.info.biBitCount = bitCount;
	header.info.biCompression = BI_RGB;
	header.info.biSizeImage = 0;
	header.info.biXPelsPerMeter = 0;
	header.info.biYPelsPerMeter = 0;
	header.info.biClrUsed = 0;
	header.info.biClrImportant = 0;

	image->bitCount = bitCount;
	image->data_ofset = header.file.bfOffBits;
	image->width = width;
	image->height = height;
	image->file_size = 0;

	image->head_data = (unsigned char*)malloc(sizeof(unsigned char)*image->data_ofset); //ヘッダー保存領域を確保

	for(i = 0 ; i < sizeof(header.file) ; i++){
		*(image->head_data+i) = *((char*)(&header.file)+i);
	}
	for(i = 0 ; i < sizeof(header.info) ; i++){
		*(image->head_data+sizeof(header.file)+i) = *((char*)(&header.info)+i);
	}

	image->image_data = (IMAGE_DATA*)malloc(sizeof(IMAGE_DATA)*width*height);
	image->image_data = (IMAGE_DATA*)memset(image->image_data, 0, sizeof(IMAGE_DATA)*width*height);

}

//ビットマップ画像をコピー   <changed by tanakura 2011,10,07>
void biCopyImage(IMAGE *dstimage, IMAGE srcimage){
	unsigned int i,j;

	if(dstimage->width < srcimage.width || dstimage->height < srcimage.height
		|| dstimage->image_data == NULL || dstimage->head_data == NULL){
		free(dstimage->image_data);//画素領域を解放
		free(dstimage->head_data);//ヘッダー領域を解放
		dstimage->image_data = (IMAGE_DATA*)malloc(sizeof(IMAGE_DATA)*(srcimage.width)*(srcimage.height));//画像データ用領域を確保
		dstimage->head_data = (unsigned char *)malloc(sizeof(unsigned char)*srcimage.data_ofset);//ヘッダー保存領域を確保
	}
	
	dstimage->width = srcimage.width;
	dstimage->height = srcimage.height;
	dstimage->bitCount = srcimage.bitCount;
	dstimage->data_ofset = srcimage.data_ofset;
	
	//ヘッダーの内容をコピー
	for(i = 0 ; i < dstimage->data_ofset ; i++){
		*(dstimage->head_data+i) = *(srcimage.head_data+i);
	}
	//画素の内容をコピー
	for(j = 0 ; j < (unsigned int)dstimage->height ; j++){
		for(i = 0 ; i < (unsigned int)dstimage->width ; i++){
			*(dstimage->image_data+i+j*dstimage->width) = *(srcimage.image_data+i+j*srcimage.width);
		}
	}
}

//ビットマップ画像をファイル出力　 <changed by tanakura 2011,10,07>
int biWriteImage(char *filename, IMAGE *image){
	FILE *op;
	int i,size;

	op = fopen( filename,"wb");
	if(op == NULL ){
		puts("画像が開けません(biWriteImage)");
		return -1;
	}

	size = image->width * image->height;

	fseek(op,0,SEEK_SET);
	
	//ヘッダー書き込み
	fwrite(image->head_data,sizeof(unsigned char), image->data_ofset, op);

	//画素を上下反転
	biReverseImage(image);

	for(i=0;i<size;i++){
		fwrite( &(image->image_data)[i].rgbBlue	, sizeof (unsigned char),1, op );
		fwrite( &(image->image_data)[i].rgbGreen, sizeof (unsigned char),1, op );
		fwrite( &(image->image_data)[i].rgbRed	, sizeof (unsigned char),1, op );
		
		if(image->bitCount == 32){
			fwrite(&(image->image_data)[i].rgbReserved, sizeof (unsigned char),sizeof (unsigned char), op );
		}
	}

	//画素を上下反転
	biReverseImage(image);

	fclose(op);

	return 0;
}

//ビットマップ画像を読み込む　 <changed by tanakura 2011,10,07>
int biLoadImage(char *filename, IMAGE *image){
	FILE *fp;
	int i,size;

	//初期化
	biBreakImage(image);
	biInitImage(image);
	
	fp = fopen( filename,"rb");
	if(fp == NULL ){
		puts("画像が開けません(biLoadImage)");
		return -1;
	}

	//ヘッダー部分読み取り
	fseek(fp,sizeof(unsigned char)*2,SEEK_CUR);
	fread( &image->file_size, sizeof (unsigned int),1, fp );//ファイルサイズ
	fseek(fp,sizeof(unsigned char)*4,SEEK_CUR);
	fread( &image->data_ofset, sizeof (unsigned int),1, fp );//データオフセット(ヘッダサイズ)
	fseek(fp,sizeof(unsigned char)*4,SEEK_CUR);
	fread( &image->width, sizeof(long),1,fp );//幅
	fread( &image->height, sizeof(long),1,fp);//高さ
	fseek(fp,sizeof(unsigned char)*2,SEEK_CUR);
	fread( &image->bitCount, sizeof(unsigned short),1,fp);//ビットの深さ

	image->head_data = (unsigned char *)malloc(sizeof(unsigned char)*image->data_ofset);//ヘッダーの保存領域を確保

	fseek(fp,0,SEEK_SET);
	fread( image->head_data, sizeof (unsigned char),image->data_ofset, fp );//ヘッダー読み込み

	size = image->width * image->height;
	image->image_data = (IMAGE_DATA*)malloc(sizeof(IMAGE_DATA)*(image->width)*(image->height));//画像データ用領域を確保

	//画像読み込み
	for(i=0;i<size;i++){
		fread( &(image->image_data)[i].rgbBlue	, sizeof (char),sizeof (char), fp );
		fread( &(image->image_data)[i].rgbGreen	, sizeof (char),sizeof (char), fp );
		fread( &(image->image_data)[i].rgbRed	, sizeof (char),sizeof (char), fp );

		if(image->bitCount == 32)fread( &(image->image_data)[i].rgbReserved	, sizeof (char),sizeof (char), fp );

		(image->image_data)[i].graylevel = ( (image->image_data)[i].rgbBlue + (image->image_data)[i].rgbRed + (image->image_data)[i].rgbGreen ) / 3; //単純平均法でモノクロに

	}

	//画素を上下反転
	biReverseImage(image);

	fclose(fp);

	return 0;
}


//画像を部分的にコピー
void biCutImage(IMAGE *image,IMAGE *output,RECT_I *rect){
	int x = rect->left;				  //切り取る左上x座標
	int y = rect->top;				  //切り取る左上y座標
	int w = rect->right - rect->left; //切り取る幅
	int h = rect->bottom - rect->top; //切り取る高さ
	int i,j; //幅、高さ

	biInitImage(output);
	biCreateImage(output,w,h,32);

	for(i = 0 ; i < w ; i++){
		for(j = 0 ; j < h ; j++){
			*(output->image_data+i+j*w) = *(image->image_data+(x+i)+(y+j)*image->width);
		}
	}

}

//baseの(x,y)にimageを張り付ける
void biPasteImage(IMAGE *base,int x, int y, IMAGE image){
	int i,j;

	for(i = 0 ; i < image.width ; i++){
		for(j = 0 ; j < image.height ; j++){
			if((x+i) < base->width && (y+j) < base->height){
				*(base->image_data+(x+i)+(y+j)*base->width) = *(image.image_data+i+j*image.width);
			}
		}
	}
}

//IplImageからbiImageを取得
void biChangeCvImage(IplImage cvimg,IMAGE *img){
	int x,y;

	if(cvimg.width != img->width || cvimg.height != img->height){
		biBreakImage(img);
		biCreateImage(img,cvimg.width,cvimg.height,24);
	}

	if(cvimg.depth == IPL_DEPTH_8U && cvimg.nChannels == 3){
		for(y = 0 ; y < cvimg.height ; y++){
			for(x = 0 ; x < cvimg.width ; x++){
				(img->image_data+x+(cvimg.height-1-y)*cvimg.width)->rgbBlue		=	*(cvimg.imageData+x*3+0+y*cvimg.widthStep);
				(img->image_data+x+(cvimg.height-1-y)*cvimg.width)->rgbGreen		=	*(cvimg.imageData+x*3+1+y*cvimg.widthStep);
				(img->image_data+x+(cvimg.height-1-y)*cvimg.width)->rgbRed		=	*(cvimg.imageData+x*3+2+y*cvimg.widthStep);
				(img->image_data+x+(cvimg.height-1-y)*cvimg.width)->graylevel = ( *(cvimg.imageData+x*3+0+y*cvimg.widthStep) + *(cvimg.imageData+x*3+1+y*cvimg.widthStep) + *(cvimg.imageData+x*3+2+y*cvimg.widthStep)) / 3;
			}
		}
	}

	//画素を上下反転
	biReverseImage(img);
}

//画像を破棄する <changed by tanakura 2011,10,26>
void biBreakImage(IMAGE *image){
	free(image->image_data);
	free(image->head_data);
	biInitImage(image);
}


//上下反転
void biReverseImage(IMAGE *image){
	int i,j;

	IMAGE_DATA *data;

	data = (IMAGE_DATA*)malloc(sizeof(IMAGE_DATA)*image->width*image->height);

	for(j = 0 ; j < image->height ; j++){
		for(i = 0 ; i < image->width ; i++){
			*(data+i+j*image->width) = *(image->image_data+i+((image->height-1)-j)*image->width);
		}
	}

	free(image->image_data);
	image->image_data = data;
}