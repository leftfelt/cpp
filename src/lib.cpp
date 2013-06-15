#include "lib.h"
//===========================================================================================================================
//膨張 <changed by tanakura 2011,10,07>
void expand(IMAGE *input,IMAGE *output){
	int i,j,k,l;
	int cnt;

	for(j = 0 ; j < input->height ; j++){
		for(i = 0 ; i < input->width ; i++){
			cnt = 0;
			//白だったらcontinue
			if((input->image_data+i+j*input->width)->graylevel == 255){
				*(output->image_data+i+j*input->width) = *(input->image_data+i+j*input->width);
				continue;
			}//*/
		//*
			//近傍値を取得
			for(k = -1 ; k <= 1  ; k++){
				for(l = -1 ; l <= 1 ; l++){
					if(i+l<0 || j+k<0 || i+l>=input->width || j+k>=input->height)continue;
					if(k != 1 || l != 1){
						if((input->image_data+(i+l)+(j+k)*input->width)->graylevel == 255){
							cnt++;
						}
					}
				}
			}//*/

			if(cnt != 0){
				(output->image_data+i+j*input->width)->rgbBlue		= 255;
				(output->image_data+i+j*input->width)->rgbGreen		= 255;
				(output->image_data+i+j*input->width)->rgbRed		= 255;
				(output->image_data+i+j*input->width)->graylevel	= 255;
			}else{
				*(output->image_data+i+j*input->width) = *(input->image_data+i+j*input->width);
			}
		}
	}
}

//収縮 <changed by tanakura 2011,10,07>
void shrink(IMAGE *input, IMAGE *output){
	int i,j,k,l;
	int cnt;

	for(j = 0 ; j < input->height ; j++){
		for(i = 0 ; i < input->width ; i++){
			cnt = 0;
			//黒だったらcontinue
			if((input->image_data+i+j*input->width)->graylevel == 0){
				*(output->image_data+i+j*input->width) = *(input->image_data+i+j*input->width);
				continue;
			}//*/
		//*
			//近傍値を取得
			for(k = -1 ; k <= 1  ; k++){
				for(l = -1 ; l <= 1 ; l++){
					if(i+l<0 || j+k<0 || i+l>=input->width || j+k>=input->height)continue;
					if(k != 1 || l != 1){
						if((input->image_data+(i+l)+(j+k)*input->width)->graylevel == 0){
							cnt++;
						}
					}
				}
			}//*/

			if(cnt != 0){
				(output->image_data+i+j*input->width)->rgbBlue		= 0;
				(output->image_data+i+j*input->width)->rgbGreen		= 0;
				(output->image_data+i+j*input->width)->rgbRed		= 0;
				(output->image_data+i+j*input->width)->graylevel	= 0;
			}else{
				*(output->image_data+i+j*input->width) = *(input->image_data+i+j*input->width);
			}
		}
	}
}


//Sobelフィルター
void sobelfilter(IMAGE *image){
	int width, height, graylevel;
	int pos = 0;
	int num[9];//近傍領域
	int i,j,k;
	int dx,dy,gray;
	IMAGE outimage;

	biInitImage(&outimage);
	biCopyImage(&outimage,*image);
	width = image->width;
	height = image->height;
	graylevel = 255;

	//カラー画像を二値化
	//auto_niti(&outimage);
	
	//Sobelフィルター
	for(i = 0 ; i < width*height ; i++){
		pos = 0;
		//近傍領域の値を取得
		for(k = -1 ; k <= 1 ; k++){
			for(j = -1; j <= 1 ; j++){
				pos = i+j+k*width;
				if((pos/width) == 0 || (pos/width) >= height-1 || (pos%width) == 0 || (pos-(width-1))%width == 0){
					num[(j+1)+(k+1)*3] = 0;
				}else{
					num[(j+1)+(k+1)*3] = (outimage.image_data+i+j+k*width)->graylevel;
				}
			}
		}
		
		dx = num[0] + 2*num[3] + num[6];
		dx = dx - num[2] - 2*num[5] - num[8];
		dy = num[0] + 2*num[1] + num[2];
		dy = dy - num[6] - 2*num[7] - num[8];

		gray = abs(dx)+abs(dy);

		if(gray > graylevel) gray = graylevel;
		(image->image_data+i)->rgbRed = gray;
		(image->image_data+i)->rgbGreen = gray;
		(image->image_data+i)->rgbBlue = gray;
		(image->image_data+i)->graylevel = gray;
	}
	biBreakImage(&outimage);
}

//2値化-閾値以下なら0以上なら1- <changed by tanakura 2011,10,07>
int niti(int num, int threshold){
	if(num >= threshold){
		return 1;
	}else{
		return 0;
	}
}


//2値化-閾値以下なら0以上なら1- <changed by tanakura 2011,10,26>
void auto_niti(IMAGE *image){
	int i,threshold;
	//*二値化
	threshold = GetThreshold(image);
	
	for(i = 0 ; i < image->width*image->height ; i++){
		(image->image_data+i)->graylevel = niti((image->image_data+i)->graylevel,threshold)*255;
		(image->image_data+i)->rgbRed	= (image->image_data+i)->graylevel;
		(image->image_data+i)->rgbGreen	= (image->image_data+i)->graylevel;
		(image->image_data+i)->rgbBlue	= (image->image_data+i)->graylevel;
	}//*/
}

//判別分析法 <changed by tanakura 2011,10,07>
int GetThreshold(IMAGE *image){
	int histogram[256];
	int i,j;
	int a,b;//下限・上限
	int sum = 0;//合計
	int n1 = 0,n2 = 0;//画素数
	double u0,u1,u2;//平均
	double O12 = 0,O22 = 0;//分散
	double OW2;//クラス内分散
	double OB2;//クラス間分散
	int t = 0;//閾値
	int threshold = 0;//決定閾値
	double proportion = -1;//比率

	MakeHistogram(image,histogram);
	
	//下限取得
	for(i = 0; i <= 255 ; i++){
		if((*(histogram+i)) != 0){
			a = i;
			break;
		}
	}
	//上限取得
	for(i = 255; i >= 0 ; i--){
		if((*(histogram+i)) != 0){
			b = i;
			break;
		}
	}
	//合計
	for(i = 0 ; i <= 255 ; i++){
		sum += i*(*(histogram+i));
	}

	//平均値
	u0 = sum / (image->width*image->height);

	for(t = a ; t <= b ; t++){
		//合計
		sum = 0;
		n1 = 0;
		for(i = a ; i <= t ; i++){
			sum += i*(*(histogram+i));
			n1 += *(histogram+i);
		}

		u1 = sum / n1; //平均値
	
		//分散
		O12 = 0;
		for(i = a ; i <= t ; i++){
			for(j = 0 ; j < *(histogram+i) ; j++){
				O12 += pow((i - u0),2);
			}
		}

		O12 = O12 / n1;
		sum = 0;
		n2 = 0;
		for(i = t ; i <= b ; i++){
			sum += i*(*(histogram+i));
			n2 += *(histogram+i);
		}

		u2 = sum / n2; //平均値

		O22 = 0;
		//分散
		for(i = t ; i <= b ; i++){
			for(j = 0 ; j < *(histogram+i) ; j++){
				O22 += pow((i - u0),2);
			}
		}

		O22 = O22 / n2;

		OW2 = ( n1 * O12 + n2 * O22 ) / (n1 + n2);
		OB2 = ( n1*pow((u1 - u0),2) + n2*pow((u2-u0),2) ) / (n1+n2);

		if(proportion < (OB2/OW2)){
			proportion = OB2/OW2;
			threshold = t;
		}
	}

	return threshold;
}

//値のとる範囲を制限 <changed by tanakura 2011,10,07>
int limit(int num, int up, int down){
	if(num > up){
		return up;
	}else if(num < down){
		return down;
	}else{
		return num;
	}
}



//指定した値を含む領域を探す <changed by tanakura 2011,10,07>
void search(IMAGE *image, int num, RECT_I *rect){
	int i,j;
	rect->top = -1;
	rect->left = image->width;
	rect->right = -1;
	rect->bottom = -1;
	for(i = 1 ; i < image->height-1 ; i++){
		for(j = 1 ; j < image->width-1 ; j++){
			if((image->image_data+j+i*image->width)->graylevel == num){
				if(rect->top == -1) rect->top = i;
				if(rect->left >= j) rect->left = j;
				if(rect->right <= j)rect->right = j;
				if(rect->bottom <= i)rect->bottom = i;
			}
		}
	}
}

//枠で囲う(カラー) <changed by tanakura 2011,10,07>
void draw_frame(RECT_I *rect, IMAGE *image, int graylevel,IMAGE *output){
	int i,j;

	int x = rect->left;				  //枠の左上x座標
	int y = rect->top;				  //枠の左上y座標
	int w = rect->right - rect->left; //枠の幅
	int h = rect->bottom - rect->top; //枠の高さ


	for(i = y ; i <= y+h;i++){
		for(j = 0 ; j < 5 ; j++){ 
			(image->image_data+x+j+i*image->width)->rgbBlue = 255;
			(image->image_data+x+j+i*image->width)->rgbRed = 255;
			(image->image_data+x+j+i*image->width)->rgbGreen = 0;
			(image->image_data+x+j+i*image->width)->graylevel = graylevel;

			(image->image_data+x-j+w+i*image->width)->rgbBlue = 255;
			(image->image_data+x-j+w+i*image->width)->rgbRed = 255;
			(image->image_data+x-j+w+i*image->width)->rgbGreen = 0;
			(image->image_data+x-j+w+i*image->width)->graylevel = graylevel;
		}
	}
	for(i = x ; i <= x+w ;i++){
		for(j = 0 ; j < 5 ; j++){ 
			(image->image_data+i+(y+j)*image->width)->rgbBlue = 255;
			(image->image_data+i+(y+j)*image->width)->rgbRed = 255;
			(image->image_data+i+(y+j)*image->width)->rgbGreen = 0;
			(image->image_data+i+(y+j)*image->width)->graylevel = graylevel;

			(image->image_data+i+(y+h-j)*image->width)->rgbBlue = 255;
			(image->image_data+i+(y+h-j)*image->width)->rgbRed = 255;
			(image->image_data+i+(y+h-j)*image->width)->rgbGreen = 0;
			(image->image_data+i+(y+h-j)*image->width)->graylevel = graylevel;
		}
	}
}


//===========================================================================================================================
//傾斜をかける
void incline(IMAGE *image,IMAGE *outimage,double R, double G, double B){
	
	int i;

	//傾斜
	for(i = 0 ; i < image->width*image->height ; i++){
		(outimage->image_data+i)->rgbRed	= (unsigned char)limit((int)((outimage->image_data+i)->rgbRed	*R)	,255,0);
		(outimage->image_data+i)->rgbGreen  = (unsigned char)limit((int)((outimage->image_data+i)->rgbGreen	*G)	,255,0);
		(outimage->image_data+i)->rgbBlue	= (unsigned char)limit((int)((outimage->image_data+i)->rgbBlue	*B)	,255,0);
		(outimage->image_data+i)->graylevel = ((outimage->image_data+i)->rgbRed + (outimage->image_data+i)->rgbGreen + (outimage->image_data+i)->rgbBlue) / 3;
	}

}

//肌色抽出 <changed by tanakura 2011,10,07>
void ChangeYIQ(IMAGE *image,IMAGE *outimage,int Ylow,int Yhigh,int Ilow,int Ihigh, int Qlow,int Qhigh)
{ 
	int i,size;
	int yiq_I,yiq_Q,yiq_Y;

	size = image->width * image->height;
	for(i=0;i<size;i++){
		yiq_Y = (unsigned char)(0.299 * (image->image_data)[i].rgbRed + 0.587 * (image->image_data)[i].rgbGreen + 0.114 * (image->image_data)[i].rgbBlue);
		yiq_I = (unsigned char)(0.596 * (image->image_data)[i].rgbRed - 0.274 * (image->image_data)[i].rgbGreen - 0.322 * (image->image_data)[i].rgbBlue);
		yiq_Q = (unsigned char)(0.211 * (image->image_data)[i].rgbRed - 0.522 * (image->image_data)[i].rgbGreen - 0.311 * (image->image_data)[i].rgbBlue);
		if( (Ylow <= yiq_Y && yiq_Y <= Yhigh) &&
			((Ilow <= yiq_I && yiq_I <= Ihigh) || (Ilow == Ihigh)) && 
			((Qlow <= yiq_Q && yiq_Q <= Qhigh) || (Qlow == Qhigh)) 
			){ 
			(outimage->image_data)[i].rgbBlue	= 255;
			(outimage->image_data)[i].rgbGreen	= 255;
			(outimage->image_data)[i].rgbRed	= 255;
			(outimage->image_data)[i].graylevel = 255;
		}else{
			(outimage->image_data)[i].rgbBlue	= 0;
			(outimage->image_data)[i].rgbGreen	= 0;
			(outimage->image_data)[i].rgbRed	= 0;
			(outimage->image_data)[i].graylevel = 0;
		}
	}

}

//ラベリング=====================================================================================
//===============================================================================================
int min_calc(int *p)
{
	int i,min=1000;

	for(i=0;i<8;i++){
		if( *(p+i)!=0 ){
				if(min > *(p+i)){
				min = *(p+i);
			}
		}
	}

	return min;
}

//===============================================================================================
int  Lookup_update(int i,int * label)
{
	int a;

	if(i == label[i]){
		return label[i];
	}else{
		a =  Lookup_update(label[i],label);
	}

	return a;
}

//===============================================================================================
//指定したラベルだけ抽出
int color_put(IMAGE *image,int max,RECT *rc)
{
	int i,j,area = 0;
	rc->top = -1;
	rc->left = image->width;
	rc->right = -1;
	rc->bottom = -1;
	//指定したラベルだけ抽出
	for(j = 0 ; j < image->height;j++){
		for(i = 0;i<image->width;i++){
			if((image->image_data+i+j*image->width)->label == max){
				(image->image_data)[i+j*image->width].rgbBlue	= 255;
				(image->image_data)[i+j*image->width].rgbGreen	= 255;
				(image->image_data)[i+j*image->width].rgbRed	= 255;
				(image->image_data)[i+j*image->width].graylevel	= 255;
				if(rc->top == -1) rc->top = j;
				if(rc->left >= i) rc->left = i;
				if(rc->right <= i)rc->right = i;
				if(rc->bottom <= j)rc->bottom = j;
				area++;
			}else{
				(image->image_data)[i+j*image->width].rgbBlue	= 0;
				(image->image_data)[i+j*image->width].rgbGreen	= 0;
				(image->image_data)[i+j*image->width].rgbRed	= 0;
				(image->image_data)[i+j*image->width].graylevel	= 0;
			}
		}
	}

	if(area == 0){
		rc->top		= 0;
		rc->left	= 0;
		rc->right	= 0;
		rc->bottom	= 0;
	}

	return area;//面積を返す
}

//ラベリング=====================================================================================
//独立したオブジェクトにラベルを振る
int label(IMAGE *image)
{
	int w,h,j,k,i,size;
	int label_count=1,flag_count;
	int min_data[8],min;
	int *Lookup_table,*sub_table;
	int max=0,label=0,Lookup_flag=0;
	
	Lookup_table = (int *)malloc(sizeof(int) * image->width * image->height);
	sub_table = (int *)malloc(sizeof(int) * image->width * image->height);

	//初期化
	size = image->width * image->height;
	for(i = 0 ;i< size;i++){
		(image->image_data+i)->label = 0;
	}
	for(i=0;i<size;i++){
		Lookup_table[i] = i;
	}

	//ラベリング
	//*
	for(k=0;k<2;k++){
		for(h = 1; h < image->height-1;h++){
			for(w=1;w<image->width-1;w++){
				if((image->image_data+w+h*image->width)->graylevel == 255){
					flag_count = 0;
					if((image->image_data+(w-1)+(h-1)*image->width)->label == 0) flag_count++;
					if((image->image_data+w+(h-1)*image->width)->label  == 0) flag_count++;
					if((image->image_data+(w+1)+(h-1)*image->width)->label  == 0) flag_count++;
					if((image->image_data+(w-1)+h*image->width)->label  == 0) flag_count++;
	
					if((image->image_data+(w+1)+h*image->width)->label  == 0) flag_count++;
					if((image->image_data+(w-1)+(h+1)*image->width)->label  == 0) flag_count++;
					if((image->image_data+w+(h+1)*image->width)->label  == 0) flag_count++;
					if((image->image_data+(w+1)+(h+1)*image->width)->label  == 0) flag_count++;
					
					if(flag_count == 8){//周りがすべて0だった
						//■■■
					    //■□■
						//■■■
						(image->image_data+w+h*image->width)->label  =label_count;//ラベルを振る
						label_count++;
					}else{//一か所でも255があった
						//１■４
					    //■□■
						//■■■
						min_data[0]= (image->image_data+(w-1)+(h-1)*image->width)->label ;
						min_data[1]= (image->image_data+w+(h-1)*image->width)->label ;
						min_data[2]= (image->image_data+(w+1)+(h-1)*image->width)->label ;
						min_data[3]= (image->image_data+(w-1)+h*image->width)->label ;
	
						min_data[4]= (image->image_data+(w+1)+h*image->width)->label ;
						min_data[5]= (image->image_data+(w-1)+(h+1)*image->width)->label ;
						min_data[6]= (image->image_data+w+(h+1)*image->width)->label ;
						min_data[7]= (image->image_data+(w+1)+(h+1)*image->width)->label ;
	
						min = min_calc(min_data);//最小のラベル番号を調べる
						//１■４
					    //■１■
						//■■■
	
						for(j=0;j<8;j++){
							if(min_data[j] != min && min_data[j] != 0){
								if(Lookup_table[min_data[j]] >= min){
									Lookup_table[min_data[j]] = min;
								}
							}
						}
											
						(image->image_data+w+h*image->width)->label = min;
					}
				}
			}
		}
		
		for(h=0;h<label_count;h++){
			if(h != Lookup_table[h]){
				Lookup_table[h] = Lookup_update(h,Lookup_table);
			}
		}

			
		for(h = 0; h < image->height;h++){
			for(w=0;w<image->width;w++){
				if((image->image_data+w+h*image->width)->label  != 0){
					(image->image_data+w+h*image->width)->label  = Lookup_table[(image->image_data+w+h*image->width)->label ];
				}
			}
		}
	}

	
	i=0;
	sub_table = (int *)memset(sub_table,-1,sizeof(int) * image->width * image->height);
	for(h=0;h<label_count;h++){
		if(sub_table[Lookup_table[h]] == -1){
			sub_table[Lookup_table[h]] = i;
			i++;
		}
	}
	label = i;
	//本ラベルに書き換え
	for(h=0;h<label_count;h++){
		Lookup_table[h] = sub_table[Lookup_table[h]];
	}

	//ラベルの割り振り
	for(h = 0; h < image->height;h++){
		for(w=0;w<image->width;w++){
			if((image->image_data+w+h*image->width)->label  != 0){
				(image->image_data+w+h*image->width)->label  = Lookup_table[(image->image_data+w+h*image->width)->label ];
			}
		}
	}
	
	free(Lookup_table);
	free(sub_table);

	return label;
}


void image_search(IMAGE *image,RECT_I *rect)
{
	int i,j;
	rect->top = -1;
	rect->left = image->width;
	rect->right = -1;
	rect->bottom = -1;

	for(i = 1 ; i < image->height-1 ; i++){
		for(j = 1 ; j < image->width-1 ; j++){
			if((image->image_data+j+i*image->width)->rgbGreen == 255){
				if(rect->top == -1) rect->top = i;
				if(rect->left >= j) rect->left = j;
				if(rect->right <= j)rect->right = j;
				if(rect->bottom <= i)rect->bottom = i;
			}
		}
	}

	//printf("rect->top=%d rect->left=%d rect->right=%d rect->bottom =%d\n",rect->top, rect->left, rect->right, rect->bottom);
}



//===========================================================================================================================

//ヒストグラムを作る <changed by tanakura 2011,10,07>
void MakeHistogram(IMAGE *image,int *histogram){
	int i;
	for(i = 0; i < 256 ; i++){
		*(histogram+i)=0;
	}
	for(i = 0; i < image->width*image->height ; i++){
		(*(histogram+(image->image_data+i)->graylevel))++;
	}
}

//指定範囲の濃度分布画像を作る <changed by tanakura 2011,10,26>
void MakeDiagram(IMAGE *image, int x, int y, int width, int height, IMAGE *diagram, int roll){
	int i,j,k,cnt;
	double num;

	//画像を作成
	
	if(roll == 1){ //下向きのグラフ
		biCreateImage(diagram,120,height*width,24);
	}else{//右向きのグラフ
		biCreateImage(diagram,height*width,120,24);
	}
	
	for(j = 0; j < height; j++){
		for(i = 0 ; i < width ; i++){
			if(roll == 1){
				num = (((image->image_data+i+x+(j+y)*image->width)->graylevel/(double)255)*100);
			}else{
				num = (((image->image_data+i+x+(y+height-j)*image->width)->graylevel/(double)255)*100);
			}
			for(k = 0 ; k < (int)num ; k++){
				if(roll == 1){
					cnt = k+(i+j)*120;
				}else{
					cnt = k*height+(i+j);
				}
				(diagram->image_data+cnt)->rgbBlue		= 0;
				(diagram->image_data+cnt)->rgbGreen		= 0;
				(diagram->image_data+cnt)->rgbRed		= 0;
				(diagram->image_data+cnt)->graylevel	= 0;
			}
		}
	}

	//画像をファイルに出力
	biWriteImage("diagram.bmp",diagram);
}

//ヒストグラムをファイルに出力 <changed by tanakura 2011,10,21>
void SaveHistogram(int *histogram){

	FILE *w_histogram;
	int i;

	//ヒストグラムをファイルに出力
	w_histogram = fopen("before_histogram.txt","w");

	for(i = 0 ; i <= 255 ; i++){
		fprintf(w_histogram, "%4d,", *(histogram+i));
		if((i+1)%10 == 0)fprintf(w_histogram,"\n");
	}

	fclose(w_histogram);

}

//ヒストグラムの濃度分布画像を出力 <changed by tanakura 2011,10,21>
void SaveHistogramImage(int *histogram,int pixelnum){
	IMAGE histogram_image;

	int i,j;
	int width = 256, height = 256;
	int max = 0;

	for(i = 0 ; i < 256 ; i++){
		if(max < *(histogram+i)){
			max = *(histogram+i);
		}
	}
	
	//画像を作成
	biCreateImage(&histogram_image,width,height,32);
		
	for(i = 0 ; i < width ; i++){
		for(j = 0; j < /*100*/(int)(((double)*(histogram+i)/pixelnum)*256); j++){
			(histogram_image.image_data+i+j*width)->rgbBlue			= 255;
			(histogram_image.image_data+i+j*width)->rgbGreen		= 0;
			(histogram_image.image_data+i+j*width)->rgbRed			= 255;
			(histogram_image.image_data+i+j*width)->graylevel		= (255 + 255) / 3;
		}
	}

	//画像をファイルに出力
	biWriteImage("histogram.bmp",&histogram_image);
}

//コントラストを改善

void ChangeContrast(IMAGE *image, IMAGE *outimage){
	int histogram[256];
	int i,a = 0,b = 255;

	MakeHistogram(image,histogram);

	//コントラストを変える
	for(i = 0; i <= 255 ; i++){
		if(histogram[i] != 0){
			a = i;
			break;
		}
	}

	for(i = 255; i >= 0 ; i--){
		if(histogram[i] != 0){
			b = i;
			break;
		}
	}

	biCopyImage(outimage, *image);

	//コントラストの改善
	for(i = 0 ; i < image->width * image->height ; i++){
		if((image->image_data+i)->graylevel >= 0 && (image->image_data+i)->graylevel < a){
			(outimage->image_data+i)->rgbRed	 = 0;
			(outimage->image_data+i)->rgbGreen	 = 0;
			(outimage->image_data+i)->rgbBlue	 = 0;
			(outimage->image_data+i)->graylevel	 = 0;

		}else if((image->image_data+i)->graylevel >=a && (image->image_data+i)->graylevel <= b){
			//*
			(outimage->image_data+i)->graylevel  =(unsigned char)( 255 * (((outimage->image_data+i)->graylevel - a) / (double)(b - a)));
			(outimage->image_data+i)->rgbRed	 *= (unsigned char)((outimage->image_data+i)->graylevel/(double)(image->image_data+i)->graylevel);
			(outimage->image_data+i)->rgbGreen	 *= (unsigned char)((outimage->image_data+i)->graylevel/(double)(image->image_data+i)->graylevel);
			(outimage->image_data+i)->rgbBlue	 *= (unsigned char)((outimage->image_data+i)->graylevel/(double)(image->image_data+i)->graylevel);

		}else if((image->image_data+i)->graylevel > b && (image->image_data+i)->graylevel <= 255){
			(outimage->image_data+i)->rgbRed	 = 255;
			(outimage->image_data+i)->rgbGreen	 = 255;
			(outimage->image_data+i)->rgbBlue	 = 255;
			(outimage->image_data+i)->graylevel	 = 255;
		}
	}
}