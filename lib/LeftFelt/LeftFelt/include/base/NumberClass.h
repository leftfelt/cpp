#ifndef NUMBERCLASS_INCLUDE
#define NUMBERCLASS_INCLUDE

#include <malloc.h>
#include <string>
#include <typeinfo>

#include <base/NumberOperatorClass.h>

/* @name : NumberClass
*	
*�@���l�^�N���X
*/

class Number : public NumberOperator{
public:
	Number();
	Number(const Number &num);
	Number(const NumberOperator &num);
	Number(const char &num);
	Number(const short &num);
	Number(const int &num);
	Number(const long &num);
	Number(const float &num);
	Number(const double &num);
	~Number();

	//���Z�q
	Number operator+(Number &num);
	Number operator-(Number &num);
	Number operator*(Number &num);
	Number operator/(Number &num);
	Number operator+=(Number &num);
	Number operator-=(Number &num);
	Number operator*=(Number &num);
	Number operator/=(Number &num);

};
//=============================================================


#endif