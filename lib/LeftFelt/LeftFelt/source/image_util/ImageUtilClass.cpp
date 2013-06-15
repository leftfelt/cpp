#include <image_util/ImageUtilClass.h>

/**
 * ぼやけ度を取得する
 * param biImage image
 * return double
 */
double ImageUtil::getBulr(biImage &image){
	double num = 0;
	Pixel pixel;
	biImage::for_each(image,[&](int i, int j){
		pixel = image.Get(i,j);
		num += (pixel.Red() + pixel.Blue() + pixel.Green());
	});
	num /= (double)(image.Width() * image.Height());
	return (double)(( 1.0 / num ) * 100);
}

bool ImageUtil::Save(biImage &image, String filename,short bitCount, int dataoffset){
	FILE *op;
	
	BITMAPFILEHEADER file;
	BITMAPINFOHEADER info;

	unsigned char R,G,B;

	op = fopen( filename.c_str(),"wb");
	if(op == NULL ){
		puts("画像が開けません(biWriteImage)");
		return false;
	}

	fseek(op,0,SEEK_SET);
	//ヘッダー作成
	unsigned char *head_data;

	//BITMAPFILEHEADER
	strcpy((char *)&file.bfType,"BM");
	file.bfSize =  0;
	file.bfReserved1 = 0;
	file.bfReserved2 = 0;
	if(bitCount <= 8){
		file.bfOffBits = 1024 + dataoffset;
	}else{
		file.bfOffBits = dataoffset;
	}

	//BITMAPINFOHEADER
	info.biSize = 40;
	info.biWidth = image.Width();
	info.biHeight = image.Height();
	info.biPlanes = 1;
	info.biBitCount = bitCount;
	info.biCompression = BI_RGB;
	info.biSizeImage = 0;
	info.biXPelsPerMeter = 0;
	info.biYPelsPerMeter = 0;
	info.biClrUsed = 0;
	info.biClrImportant = 0;

	head_data = (unsigned char*)malloc(sizeof(unsigned char)*file.bfOffBits); //ヘッダー保存領域を確保

	for(int i = 0 ; i < sizeof(file) ; i++){
		*(head_data+i) = *((char*)(&file)+i);
	}
	for(int i = 0 ; i < sizeof(info) ; i++){
		*(head_data+sizeof(file)+i) = *((char*)(&info)+i);
	}

	//ヘッダー書き込み
	fwrite(head_data,sizeof(unsigned char), file.bfOffBits, op);

	//画素を上下反転
	image.Height(-image.Height());
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			if(bitCount == 8){
				G = image.Get(i,j).Lightness();
				fwrite(	&G	, sizeof (unsigned char),1, op );
			}else{
				B = image.Get(i,j).Blue();
				G = image.Get(i,j).Green();
				R = image.Get(i,j).Red();
				fwrite( &B	, sizeof (unsigned char),1, op );
				fwrite(	&G	, sizeof (unsigned char),1, op );
				fwrite( &R	, sizeof (unsigned char),1, op );
				
				if(bitCount == 32){
					G = 0;
					fwrite( &G, sizeof (unsigned char),1, op );
				}
			}
		}
		//3byteずつだと4の倍数にならないため1行ごとに1byte余分に書き込む
		if(bitCount == 24){
			G = 0;
			for(int k = 0 ; k < image.Width()%4 ; k++){
				fwrite( &G, sizeof (unsigned char),1, op );
			}
		}
	}

	//画素を上下反転
	image.Height(-image.Height());

	fclose(op);

	return true;
}

bool ImageUtil::Load(biImage &image, String filename){
	FILE *fp;

	int width, height;
	short bitCount ;
	int data_ofset;
	unsigned char R,G,B;
	
	fp = fopen( filename.c_str(),"rb");
	if(fp == NULL ){
		throw new String("画像が開けません(biLoadImage)");
		return false;
	}

	//初期化
	image.Delete();

	//ヘッダー部分読み取り
	fseek(fp,sizeof(unsigned char)*10,SEEK_CUR);
	fread( &data_ofset, sizeof (unsigned int),1, fp );//データオフセット(ヘッダサイズ)
	fseek(fp,sizeof(unsigned char)*4,SEEK_CUR);
	fread( &width, sizeof(long),1,fp );//&幅
	fread( &height, sizeof(long),1,fp);//高さ
	fseek(fp,sizeof(unsigned char)*2,SEEK_CUR);
	fread( &bitCount, sizeof(unsigned short),1,fp);//ビットの深さ

	fseek(fp,data_ofset,SEEK_SET);

	image.Create(width, height);

	//画像読み込み
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			if(bitCount == 8){
				fread( &G	, sizeof(unsigned char), 1, fp );
				image.Put(i,j,Pixel(G));
			}else{
				fread( &B	, sizeof(unsigned char), 1, fp );
				fread( &G	, sizeof(unsigned char), 1, fp );
				fread( &R	, sizeof(unsigned char), 1, fp );

				image.Put(i,j,Pixel(R,G,B));

				if(bitCount == 32)fread( &G, sizeof (unsigned char), 1, fp );
			}
		}
		//3byteずつだと4の倍数にならないため1行ごとに1byte余分に読み込む
		if(bitCount == 24){
			for(int k = 0 ; k < image.Width()%4 ; k++){
				fread( &G, sizeof (unsigned char), 1, fp );
			}
		}
	}

	//画素を上下反転
	image.Height(-image.Height());

	fclose(fp);

	return true;
}

void ImageUtil::Shrink(biImage &image){
	int cnt;
	biImage output(image.Width(),image.Height());

	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			cnt = 0;
			//黒だったらcontinue
			if(image.Get(i,j).Lightness() == 0){
				output.Put(i,j,image.Get(i,j));
				continue;
			}//*/
		//*
			//近傍値を取得
			for(int k = -1 ; k <= 1  ; k++){
				for(int l = -1 ; l <= 1 ; l++){
					if(i+l<0 || j+k<0 || i+l>=image.Width() || j+k>=image.Height())continue;
					if(k != 1 || l != 1){
						if(image.Get(i+l, j+k).Lightness() == 0){
							cnt++;
						}
					}
				}
			}//*/

			if(cnt != 0){
				output.Put(i,j,Pixel(0));
			}else{
				output.Put(i,j,image.Get(i,j));
			}
		}
	}

	image = output;
	output.Delete();
}
void ImageUtil::Expand(biImage &image){
	int cnt;
	biImage output(image.Width(),image.Height());

	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			cnt = 0;
			//白だったらcontinue
			if(image.Get(i,j).Lightness() == 255){
				output.Put(i,j,image.Get(i,j));
				continue;
			}//*/
		//*
			//近傍値を取得
			for(int k = -1 ; k <= 1  ; k++){
				for(int l = -1 ; l <= 1 ; l++){
					if(i+l<0 || j+k<0 || i+l>=image.Width() || j+k>=image.Height())continue;
					if(k != 1 || l != 1){
						if(image.Get(i+l, j+k).Lightness() == 255){
							cnt++;
						}
					}
				}
			}//*/

			if(cnt != 0){
				output.Put(i,j,Pixel(255));
			}else{
				output.Put(i,j,image.Get(i,j));
			}
		}
	}

	image = output;
	output.Delete();
}
//オープニング（白のノイズを除去する）
void ImageUtil::Opening(biImage &image,int level){
	//白（255）の画素を小さくする（収縮）
	for(int i = 0 ; i < level ; i++){
		Shrink(image);
	}

	//白（255）の画素を大きくする（膨張）
	for(int i = 0 ; i < level ; i++){
		Expand(image);
	}
}

//クロージング（黒のノイズを除去する）
void ImageUtil::Closing(biImage &image,int level){
	//白（255）の画素を大きくする（膨張）
	for(int i = 0 ; i < level ; i++){
		Expand(image);
	}

	//白（255）の画素を小さくする（収縮）
	for(int i = 0 ; i < level ; i++){
		Shrink(image);
	}
}

//トップハット
void ImageUtil::TopHat(biImage &image,int level){
	biImage temp;
	temp = image;
	Opening(temp,level);
	image = image - temp;

}
//ブラックハット
void ImageUtil::BlackHat(biImage &image,int level){
	biImage temp;
	temp = image;
	Closing(temp,level);
	image = image - temp;
}
//細線化
int ImageUtil::Thinning(biImage &image){

	biImage temp;
	//-1はワイルドカード*と同じ役割
	int mask[][9] = {
		{ 0, 0,-1,
		  0, 1, 1,
		 -1, 1,-1},//M1
		{ 0, 0, 0,
		 -1, 1,-1,
		  1, 1,-1},//M2
		{-1, 0, 0,
		  1, 1, 0,
		 -1, 1,-1},//M3
		{ 1,-1, 0,
		  1, 1, 0,
		 -1,-1, 0},//M4
		{-1, 1,-1,
		  1, 1, 0,
		 -1, 0, 0},//M5
		{-1, 1, 1,
		 -1, 1,-1,
		  0, 0, 0},//M6
		{-1, 1,-1,
		  0, 1, 1,
		  0, 0,-1},//M7
		{ 0,-1,-1,
		  0, 1, 1,
		  0,-1, 1},//M8
		{-2},//終了の判定に使うので消さない
	};

	//消去された画素数（これが0を返すまで繰り返す）
	int count = 0;
	temp = image;
	bool continue_flag = true;
	//マスクのどれかに一致する画素を消去する（この処理を消去する画素がなくなるまで繰り返す）
	int mask_num;
	while(continue_flag){
		continue_flag = false;
		mask_num = 0;
		while(mask[mask_num][0] != -2){
			for(int j=1 ; j<image.Height()-1 ; j++){
				for(int i=1 ; i<image.Width()-1 ; i++){
					if(image.Get(i,j) == Pixel(0)) continue;
					int k;
					for(k=0 ; k<9 ; k++){
						//-1はワイルドカード*と同じ役割
						if(mask[mask_num][k] != -1){
							int pixel = mask[mask_num][k] * 255;
							//マスクの値とひとつでも違ったらループをとめる
							if(image.Get(i+(k%3)-1,j+(k/3)-1) != Pixel(pixel))break;
						}
					}
					//kのループが最後まで回ったらmask_num番目のmaskと一致している
					if(k == 9){
						//注目画素を消去（0にする）
						temp.Put(i,j,Pixel(0));
						count++;
						continue_flag = true;
					}
				}
			}
			image = temp;
			mask_num++;
		}
	}
	return count;
}

//鮮鋭化
void ImageUtil::UnSharpMasking(biImage &image,int level){
	biImage temp;
	temp = image;
	LowResolution(temp,level);
	image = image + (image - temp)*2;
}

//低解像度化(ぼかし)
void ImageUtil::LowResolution(biImage &image,int level){
	//注目画素を-level～levelの範囲の平均値で置き換える
	long R=0,G=0,B=0;
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			R=0;
			G=0;
			B=0;
			for(int k=-level ; k<=level ; k++){
				for(int l=-level ; l<=level ; l++){
					Pixel pixel = image.Get(i+l,j+k);
					R += pixel.Red();
					G += pixel.Green();
					B += pixel.Blue();
				}
			}
			image.Put(i,j,Pixel(
				(unsigned char)(R/pow((level*2+1)*1.0,2)),
				(unsigned char)(G/pow((level*2+1)*1.0,2)),
				(unsigned char)(B/pow((level*2+1)*1.0,2))
			));
		}
	}
}

//色領域分割
void ImageUtil::colorRegionSplit(biImage &image,int threshold){
	int x,y;
	int label_num = 0;
	int unattended_num = 0;
	std::map<unsigned long,unsigned long> label_disp; //ラベル画面
	std::map<unsigned long,Pixel> tab; //label_num番目のPixel
	std::map<unsigned long,unsigned long> label_tab_num;
	std::map<unsigned long,int> unlabel_x;
	std::map<unsigned long,int> unlabel_y;

	for(y = 0 ; y < image.Height() ; y++){
		for(x = 0 ; x < image.Width() ; x++){
			if(label_disp[x+y*image.Width()] > 0) continue;
			label_num++;
			label_disp[x+y*image.Width()] = label_num;
			tab[label_num] = image.Get(x,y);
			unattended_num = 1;
			unlabel_x[unattended_num]=x; //未処理画素の座標
			unlabel_y[unattended_num]=y;
			while(unattended_num > 0){
				int mx = unlabel_x[unattended_num];
				int my = unlabel_y[unattended_num];
				unattended_num--;
				for(int i=0 ; i<9 ; i++){
					int cx = (i%3)-1;
					int cy = (i/3)-1;
					if(0<=mx+cx && mx+cx<image.Width() &&
							0<=my+cy && my+cy<image.Height()){
						if(label_disp[mx+cx+(my+cy)*image.Width()] == 0){
							Pixel pixel = image.Get(mx+cx,my+cy);
							int diff =  abs(tab[label_num].Red()-pixel.Red()) +
										abs(tab[label_num].Green()-pixel.Green()) +
										abs(tab[label_num].Blue()-pixel.Blue());
							if(diff > threshold) continue;
							label_disp[mx+cx+(my+cy)*image.Width()] = label_num;
							label_tab_num[label_num]++;
							unattended_num++;
							unlabel_x[unattended_num] = mx+cx;
							unlabel_y[unattended_num] = my+cy;
						}
					}
				}
			}
		}
	}
	int seg_num = label_num; //領域数
	for(y = 0 ; y < image.Height() ; y++){
		for(x = 0 ; x < image.Width() ; x++){
			label_num = label_disp[x+y*image.Width()];//ラベル番号を取り出す
			Pixel pixel = tab[label_num];
			image.Put(x,y,pixel);
		}
	}
}

void ImageUtil::getHistogram(biImage &image, int *histogram){
	int i;
	for(i = 0; i < 256 ; i++){
		*(histogram+i)=0;
	}
	for(int j = 0 ; j<image.Height() ; j++){
		for(int i = 0; i < image.Width() ; i++){
			(*(histogram+image.Get(i,j).Lightness()))++;
		}
	}
}

//判別分析法による閾値取得
int ImageUtil::getThreshold(biImage &image){
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

	getHistogram(image,histogram);
	
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
	u0 = sum / (image.Width()*image.Height());

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
//二値化
int ImageUtil::Binarize(biImage &image, int threshold){
	int ret = 0;
	
	if(threshold == -1){
		ret = getThreshold(image);
		threshold = ret;
	}
	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			if(image.Get(i,j).Lightness() >= threshold){
				image.Put(i,j,Pixel(255));
			}else{
				image.Put(i,j,Pixel(0));
			}
		}
	}

	return ret;

}
//傾きをかける
void ImageUtil::Incline(biImage &image, double R,double G,double B){
	
	//傾斜
	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			image.Put(
				i,j,
				Pixel(
					(int)(R * image.Get(i,j).Red()),
					(int)(G * image.Get(i,j).Green()),
					(int)(B * image.Get(i,j).Blue())
				)
			);
		}
	}
}

//ゾーベルフィルタ
void ImageUtil::Sobel(biImage &image){
	int width, height, graylevel;
	int pos = 0;
	int num[9];//近傍領域
	int dx,dy,gray;
	biImage outimage;

	outimage = image;
	width = image.Width();
	height = image.Height();
	graylevel = 255;

	//カラー画像を二値化
	//auto_niti(&outimage);
	
	//Sobelフィルター
	for(int j = 0 ; j < height ; j++){
		for(int i = 0 ; i < width ; i++){
			pos = 0;
			//近傍領域の値を取得
			for(int l = -1 ; l <= 1 ; l++){//k
				for(int k = -1; k <= 1 ; k++){//j
					pos = i+(j+l)*width;
					if((pos/width) == 0 || (pos/width) >= height-1 || (pos%width) == 0 || (pos-(width-1))%width == 0){
						num[(k+1)+(l+1)*3] = 0;
					}else{
						num[(k+1)+(l+1)*3] = outimage.Get(i,j+l).Lightness();
					}
				}
			}
			
			dx = num[0] + 2*num[3] + num[6];
			dx = dx - num[2] - 2*num[5] - num[8];
			dy = num[0] + 2*num[1] + num[2];
			dy = dy - num[6] - 2*num[7] - num[8];

			gray = abs(dx)+abs(dy);

			if(gray > graylevel) gray = graylevel;
			image.Put(i,j,Pixel(gray));
		}
	}
}

// ラプラシアンフィルタ
void ImageUtil::Laplacian(biImage &image){
	Pixel pixel;
	biImage temp;
	int mask[9] = {0,1,0,1,-4,1,0,1,0};
	temp = image;
	for(int j = 1 ; j < image.Height()-1 ; j++){
		for(int i = 1 ; i < image.Width()-1 ; i++){
			pixel.setRGB(0,0,0);
			for(int k = 0 ; k < 9 ; k++){
				pixel += image.Get(i+(k%3-1),j+(k/3-1)) * mask[k];
			}
			temp.Put(i,j,pixel);
		}
	}
	image = temp;
}


//ノイズ除去
void ImageUtil::AntiNoise(biImage &image,unsigned int level){
	Opening(image,level);
}

//コントラスト改善
void ImageUtil::Contrast(biImage &image){
	int histogram[256];
	int a = 0,b = 255;

	getHistogram(image,histogram);

	//コントラストを変える
	for(int i = 0; i <= 255 ; i++){
		if(histogram[i] != 0){
			a = i;
			break;
		}
	}

	for(int i = 255; i >= 0 ; i--){
		if(histogram[i] != 0){
			b = i;
			break;
		}
	}

	//コントラストの改善
	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			if( image.Get(i,j).Lightness() >= 0 && image.Get(i,j).Lightness() < a){
				image.Put(i,j,Pixel(0));
			}else if(image.Get(i,j).Lightness() >=a && image.Get(i,j).Lightness() <= b){
				image.Put(i,j,Pixel( (int)(255 * ((image.Get(i,j).Lightness() - a) / (double)(b - a))) ));
			}else if(image.Get(i,j).Lightness() > b && image.Get(i,j).Lightness() <= 255){
				image.Put(i,j,Pixel(255));
			}
		}
	}
}

//肌色抽出
void ImageUtil::YIQRange(biImage &image, int Ylow, int Yhigh,int Ilow, int Ihigh, int Qlow, int Qhigh){
	int yiq_I,yiq_Q,yiq_Y;

	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			yiq_Y = image.Get(i,j).Y();
			yiq_I = image.Get(i,j).I();
			yiq_Q = image.Get(i,j).Q();
			if( (Ylow <= yiq_Y && yiq_Y <= Yhigh) &&
				((Ilow <= yiq_I && yiq_I <= Ihigh) || (Ilow == Ihigh)) && 
				((Qlow <= yiq_Q && yiq_Q <= Qhigh) || (Qlow == Qhigh)) 
				){ 
					image.Put(i,j,Pixel(255));
			}else{
					image.Put(i,j,Pixel(0));
			}
		}
	}
}

//HSV範囲指定
void ImageUtil::HSVRange(biImage &image, int Hlow, int Hhigh,int Slow, int Shigh, int Vlow, int Vhigh){
	ImageUtil::HRange(image,Hlow,Hhigh);
	ImageUtil::SRange(image,Slow,Shigh);
	ImageUtil::VRange(image,Vlow,Vhigh);
}

//色相が指定範囲の画素を抽出
void ImageUtil::HRange(biImage &image, int low, int high, bool in_range){
	Pixel pixel;
	low		= Math::limit(low	,0,360);
	high	= Math::limit(high	,0,360);
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			pixel = image.Get(i,j);
			if( !((low <= pixel.H() && pixel.H() <= high) == in_range) ) {
				image.Put(i,j,Pixel(0));
			}
		}
	}
}
//彩度が指定範囲の画素を抽出
void ImageUtil::SRange(biImage &image, int low, int high, bool in_range){
	Pixel pixel;
	low		= Math::limit(low	,0,100);
	high	= Math::limit(high	,0,100);
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			pixel = image.Get(i,j);
			if( !((low <= pixel.S() && pixel.S() <= high) == in_range) ) {
				image.Put(i,j,Pixel(0));
			}
		}
	}
}
//明度が指定範囲の画素を抽出
void ImageUtil::VRange(biImage &image, int low, int high, bool in_range){
	Pixel pixel;
	low		= Math::limit(low	,0,100);
	high	= Math::limit(high	,0,100);
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			pixel = image.Get(i,j);
			if( !((low <= pixel.V() && pixel.V() <= high) == in_range) ) {
				image.Put(i,j,Pixel(0));
			}
		}
	}
}

void ImageUtil::GammaCorrection(biImage &image, double gamma){
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			Pixel pixel = image.Get(i,j);
			image.Put(
				i,j,Pixel(
					(unsigned char)(255*pow((pixel.Red()/255.0),1/gamma)),
					(unsigned char)(255*pow((pixel.Green()/255.0),1/gamma)),
					(unsigned char)(255*pow((pixel.Blue()/255.0),1/gamma))
				));
		}
	}
}

//明るさ調整
void ImageUtil::Brightness(biImage &image, int threshold){
	int average = 0;
	int count = 0;
	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			Pixel pixel = image.Get(i,j);
			if(pixel.Lightness() != 255){
				average += pixel.Lightness();
				count++;
			}
		}
	}
	average /= count;
	image += (threshold - average);
}

//有効範囲を取得する(255がある範囲)
Area ImageUtil::Scope(biImage &image){
	Area area;
	int top=image.Height();
	int bottom=0;
	int left=image.Width();
	int right=0;
	for(int j = 0 ; j < image.Height() ; j++){
		for(int i = 0 ; i < image.Width() ; i++){
			if(image.Get(i,j) == Pixel(255)){				
				if(top > j) top = j;
				if(bottom < j)bottom = j;
				if(left > i) left = i;
				if(right < i)right = i;
			}
		}
	}
	area.Pos(left,top);
	area.Size(right-left,bottom-top);
	return area;
}
//テンプレートマッチング
//テンプレートが大きいとかなり遅い
Point ImageUtil::TemplateMatching(biImage &image, biImage temp, int threshold){
	int i,j,k,l;
	Point pt(0,0);
	int max = 255 * 3 *temp.Width() * temp.Height();
	int min = max;
	int valid_num = 0;
	int count = 0;

	//mimageよりtempのほうが大きいときは終了
	if(image.Width() < temp.Width() || 
		image.Height() < temp.Height())return pt;

	for(j = -temp.Height() ; j < image.Height() ; j++){
		for(i = -temp.Width() ; i < image.Width() ; i++){
			count = 0;
			valid_num = 0;
			for(l = 0 ; l < temp.Height(); l++){
				for(k = 0 ; k < temp.Width() ; k++){
					//範囲外・透明色の部分は完全に一致したものとする
					if( temp.Get(k,l) == temp.Clear()) {
						count += 255*3;
						continue;
					}
					if((i+k) >= image.Width() || (j+l) >= image.Height() || (i+k) < 0 || (j+l) < 0){
						count += 255*3;
						continue;
					}
					valid_num++;
					count += abs( image.Get((i+k),(j+l)).Blue()		- temp.Get(k,l).Blue());
					count += abs( image.Get((i+k),(j+l)).Green()	- temp.Get(k,l).Green());
					count += abs( image.Get((i+k),(j+l)).Red()		- temp.Get(k,l).Red());
				}
			}
			max = 255*3*valid_num;
			//最も類似度が高く閾値を超えている位置を選択する
			if(min > count && (threshold <= (max-count)/(double)max * 100 || threshold == -1) ){
				min = count;
				pt.Pos(i,j);
			}
		}
	}

	return pt;
}

//テンプレートマッチング
Point ImageUtil::TemplateMatching(biImage &image, int x, int y, int width, int height, biImage temp, int threshold){
	int i,j;
	Point pt(x,y);
	int max = 0;
	int count = 0;

	//mimageよりtempのほうが大きいときは終了
	if(image.Width() < temp.Width() || 
		image.Height() < temp.Height())return pt;

	for(j = y-height/2 ; j <= y+height/2 ; j++){
		for(i = x-width/2 ; i <= x+width/2 ; i++){
			count = 0;
			if(i >= image.Width() || j >= image.Height() || i < 0 || j < 0)continue;
			count = Matching(image,i-temp.Width()/2,j-temp.Height()/2,temp);
			//最も類似度が高く閾値を超えている位置を選択する
			if(max < count && (threshold <= count || threshold == -1) ){ 
				max = count;
				pt.Pos(i,j);
			}
		}
	}

	return pt;
}

//テンプレートマッチング範囲限定
int ImageUtil::Matching(biImage &image, int x,int y, biImage temp){
	int i,j;
	int max = 255 * 3 *temp.Width() * temp.Height();
	int count = 0;
	int valid_num = 0;

	//mimageよりtempのほうが大きいときは終了
	if(image.Width() < temp.Width() || 
		image.Height() < temp.Height())return 0;

	for(j = 0 ; j < temp.Height(); j++){
		for(i = 0 ; i < temp.Width() ; i++){
			//範囲外・透明色の部分は完全に一致したものとする
			if( temp.Get(i,j) == temp.Clear()){
				//count += 255*3;
				continue;
			}
			if((i+x) >= image.Width() || (j+y) >= image.Height() || (i+x) < 0 || (j+y) < 0){
				//画面外に出ると極端に一致率が下がる
				count += 255*3;
				continue;
			}
			valid_num++;
			count += abs( image.Get((i+x),(j+y)).Blue()  - temp.Get(i,j).Blue());
			count += abs( image.Get((i+x),(j+y)).Green() - temp.Get(i,j).Green());
			count += abs( image.Get((i+x),(j+y)).Red()   - temp.Get(i,j).Red());
		}
	}
	max = 255*3*valid_num;
	return (int)((max-count)/(double)max * 100);
}

//端点リストを取得する
PointList ImageUtil::getNodeList(biImage &image,int connected_num){
	PointList point_list;
	Point point;
	for(int j = 0 ; j < image.Height(); j++){
		for(int i = 0 ; i < image.Width() ; i++){
			if(image.Get(i,j) == Pixel(0)) continue;
			if(ImageUtil::getConnectedNum(image,i,j) == connected_num){
				point.Pos(i,j);
				point_list.push_back(point);
			}
		}
	}
	return point_list;
}

//周囲8マスにあるPixel(255)の数を取得
int ImageUtil::getConnectedNum(biImage &image,int x,int y){
	int count = 0;
	for(int j = -1 ; j <= 1; j++){
		for(int i = -1 ; i <= 1 ; i++){
			if(i==0&&j==0)continue;
			if(image.Get(x+i,y+j) == Pixel(0)) continue;
			count++;
		}
	}
	return count;
}

void ImageUtil::toCvImage(IplImage **cv_image, biImage &image){
	*cv_image = cvCreateImage(
		cvSize(image.Width(), image.Height()),
		IPL_DEPTH_8U,
		3
	);

	biImage::for_each(image,[&](int x, int y){
		Pixel pixel = image.Get(x,y);
		(*cv_image)->imageData[x*3+0+y*((*cv_image)->widthStep)] = pixel.Blue();
		(*cv_image)->imageData[x*3+1+y*((*cv_image)->widthStep)] = pixel.Green();
		(*cv_image)->imageData[x*3+2+y*((*cv_image)->widthStep)] = pixel.Red();
	});
}

void ImageUtil::fromCvImage(biImage &image, IplImage *cv_image){
	if(cv_image->depth == IPL_DEPTH_8U && cv_image->nChannels == 3 && cv_image->imageData != NULL){
		for(int y = 0 ; y < cv_image->height ; y++){
			for(int x = 0 ; x < cv_image->width ; x++){
				image.Put(x,y,Pixel(
					(*(cv_image->imageData+x*3+2+y*cv_image->widthStep)),
					(*(cv_image->imageData+x*3+1+y*cv_image->widthStep)),
					(*(cv_image->imageData+x*3+0+y*cv_image->widthStep))
					));
			}
		}
	}else{
		throw std::exception("can't to biImage");
	}
}


std::vector<SurfFeature> ImageUtil::getSurf(biImage &image, double threshold){

	IplImage *ipl_image = NULL;
	ImageUtil::toCvImage(&ipl_image, image);
	cv::Mat mat(ipl_image), grayImage;
	cvSaveImage("hoge.bmp",ipl_image);
	
	//toGray
	cv::cvtColor(mat,grayImage,CV_BGR2GRAY);

	cv::SURF calc_surf = cv::SURF(threshold,4,2,true);

	std::vector<cv::KeyPoint> kp_vec;
	std::vector<float> desc_vec;
	int dimension_num = calc_surf.descriptorSize();
	calc_surf(grayImage, cv::Mat(), kp_vec, desc_vec);

	std::vector<SurfFeature> result;
	//*
	for(int i = 0 ; i < (signed)kp_vec.size() ; i++){
		SurfFeature surf;
		surf.point = kp_vec.at(i);
		for(int j = 0 ; j < dimension_num ; j++){
			surf.features.push_back(desc_vec.at(i*dimension_num+j));
		}
		result.push_back(surf);
	}
	//*/
	cvReleaseImage(&ipl_image);
	return result;
}

//*
//ラベリング
int ImageUtil::Labeling(biImage &image){
	int w,h,j,k,i,size;
	int label_count=1,flag_count;
	int min_data[8],min;
	int *Lookup_table,*sub_table,*label_list;
	int max=0,label=0,Lookup_flag=0;
	
	Lookup_table = new int[image.Width() * image.Height()];
	sub_table = new int[image.Width() * image.Height()];
	label_list = new int[image.Width() * image.Height()];

	//初期化
	size = image.Width() * image.Height();
	for(i = 0 ;i< size;i++){
		*(label_list+i) = 0;
		*(sub_table+i) = 0;
	}
	for(i=0;i<size;i++){
		Lookup_table[i] = i;
	}

	//ラベリング
	//*
	for(k=0;k<2;k++){
		for(h = 1; h < image.Height()-1;h++){
			for(w=1;w<image.Width()-1;w++){
				if(image.Get(w,h).Lightness() == 255){
					flag_count = 0;
					for(int i=0 ; i<9 ; i++){
						if( (i%3-1) == 0 && (i/3-1) == 0 ) continue;
						if( *(label_list+(w+i%3-1)+(h+i/3-1)*image.Width()) == 0 ) flag_count++;
					}
					
					if(flag_count == 8){//周りがすべて0だった
						//■■■
					    //■□■
						//■■■
						*(label_list+w+h*image.Width()) = label_count;//ラベルを振る
						label_count++;
					}else{//一か所でも255があった
						//１■４
					    //■□■
						//■■■
						for(int i=0,count=0 ; i<9 ; i++){
							if( (i%3-1) == 0 && (i/3-1) == 0 ) continue;
							min_data[count] = *(label_list+(w+i%3-1)+(h+i/3-1)*image.Width());
							count++;
						}
	
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
											
						*(label_list+w+h*image.Width()) = min;
					}
				}
			}
		}
		
		for(h=0;h<label_count;h++){
			if(h != Lookup_table[h]){
				Lookup_table[h] = Lookup_update(h,Lookup_table);
			}
		}

			
		for(h = 0 ; h < image.Height() ; h++){
			for(w = 0 ; w < image.Width() ; w++){
				if(*(label_list+w+h*image.Width())  != 0){
					*(label_list+w+h*image.Width())  = Lookup_table[*(label_list+w+h*image.Width())];
				}
			}
		}
	}

	
	i=0;
	sub_table = (int *)memset(sub_table,-1,sizeof(int) * image.Width() * image.Height());
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

	Pixel pixel;
	//ラベルの割り振り
	for(h = 0 ; h < image.Height() ; h++){
		for(w = 0 ; w < image.Width() ; w++){
			if(*(label_list+w+h*image.Width()) != 0){
				pixel = image.Get(w,h);
				pixel.Label(Lookup_table[*(label_list+w+h*image.Width())]);
				image.Put(w,h,pixel);
			}
		}
	}
	
	free(Lookup_table);
	free(sub_table);

	return label;
}

//ラベル番号からオブジェクトを抽出する。ついでに面積も返す。
int ImageUtil::SamplingObject(biImage &image, int label){
	Pixel pixel;
	int count=0;
	for(int j=0 ; j<image.Height() ; j++){
		for(int i=0 ; i<image.Width() ; i++){
			pixel = image.Get(i,j);
			if((pixel.Label() == label)){
				pixel.Lightness(255);
				image.Put(i,j,pixel);
				count++;
			}else{
				pixel.Lightness(0);
				image.Put(i,j,pixel);
			}
		}
	}

	return count;
}

//ラベリング済み画像から面積が最大のオブジェクトを抽出する
int ImageUtil::SamplingLargeObject(biImage &image){
	std::map<int,int> area_map;
	std::map<int,int>::iterator area;
	//面積を取得
	for(int j=0 ; j < image.Height() ; j++){
		for(int i=0 ; i < image.Width() ; i++){
			int id = image.Get(i,j).Label();
			if(id != 0){//0はオブジェクトではないので
				area_map[id]++;
			}
		}
	}
	//面積が大きいのを抽出
	int area_id, num,max=0;
	for(area = area_map.begin(); area != area_map.end() ; ++area){
 		num = area->second;//面積
		if(max < num){
			max = num;
			area_id = area->first;//ID
		}
	}
	ImageUtil::SamplingObject(image,area_id);
	return area_id;
}

//===============================================================================================

int ImageUtil::min_calc(int *p)
{
	int i,min=0xFFFF;

	for(i=0;i<8;i++){
		if( *(p+i)!=0 ){
				if(min > *(p+i)){
				min = *(p+i);
			}
		}
	}

	return min;
}

int ImageUtil::Lookup_update(int i,int * label)
{
	int a;

	if(i == label[i]){
		return label[i];
	}else{
		a =  Lookup_update(label[i],label);
	}

	return a;
}
