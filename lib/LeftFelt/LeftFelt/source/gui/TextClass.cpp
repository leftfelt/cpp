#include "gui/TextClass.h"

Text::Text(){
	ZeroMemory(&logfont,sizeof(logfont));
	color = RGB(0,0,0);
	area.Pos(0,0);
	area.Size(0,0);
	text = "";
}

//初期化
void Text::Initialize(){
	this->hFont=CreateFontIndirect(&logfont);
}
//更新処理
void Text::Update(){
}
//描画処理
void Text::Draw(){
	RECT area;
	//領域
	area.left = this->area.X();
	area.top = this->area.Y();
	area.right = area.left + this->area.Width();
	area.bottom = area.top + this->area.Height();

	//色の切り替え
	SetTextColor(this->wnd->hdc, this->color);
	//フォントの切り替え
	SelectObject(this->wnd->hdc,hFont);
	TextOut(this->wnd->hdc,area.left,area.top,text.c_str(),text.size());
	//DrawText(this->wnd->hdc,text.c_str(),-1,&area, DT_LEFT | DT_WORDBREAK); //表示できてない
}
//削除処理
void Text::Delete(){
	DeleteObject(this->hFont);
}
//文字を設定
void Text::Set(std::string text){
	this->text = text;
}
//色を設定
void Text::Color(unsigned char r,unsigned char g,unsigned char b){
	this->color = RGB(r,g,b);
}
//位置を設定
void Text::Pos(int x, int y){
	area.Pos(x,y);
}
//大きさを設定
void Text::Size(int width, int height){
	logfont.lfWidth = width;
	logfont.lfHeight = height;
	this->hFont=CreateFontIndirect(&logfont);
}
//大きさを設定
void Text::AreaSize(int width, int height){
	area.Size(width,height);
}
//フォントの設定
void Text::Font(std::string fontname){
	strcpy(logfont.lfFaceName,fontname.c_str());
	this->hFont=CreateFontIndirect(&logfont);
}