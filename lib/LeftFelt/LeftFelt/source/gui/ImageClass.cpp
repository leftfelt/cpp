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

void Image::Initialize(HWND hWnd, HDC hdc){
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
	bhdc = GetDC(hWnd);
	hBmp = CreateDIBSection(bhdc, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS,((void**)&bits),NULL,0);
	hdc_mem = CreateCompatibleDC(bhdc);
	SelectObject(hdc_mem,hBmp);
	ReleaseDC(hWnd,bhdc);
}

void Image::Update(HWND hWnd, HDC hdc){
	
	int i,j;
	int width = Width();
	int height = Height();
	
	//画素をコピー
	for(j = 0 ; j < height ; j++){
		for(i = 0 ; i < width ; i++){
			if(j < height &&  i < width){//サイズが超えてないか
				(bits+i+(height-1-j)*width)->rgbBlue		 = mimage.at(i+j*width).Blue();
				(bits+i+(height-1-j)*width)->rgbGreen		 = mimage.at(i+j*width).Green();
				(bits+i+(height-1-j)*width)->rgbRed			 = mimage.at(i+j*width).Red();
				(bits+i+(height-1-j)*width)->rgbReserved	 = 0;
			}
		}
	}
}

void Image::Draw(HWND hWnd, HDC hdc){
	//表示
	BitBlt(hdc,mx,my,Width(),Height(),hdc_mem,0,0,SRCCOPY);
	
}

void Image::Delete(HWND hWnd, HDC hdc){
	DeleteObject(hBmp);
	DeleteDC(hdc_mem);
}

