#include <base/NumberOperatorClass.h>


//コンストラクタ
NumberOperator::NumberOperator(){
	this->value = 0;
	this->type = 0;
}

//コピーコンストラクタ
NumberOperator::NumberOperator(const NumberOperator &num){
	this->type = num.type;
	this->value = num.value;
}
NumberOperator::NumberOperator(const char &num){
	*this << num;
}
NumberOperator::NumberOperator(const short &num){
	*this << num;
}
NumberOperator::NumberOperator(const int &num){
	*this << num;
}
NumberOperator::NumberOperator(const long &num){
	*this << num;
}
NumberOperator::NumberOperator(const float &num){
	*this << num;
}
NumberOperator::NumberOperator(const double &num){
	*this << num;
}
//デストラクタ
NumberOperator::~NumberOperator(){
}

//型を判別する
bool NumberOperator::isChar(){
	return typeid(char).hash_code() == type;
}

bool NumberOperator::isShort(){
	return typeid(short).hash_code() == type;
}

bool NumberOperator::isInt(){
	return typeid(int).hash_code() == type;
}
bool NumberOperator::isLong(){
	return typeid(long).hash_code() == type;
}
bool NumberOperator::isFloat(){
	return typeid(float).hash_code() == type;
}
bool NumberOperator::isDouble(){
	return typeid(double).hash_code() == type;
}

//現在の型名取得
const char* NumberOperator::getType(){
	if(isChar())return typeid(char).name();
	if(isShort())return typeid(short).name();
	if(isInt())return typeid(int).name();
	if(isLong())return typeid(long).name();
	if(isFloat())return typeid(float).name();
	if(isDouble())return typeid(double).name();
	return NULL;
}