#ifndef SQUARECLASS_INCLUDE
#define SQUARECLASS_INCLUDE


#include <windows.h>
#include <gui/LineClass.h>

//四角形
class Square{
private:
	Line line[4];
public:
	void Apex(POINT pt0,POINT pt1,POINT pt2,POINT pt3);//4つの頂点を設定
	void Apex(int x, int y, int width, int height);
	void Apex(char num, int x, int y);//指定した頂点を設定
	void Move(int x, int y);//中心座標を移動
	POINT getApex(char num);//0～3で指定した頂点の座標を取得
	POINT getMiddle();//中点の座標を取得
	int getArea();//面積を取得（正確な値ではなく近似値・正確な場合もある）
	int getArea(biImage &image);//面積を取得（正確な値ではなく近似値・正確な場合もある）
	int getPerimeter();//周囲長を取得
	void Color(Pixel color);//色を指定
	void Draw(biImage &image);
};

#endif