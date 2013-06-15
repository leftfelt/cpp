#ifndef INCLUDE_IMAGECLASS
#define INCLUDE_IMAGECLASS

#include <string>
#include <windows.h>
#include <gui/ObjectClass.h>
#include <image_util/biImageClass.h>

class Image : public biImage,public WindowObject{
private:
	HBITMAP hBmp;
	HDC hdc_mem;
	int mx,my;
	BITMAPINFOHEADER pbmi;
	BITMAPINFO bmi;
	RGBQUAD *bits;
	void init();
public:
	Image();
	Image(int width, int height);
	void setPos(int x,int y);
	virtual void Initialize(HWND hWnd, HDC hdc);
	virtual void Update(HWND hWnd, HDC hdc);
	virtual void Draw(HWND hWnd, HDC hdc);
	virtual void Delete(HWND hWnd, HDC hdc);
};


#endif