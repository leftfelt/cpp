#ifndef INCLUDE_TEXTCLASS
#define INCLUDE_TEXTCLASS

#include <string>
#include <windows.h>
#include <gui/WindowClass.h>
#include <base/AreaClass.h>

//Text Class
class Text : public WindowObject{
private:
	LOGFONT logfont;
	COLORREF color;
	HFONT hFont;
	std::string text;
	//表示領域
	Area area;
public:
	Text();
	void Initialize();
	void Update();
	void Draw();
	void Delete();

	void Set(std::string text); //文字を設定
	void Color(unsigned char r,unsigned char g,unsigned char b); //色を設定
	void Pos(int x, int y);//位置を設定
	void Size(int width, int height);//大きさを設定
	void AreaSize(int width, int height);//大きさを設定
	void Font(std::string fontname);//フォントを設定
};

#endif