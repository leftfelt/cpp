#ifndef INCLUDE_IMAGECLASS
#define INCLUDE_IMAGECLASS

#include <string>
#include <windows.h>
#include <gui/WindowClass.h>
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
	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void Delete();
};


#endif