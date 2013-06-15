#ifndef _AREA_INCLUDE
#define _AREA_INCLUDE

#include <math.h>

#define AreaList std::vector<Area>
/*
矩形又は円形のエリアを作成する
エリア同士の衝突判定を行う
*/
class Area{
private:
	int mx;			//X座標
	int my;			//Y座標
	int mheight;	//高さ
	int mwidth;		//幅
public:
	Area();
	Area(int x, int y, int width, int height);		//コンストラクタ
	void Pos(int x, int y);					//位置を設定
	void Size(int width, int height);		//サイズを設定
	int X();							//X座標
	int Y();							//Y座標
	int Width();						//幅を返す
	int Height();						//高さを返す
	bool HitCheck(int x,int y);				//当たり判定（点）
	bool HitCheck(Area area);				//当たり判定（矩形）
};

#endif