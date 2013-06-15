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
	//�\���̈�
	Area area;
public:
	Text();
	void Initialize();
	void Update();
	void Draw();
	void Delete();

	void Set(std::string text); //������ݒ�
	void Color(unsigned char r,unsigned char g,unsigned char b); //�F��ݒ�
	void Pos(int x, int y);//�ʒu��ݒ�
	void Size(int width, int height);//�傫����ݒ�
	void AreaSize(int width, int height);//�傫����ݒ�
	void Font(std::string fontname);//�t�H���g��ݒ�
};

#endif