#include "gui/TextClass.h"

Text::Text(){
	ZeroMemory(&logfont,sizeof(logfont));
	color = RGB(0,0,0);
	area.Pos(0,0);
	area.Size(0,0);
	text = "";
}

//������
void Text::Initialize(){
	this->hFont=CreateFontIndirect(&logfont);
}
//�X�V����
void Text::Update(){
}
//�`�揈��
void Text::Draw(){
	RECT area;
	//�̈�
	area.left = this->area.X();
	area.top = this->area.Y();
	area.right = area.left + this->area.Width();
	area.bottom = area.top + this->area.Height();

	//�F�̐؂�ւ�
	SetTextColor(this->wnd->hdc, this->color);
	//�t�H���g�̐؂�ւ�
	SelectObject(this->wnd->hdc,hFont);
	TextOut(this->wnd->hdc,area.left,area.top,text.c_str(),text.size());
	//DrawText(this->wnd->hdc,text.c_str(),-1,&area, DT_LEFT | DT_WORDBREAK); //�\���ł��ĂȂ�
}
//�폜����
void Text::Delete(){
	DeleteObject(this->hFont);
}
//������ݒ�
void Text::Set(std::string text){
	this->text = text;
}
//�F��ݒ�
void Text::Color(unsigned char r,unsigned char g,unsigned char b){
	this->color = RGB(r,g,b);
}
//�ʒu��ݒ�
void Text::Pos(int x, int y){
	area.Pos(x,y);
}
//�傫����ݒ�
void Text::Size(int width, int height){
	logfont.lfWidth = width;
	logfont.lfHeight = height;
	this->hFont=CreateFontIndirect(&logfont);
}
//�傫����ݒ�
void Text::AreaSize(int width, int height){
	area.Size(width,height);
}
//�t�H���g�̐ݒ�
void Text::Font(std::string fontname){
	strcpy(logfont.lfFaceName,fontname.c_str());
	this->hFont=CreateFontIndirect(&logfont);
}