#ifndef LINECLASS_INCLUDE
#define LINECLASS_INCLUDE


#include <windows.h>
#include <math.h>
#include <image_util/biImageClass.h>

//線分
class Line{
private:
	POINT pt[2];//0:始点 1:終点
	Pixel mcolor; //色
public:
	Line();
	void Color(Pixel color);	//色を指定
	void Start(int x , int y); //始点を設定
	void End(int x, int y);	   //終点を設定
	void Move(int x, int y);
	POINT getMiddle();//中点を取得
	POINT getStart();		//始点を取得
	POINT getEnd();		//終点を取得
	int getDistance();//二点間の距離を取得
	void Draw(biImage &image);
	bool isClossing(Line line); //交差判定
};

#endif