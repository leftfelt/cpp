#ifndef POINTCLASS_INCLUDE
#define POINTCLASS_INCLUDE

#define PointList std::vector<Point>
/*
*/
class Point{
private:
	int mx;			//X���W
	int my;			//Y���W
public:
	Point();
	Point(int x, int y);		//�R���X�g���N�^
	void Pos(int x, int y);				//�ʒu��ݒ�
	int X();							//X���W
	int Y();							//Y���W

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