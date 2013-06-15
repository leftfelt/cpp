#include <iostream>
#include <base/PointClass.h>

using namespace std;

int main(){
	Point point;
	int x = 100, y=100;
	
	//À•W‚ğİ’è‚µ‚Äæ“¾‚Å‚«‚é‚©
	point.Pos(x,y);
	cout << "x:" << point.X() << "y:" << point.Y() << endl;

	point += 2;
	cout << "x:" << point.X() << "y:" << point.Y() << endl;
	point -= 2;
	cout << "x:" << point.X() << "y:" << point.Y() << endl;
	point *= 2;
	cout << "x:" << point.X() << "y:" << point.Y() << endl;
	point /= 2;
	cout << "x:" << point.X() << "y:" << point.Y() << endl;

	cin >> x;
	return 0;
}