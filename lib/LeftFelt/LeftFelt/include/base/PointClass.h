#ifndef POINTCLASS_INCLUDE
#define POINTCLASS_INCLUDE

#define PointList std::vector<Point>
/*
*/
class Point{
private:
	int mx;			//X座標
	int my;			//Y座標
public:
	Point();
	Point(int x, int y);		//コンストラクタ
	void Pos(int x, int y);				//位置を設定
	int X();							//X座標
	int Y();							//Y座標

	Point operator+(Point &point);
	Point operator-(Point &point);
	Point operator*(Point &point);
	Point operator/(Point &point);
	
	Point operator=(int value);
	Point operator+(int value);
	Point operator-(int value);
	Point operator*(int value);
	Point operator/(int value);
	
	Point operator+=(int value);
	Point operator-=(int value);
	Point operator*=(int value);
	Point operator/=(int value);
};

#endif