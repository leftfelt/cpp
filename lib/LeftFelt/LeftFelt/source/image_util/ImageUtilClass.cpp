#include <image_util/ImageUtilClass.h>

/**
 * �ڂ₯�x���擾����
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
		puts("�摜���J���܂���(biWriteImage)");
		return false;
	}

	fseek(op,0,SEEK_SET);
	//�w�b�_�[�쐬
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

	head_data = (unsigned char*)malloc(sizeof(unsigned char)*file.bfOffBits); //�w�b�_�[�ۑ��̈���m��

	for(int i = 0 ; i < sizeof(file) ; i++){
		*(head_data+i) = *((char*)(&file)+i);
	}
	for(int i = 0 ; i < sizeof(info) ; i++){
		*(head_data+sizeof(file)+i) = *((char*)(&info)+i);
	}

	//�w�b�_�[��������
	fwrite(head_data,sizeof(unsigned char), file.bfOffBits, op);

	//��f���㉺���]
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
		//3byte������4�̔{���ɂȂ�Ȃ�����1�s���Ƃ�1byte�]���ɏ�������
		if(bitCount == 24){
			G = 0;
			for(int k = 0 ; k < image.Width()%4 ; k++){
				fwrite( &G, sizeof (unsigned char),1, op );
			}
		}
	}

	//��f���㉺���]
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
		throw new String("�摜���J���܂���(biLoadImage)");
		return false;
	}

	//������
	image.Delete();

	//�w�b�_�[�����ǂݎ��
	fseek(fp,sizeof(unsigned char)*10,SEEK_CUR);
	fread( &data_ofset, sizeof (unsigned int),1, fp );//�f�[�^�I�t�Z�b�g(�w�b�_�T�C�Y)
	fseek(fp,sizeof(unsigned char)*4,SEEK_CUR);
	fread( &width, sizeof(long),1,fp );//&��
	fread( &height, sizeof(long),1,fp);//����
	fseek(fp,sizeof(unsigned char)*2,SEEK_CUR);
	fread( &bitCount, sizeof(unsigned short),1,fp);//�r�b�g�̐[��

	fseek(fp,data_ofset,SEEK_SET);

	image.Create(width, height);

	//�摜�ǂݍ���
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
		//3byte������4�̔{���ɂȂ�Ȃ�����1�s���Ƃ�1byte�]���ɓǂݍ���
		if(bitCount == 24){
			for(int k = 0 ; k < image.Width()%4 ; k++){
				fread( &G, sizeof (unsigned char), 1, fp );
			}
		}
	}

	//��f���㉺���]
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
			//����������continue
			if(image.Get(i,j).Lightness() == 0){
				output.Put(i,j,image.Get(i,j));
				continue;
			}//*/
		//*
			//�ߖT�l���擾
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
			//����������continue
			if(image.Get(i,j).Lightness() == 255){
				output.Put(i,j,image.Get(i,j));
				continue;
			}//*/
		//*
			//�ߖT�l���擾
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
//�I�[�v�j���O�i���̃m�C�Y����������j
void ImageUtil::Opening(biImage &image,int level){
	//���i255�j�̉�f������������i���k�j
	for(int i = 0 ; i < level ; i++){
		Shrink(image);
	}

	//���i255�j�̉�f��傫������i�c���j
	for(int i = 0 ; i < level ; i++){
		Expand(image);
	}
}

//�N���[�W���O�i���̃m�C�Y����������j
void ImageUtil::Closing(biImage &image,int level){
	//���i255�j�̉�f��傫������i�c���j
	for(int i = 0 ; i < level ; i++){
		Expand(image);
	}

	//���i255�j�̉�f������������i���k�j
	for(int i = 0 ; i < level ; i++){
		Shrink(image);
	}
}

//�g�b�v�n�b�g
void ImageUtil::TopHat(biImage &image,int level){
	biImage temp;
	temp = image;
	Opening(temp,level);
	image = image - temp;

}
//�u���b�N�n�b�g
void ImageUtil::BlackHat(biImage &image,int level){
	biImage temp;
	temp = image;
	Closing(temp,level);
	image = image - temp;
}
//�א���
int ImageUtil::Thinning(biImage &image){

	biImage temp;
	//-1�̓��C���h�J�[�h*�Ɠ�������
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
		{-2},//�I���̔���Ɏg���̂ŏ����Ȃ�
	};

	//�������ꂽ��f���i���ꂪ0��Ԃ��܂ŌJ��Ԃ��j
	int count = 0;
	temp = image;
	bool continue_flag = true;
	//�}�X�N�̂ǂꂩ�Ɉ�v�����f����������i���̏��������������f���Ȃ��Ȃ�܂ŌJ��Ԃ��j
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
						//-1�̓��C���h�J�[�h*�Ɠ�������
						if(mask[mask_num][k] != -1){
							int pixel = mask[mask_num][k] * 255;
							//�}�X�N�̒l�ƂЂƂł�������烋�[�v���Ƃ߂�
							if(image.Get(i+(k%3)-1,j+(k/3)-1) != Pixel(pixel))break;
						}
					}
					//k�̃��[�v���Ō�܂ŉ������mask_num�Ԗڂ�mask�ƈ�v���Ă���
					if(k == 9){
						//���ډ�f�������i0�ɂ���j
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

//�N�s��
void ImageUtil::UnSharpMasking(biImage &image,int level){
	biImage temp;
	temp = image;
	LowResolution(temp,level);
	image = image + (image - temp)*2;
}

//��𑜓x��(�ڂ���)
void ImageUtil::LowResolution(biImage &image,int level){
	//���ډ�f��-level�`level�͈̔͂̕��ϒl�Œu��������
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

//�F�̈敪��
void ImageUtil::colorRegionSplit(biImage &image,int threshold){
	int x,y;
	int label_num = 0;
	int unattended_num = 0;
	std::map<unsigned long,unsigned long> label_disp; //���x�����
	std::map<unsigned long,Pixel> tab; //label_num�Ԗڂ�Pixel
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
			unlabel_x[unattended_num]=x; //��������f�̍��W
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
	int seg_num = label_num; //�̈搔
	for(y = 0 ; y < image.Height() ; y++){
		for(x = 0 ; x < image.Width() ; x++){
			label_num = label_disp[x+y*image.Width()];//���x���ԍ������o��
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

//���ʕ��͖@�ɂ��臒l�擾
int ImageUtil::getThreshold(biImage &image){
	int histogram[256];
	int i,j;
	int a,b;//�����E���
	int sum = 0;//���v
	int n1 = 0,n2 = 0;//��f��
	double u0,u1,u2;//����
	double O12 = 0,O22 = 0;//���U
	double OW2;//�N���X�����U
	double OB2;//�N���X�ԕ��U
	int t = 0;//臒l
	int threshold = 0;//����臒l
	double proportion = -1;//�䗦

	getHistogram(image,histogram);
	
	//�����擾
	for(i = 0; i <= 255 ; i++){
		if((*(histogram+i)) != 0){
			a = i;
			break;
		}
	}
	//����擾
	for(i = 255; i >= 0 ; i--){
		if((*(histogram+i)) != 0){
			b = i;
			break;
		}
	}
	//���v
	for(i = 0 ; i <= 255 ; i++){
		sum += i*(*(histogram+i));
	}

	//���ϒl
	u0 = sum / (image.Width()*image.Height());

	for(t = a ; t <= b ; t++){
		//���v
		sum = 0;
		n1 = 0;
		for(i = a ; i <= t ; i++){
			sum += i*(*(histogram+i));
			n1 += *(histogram+i);
		}

		u1 = sum / n1; //���ϒl
	
		//���U
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

		u2 = sum / n2; //���ϒl

		O22 = 0;
		//���U
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
//��l��
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
//�X����������
void ImageUtil::Incline(biImage &image, double R,double G,double B){
	
	//�X��
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

//�]�[�x���t�B���^
void ImageUtil::Sobel(biImage &image){
	int width, height, graylevel;
	int pos = 0;
	int num[9];//�ߖT�̈�
	int dx,dy,gray;
	biImage outimage;

	outimage = image;
	width = image.Width();
	height = image.Height();
	graylevel = 255;

	//�J���[�摜���l��
	//auto_niti(&outimage);
	
	//Sobel�t�B���^�[
	for(int j = 0 ; j < height ; j++){
		for(int i = 0 ; i < width ; i++){
			pos = 0;
			//�ߖT�̈�̒l���擾
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

// ���v���V�A���t�B���^
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


//�m�C�Y����
void ImageUtil::AntiNoise(biImage &image,unsigned int level){
	Opening(image,level);
}

//�R���g���X�g���P
void ImageUtil::Contrast(biImage &image){
	int histogram[256];
	int a = 0,b = 255;

	getHistogram(image,histogram);

	//�R���g���X�g��ς���
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

	//�R���g���X�g�̉��P
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

//���F���o
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

//HSV�͈͎w��
void ImageUtil::HSVRange(biImage &image, int Hlow, int Hhigh,int Slow, int Shigh, int Vlow, int Vhigh){
	ImageUtil::HRange(image,Hlow,Hhigh);
	ImageUtil::SRange(image,Slow,Shigh);
	ImageUtil::VRange(image,Vlow,Vhigh);
}

//�F�����w��͈͂̉�f�𒊏o
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
//�ʓx���w��͈͂̉�f�𒊏o
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
//���x���w��͈͂̉�f�𒊏o
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

//���邳����
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

//�L���͈͂��擾����(255������͈�)
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
//�e���v���[�g�}�b�`���O
//�e���v���[�g���傫���Ƃ��Ȃ�x��
Point ImageUtil::TemplateMatching(biImage &image, biImage temp, int threshold){
	int i,j,k,l;
	Point pt(0,0);
	int max = 255 * 3 *temp.Width() * temp.Height();
	int min = max;
	int valid_num = 0;
	int count = 0;

	//mimage���temp�̂ق����傫���Ƃ��͏I��
	if(image.Width() < temp.Width() || 
		image.Height() < temp.Height())return pt;

	for(j = -temp.Height() ; j < image.Height() ; j++){
		for(i = -temp.Width() ; i < image.Width() ; i++){
			count = 0;
			valid_num = 0;
			for(l = 0 ; l < temp.Height(); l++){
				for(k = 0 ; k < temp.Width() ; k++){
					//�͈͊O�E�����F�̕����͊��S�Ɉ�v�������̂Ƃ���
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
			//�ł��ގ��x������臒l�𒴂��Ă���ʒu��I������
			if(min > count && (threshold <= (max-count)/(double)max * 100 || threshold == -1) ){
				min = count;
				pt.Pos(i,j);
			}
		}
	}

	return pt;
}

//�e���v���[�g�}�b�`���O
Point ImageUtil::TemplateMatching(biImage &image, int x, int y, int width, int height, biImage temp, int threshold){
	int i,j;
	Point pt(x,y);
	int max = 0;
	int count = 0;

	//mimage���temp�̂ق����傫���Ƃ��͏I��
	if(image.Width() < temp.Width() || 
		image.Height() < temp.Height())return pt;

	for(j = y-height/2 ; j <= y+height/2 ; j++){
		for(i = x-width/2 ; i <= x+width/2 ; i++){
			count = 0;
			if(i >= image.Width() || j >= image.Height() || i < 0 || j < 0)continue;
			count = Matching(image,i-temp.Width()/2,j-temp.Height()/2,temp);
			//�ł��ގ��x������臒l�𒴂��Ă���ʒu��I������
			if(max < count && (threshold <= count || threshold == -1) ){ 
				max = count;
				pt.Pos(i,j);
			}
		}
	}

	return pt;
}

//�e���v���[�g�}�b�`���O�͈͌���
int ImageUtil::Matching(biImage &image, int x,int y, biImage temp){
	int i,j;
	int max = 255 * 3 *temp.Width() * temp.Height();
	int count = 0;
	int valid_num = 0;

	//mimage���temp�̂ق����傫���Ƃ��͏I��
	if(image.Width() < temp.Width() || 
		image.Height() < temp.Height())return 0;

	for(j = 0 ; j < temp.Height(); j++){
		for(i = 0 ; i < temp.Width() ; i++){
			//�͈͊O�E�����F�̕����͊��S�Ɉ�v�������̂Ƃ���
			if( temp.Get(i,j) == temp.Clear()){
				//count += 255*3;
				continue;
			}
			if((i+x) >= image.Width() || (j+y) >= image.Height() || (i+x) < 0 || (j+y) < 0){
				//��ʊO�ɏo��Ƌɒ[�Ɉ�v����������
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

//�[�_���X�g���擾����
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

//����8�}�X�ɂ���Pixel(255)�̐����擾
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
//���x�����O
int ImageUtil::Labeling(biImage &image){
	int w,h,j,k,i,size;
	int label_count=1,flag_count;
	int min_data[8],min;
	int *Lookup_table,*sub_table,*label_list;
	int max=0,label=0,Lookup_flag=0;
	
	Lookup_table = new int[image.Width() * image.Height()];
	sub_table = new int[image.Width() * image.Height()];
	label_list = new int[image.Width() * image.Height()];

	//������
	size = image.Width() * image.Height();
	for(i = 0 ;i< size;i++){
		*(label_list+i) = 0;
		*(sub_table+i) = 0;
	}
	for(i=0;i<size;i++){
		Lookup_table[i] = i;
	}

	//���x�����O
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
					
					if(flag_count == 8){//���肪���ׂ�0������
						//������
					    //������
						//������
						*(label_list+w+h*image.Width()) = label_count;//���x����U��
						label_count++;
					}else{//�ꂩ���ł�255��������
						//�P���S
					    //������
						//������
						for(int i=0,count=0 ; i<9 ; i++){
							if( (i%3-1) == 0 && (i/3-1) == 0 ) continue;
							min_data[count] = *(label_list+(w+i%3-1)+(h+i/3-1)*image.Width());
							count++;
						}
	
						min = min_calc(min_data);//�ŏ��̃��x���ԍ��𒲂ׂ�
						//�P���S
					    //���P��
						//������
	
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
	//�{���x���ɏ�������
	for(h=0;h<label_count;h++){
		Lookup_table[h] = sub_table[Lookup_table[h]];
	}

	Pixel pixel;
	//���x���̊���U��
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

//���x���ԍ�����I�u�W�F�N�g�𒊏o����B���łɖʐς��Ԃ��B
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

//���x�����O�ς݉摜����ʐς��ő�̃I�u�W�F�N�g�𒊏o����
int ImageUtil::SamplingLargeObject(biImage &image){
	std::map<int,int> area_map;
	std::map<int,int>::iterator area;
	//�ʐς��擾
	for(int j=0 ; j < image.Height() ; j++){
		for(int i=0 ; i < image.Width() ; i++){
			int id = image.Get(i,j).Label();
			if(id != 0){//0�̓I�u�W�F�N�g�ł͂Ȃ��̂�
				area_map[id]++;
			}
		}
	}
	//�ʐς��傫���̂𒊏o
	int area_id, num,max=0;
	for(area = area_map.begin(); area != area_map.end() ; ++area){
 		num = area->second;//�ʐ�
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
