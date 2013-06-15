#include <base/NumberClass.h>

Number::Number(){
}

Number::Number(const Number &num){
	this->type = num.type;
	this->value = num.value;
}
Number::Number(const NumberOperator &num){
	*(NumberOperator*)(this) = num;
}
Number::Number(const char &num){
	*this << num;
}
Number::Number(const short &num){
	*this << num;
}
Number::Number(const int &num){
	*this << num;
}
Number::Number(const long &num){
	*this << num;
}
Number::Number(const float &num){
	*this << num;
}
Number::Number(const double &num){
	*this << num;
}

Number::~Number(){
}

//‰‰Zq======================================================================
Number Number::operator+(Number &num){
	if(num.isDouble()) {
		return value + (double)num.value;
	}else if(num.isFloat()) {
		return value + (float)num.value;
	}else if(num.isLong()) {
		return value + (long)num.value;
	}else if(num.isInt()) {
		return value + (int)num.value;
	}else if(num.isShort()) {
		return value + (short)num.value;
	}else if(num.isChar()) {
		return value + (char)num.value;
	}
	return value + (double)num.value;
}

Number Number::operator-(Number &num){
	if(num.isDouble()) {
		return value - (double)num.value;
	}else if(num.isFloat()) {
		return value - (float)num.value;
	}else if(num.isLong()) {
		return value - (long)num.value;
	}else if(num.isInt()) {
		return value - (int)num.value;
	}else if(num.isShort()) {
		return value - (short)num.value;
	}else if(num.isChar()) {
		return value - (char)num.value;
	}
	return value - (double)num.value;
}
Number Number::operator*(Number &num){
	if(num.isDouble()) {
		return value * (double)num.value;
	}else if(num.isFloat()) {
		return value * (float)num.value;
	}else if(num.isLong()) {
		return value * (long)num.value;
	}else if(num.isInt()) {
		return value * (int)num.value;
	}else if(num.isShort()) {
		return value * (short)num.value;
	}else if(num.isChar()) {
		return value * (char)num.value;
	}
	return value * (double)num.value;
}
Number Number::operator/(Number &num){
	if(num.isDouble()) {
		return value / (double)num.value;
	}else if(num.isFloat()) {
		return value / (float)num.value;
	}else if(num.isLong()) {
		return value / (long)num.value;
	}else if(num.isInt()) {
		return value / (int)num.value;
	}else if(num.isShort()) {
		return value / (short)num.value;
	}else if(num.isChar()) {
		return value / (char)num.value;
	}
	return value / (double)num.value;
}


Number Number::operator+=(Number &num){
	*this = *this + num;
	return *this;
}
Number Number::operator-=(Number &num){
	*this = *this - num;
	return *this;
}
Number Number::operator*=(Number &num){
	*this = *this * num;
	return *this;
}
Number Number::operator/=(Number &num){
	*this = *this / num;
	return *this;
}