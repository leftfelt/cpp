#include <base/PointClass.h>


Point::Point(){
	this->Pos(0,0);
}
//コンストラクタ
Point::Point(int x, int y){
	this->Pos(x,y);
}
//位置を設定
void Point::Pos(int x, int y){
	this->mx = x;
	this->my = y;
}
//X座標
int Point::X(){
	return mx;
}
//Y座標
int Point::Y(){
	return my;
}
//==============================================================================
Point Point::operator+(Point &point){
	Point pt = *this;
	pt.Pos(
		pt.X() + point.X(),
		pt.Y() + point.Y()
	);
	return pt;
}
Point Point::operator-(Point &point){
	Point pt = *this;
	pt.Pos(
		pt.X() - point.X(),
		pt.Y() - point.Y()
	);
	return pt;
}
Point Point::operator*(Point &point){
	Point pt = *this;
	pt.Pos(
		pt.X() * point.X(),
		pt.Y() * point.Y()
	);
	return pt;
}
Point Point::operator/(Point &point){
	Point pt = *this;
	pt.Pos(
		pt.X() / point.X(),
		pt.Y() / point.Y()
	);
	return pt;
}


Point Point::operator=(int value){
	Point pt = *this;
	pt.Pos( value, value );
	return pt;
}
Point Point::operator+(int value){
	Point pt = *this;
	pt.Pos(
		pt.X() + value,
		pt.Y() + value
	);
	return pt;
}
Point Point::operator-(int value){
	Point pt = *this;
	pt.Pos(
		pt.X() - value,
		pt.Y() - value
	);
	return pt;
}
Point Point::operator*(int value){
	Point pt = *this;
	pt.Pos(
		pt.X() * value,
		pt.Y() * value
	);
	return pt;
}
Point Point::operator/(int value){
	Point pt = *this;
	pt.Pos(
		pt.X() / value,
		pt.Y() / value
	);
	return pt;
}


Point Point::operator+=(int value){
	*this = *this + value;
	return *this;
}
Point Point::operator-=(int value){
	*this = *this - value;
	return *this;
}
Point Point::operator*=(int value){
	*this = *this * value;
	return *this;
}
Point Point::operator/=(int value){
	*this = *this / value;
	return *this;
}