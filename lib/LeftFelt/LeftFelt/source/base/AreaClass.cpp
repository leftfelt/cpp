#include "base/AreaClass.h"

//コンストラクタ
Area::Area(int x=0, int y=0, int width=0, int height=0){
	mx			= x;
	my			= y;
	mwidth		= width;
	mheight	= height;
}

Area::Area(){
	mx			= 0;
	my			= 0;
	mwidth		= 0;
	mheight	= 0;
}

//位置を設定する
void Area::Pos(int x, int y){
	mx = x;
	my = y;
}

//サイズを設定する
void Area::Size(int width, int height){
	mwidth = width;
	mheight = height;
}

//X座標を取得
int Area::X(){
	return mx;
}

//Y座標を取得
int Area::Y(){
	return my;
}

//幅を取得
int Area::Width(){
	return mwidth;
}

//Y座標を取得
int Area::Height(){
	return mheight;
}

//当たり判定（点）
bool Area::HitCheck(int x,int y){
	if( mx <= x && x <= mx+mwidth && my <= y && y <= my+mheight){
		return true;
	}
	return false;
}
//当たり判定（矩形）
bool Area::HitCheck(Area area){
	if( area.HitCheck(mx,my) ||
		area.HitCheck(my+mwidth,my) ||
		area.HitCheck(my,my+mheight) ||
		area.HitCheck(my+mwidth,my+mheight) 
		){
		return true;
	}
	return false;
}
