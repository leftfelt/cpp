#ifndef NUMBEROPERATORCLASS_INCLUDE
#define NUMBEROPERATORCLASS_INCLUDE

#include <malloc.h>
#include <string>
#include <typeinfo>

/* @name : NumberOperatorClass
*	
*Å@êîílå^ópââéZéqÉNÉâÉX
*/

class NumberOperator {
public:
	NumberOperator();
	NumberOperator(const NumberOperator &num);
	NumberOperator(const char &num);
	NumberOperator(const short &num);
	NumberOperator(const int &num);
	NumberOperator(const long &num);
	NumberOperator(const float &num);
	NumberOperator(const double &num);
	~NumberOperator();
	//å^îªíË
	bool isChar();
	bool isShort();
	bool isInt();
	bool isLong();
	bool isFloat();
	bool isDouble();
	//åªç›ÇÃå^ñºéÊìæ
	const char* getType();
protected:
	double value;	//íl
	size_t type;	//å^îªï óp
	
private:
	// +ââéZéq
	template <typename T> friend NumberOperator operator+(NumberOperator& num, T value);
	template <typename T> friend NumberOperator operator+(T value, NumberOperator& num);
	template <typename T> friend NumberOperator operator-(NumberOperator& num, T value);
	template <typename T> friend NumberOperator operator-(T value, NumberOperator& num);
	template <typename T> friend NumberOperator operator*(NumberOperator& num, T value);
	template <typename T> friend NumberOperator operator*(T value, NumberOperator& num);
	template <typename T> friend NumberOperator operator/(NumberOperator& num, T value);
	template <typename T> friend NumberOperator operator/(T value, NumberOperator& num);

	// <<ââéZéq(ë„ì¸Åj
	template <typename T> friend NumberOperator operator<<(NumberOperator &num, T value);
	template <typename T> friend T operator<<(T &value, NumberOperator& num);

	template <typename T> friend NumberOperator operator+=(NumberOperator &num, T value);
	template <typename T> friend NumberOperator operator+=(T &value, NumberOperator& num);
	template <typename T> friend NumberOperator operator-=(NumberOperator &num, T value);
	template <typename T> friend NumberOperator operator-=(T &value, NumberOperator& num);
	template <typename T> friend NumberOperator operator*=(NumberOperator &num, T value);
	template <typename T> friend NumberOperator operator*=(T &value, NumberOperator& num);
	template <typename T> friend NumberOperator operator/=(NumberOperator &num, T value);
	template <typename T> friend NumberOperator operator/=(T &value, NumberOperator num);
};
//=============================================================

template <typename T> NumberOperator operator<<(NumberOperator& num, T value){
	num.type = typeid(T).hash_code();
	num.value = (double)value;
	return num;
}
template <typename T> T operator<<(T &value, NumberOperator& num){
	value = (T)num.value;
	return value;
}

template <typename T> NumberOperator operator+(NumberOperator& num, T value){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return Num + value;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return Num + value;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return Num + value;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return Num + value;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return Num + value;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return Num + value;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}

template <typename T> NumberOperator operator+(T value, NumberOperator& num){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return value + Num;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return value + Num;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return value + Num;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return value + Num;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return value + Num;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return value + Num;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}
template <typename T> NumberOperator operator-(NumberOperator& num, T value){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return Num - value;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return Num - value;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return Num - value;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return Num - value;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return Num - value;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return Num - value;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}

template <typename T> NumberOperator operator-(T value, NumberOperator& num){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return value - Num;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return value - Num;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return value - Num;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return value - Num;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return value - Num;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return value - Num;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}
template <typename T> NumberOperator operator*(NumberOperator& num, T value){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return Num * value;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return Num * value;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return Num * value;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return Num * value;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return Num * value;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return Num * value;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}

template <typename T> NumberOperator operator*(T value, NumberOperator& num){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return value * Num;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return value * Num;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return value * Num;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return value * Num;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return value * Num;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return value * Num;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}
template <typename T> NumberOperator operator/(NumberOperator& num, T value){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return Num / value;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return Num / value;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return Num / value;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return Num / value;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return Num / value;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return Num / value;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}

template <typename T> NumberOperator operator/(T value, NumberOperator& num){
	if(num.isDouble()) {
		double Num;
		Num << num;
		return value / Num;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		return value / Num;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		return value / Num;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		return value / Num;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		return value / Num;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		return value / Num;
	}else{
		double Num;
		Num << num;
		return Num + value;
	}
}
//========================================================================================

template <typename T> NumberOperator operator+=(NumberOperator &num, T value){
	num = num + value;
	return num;
}
template <typename T> NumberOperator operator+=(T &value, NumberOperator& num){
	num = value + num;
	return num;
}
template <typename T> NumberOperator operator-=(NumberOperator &num, T value){
	num = num - value;
	return num;
}
template <typename T> NumberOperator operator-=(T &value, NumberOperator& num){
	num = value - num;
	return num;
}
template <typename T> NumberOperator operator*=(NumberOperator &num, T value){
	num = num * value;
	return num;
}
template <typename T> NumberOperator operator*=(T &value, NumberOperator& num){
	num = value * num;
	return num;
}
template <typename T> NumberOperator operator/=(NumberOperator &num, T value){
	num = num / value;
	return num;
}
template <typename T> NumberOperator operator/=(T &value, NumberOperator& num){
	num = value / num;
	return num;
}

#endif