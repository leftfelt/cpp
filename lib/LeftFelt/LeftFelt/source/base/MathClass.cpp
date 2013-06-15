#include <base/MathClass.h>

double Math::Pi = 3.14159265358979323846264338327950288 ; //‰~Žü—¦

double Math::Angle(double radian){
	return (double)radian / (Pi / 180);
}
double Math::Radian(double angle){
	return (double)angle * ( Pi / 180);
}
