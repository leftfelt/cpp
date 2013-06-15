#include "gui/ImageClass.h"

Image::Image(){
	init();
}

Image::Image(int width, int height ){
	init();
	this->Create(width,height);
}

void Image::init(){
	hBmp = NULL;
	hdc_mem = NULL;
	
	this->setPos(0,0);
}

//座標設定
void Image::setPos(int x,int y){
	this->mx = x;
	this->my = y;
}

void Image::Initialize(){
	HDC bhdc;

	//初期設定
	ZeroMemory(&pbmi,sizeof(BITMAPINFOHEADER));
	pbmi.biSize = sizeof(BITMAPINFOHEADER);
	pbmi.biBitCount = 32;
	pbmi.biWidth = Width();
	pbmi.biHeight = Height();
	pbmi.biPlanes = 1;
	pbmi.biCompression = BI_RGB;

	bmi.bmiHeader = pbmi;
	
	//HBITMAP作成
	bhdc = GetDC(this->wnd->hWnd);
	hBmp = CreateDIBSection(bhdc, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS,((void**)&bits),NULL,0);
	hdc_mem = CreateCompatibleDC(bhdc);
	SelectObject(hdc_mem,hBmp);
	ReleaseDC(this->wnd->hWnd,bhdc);
}

void Image::Update(){
	
	int i,j;
	int width = Width();
	int height = Height();
	
	//画素をコピー
	for(j = 0 ; j < height ; j++){
		for(i = 0 ; i < width ; i++){
			if(j < height &&  i < width){//サイズが超えてないか
				(bits+i+(height-1-j)*width)->rgbBlue		 = (mimage+i+j*width)->Blue();
				(bits+i+(height-1-j)*width)->rgbGreen		 = (mimage+i+j*width)->Green();
				(bits+i+(height-1-j)*width)->rgbRed			 = (mimage+i+j*width)->Red();
				(bits+i+(height-1-j)*width)->rgbReserved	 = 0;
			}
		}
	}
}

void Image::Draw(){
	//表示
	BitBlt(this->wnd->hdc,mx,my,Width(),Height(),hdc_mem,0,0,SRCCOPY);
	
}

void Image::Delete(){
	DeleteObject(hBmp);
	DeleteDC(hdc_mem);
}

