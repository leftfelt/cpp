#ifndef STRINGCLASS_INCLUDE
#define STRINGCLASS_INCLUDE

#include <string>
#include <vector>

#include <sstream>
#include <base/NumberClass.h>

//
#define StringList std::vector<String>

/*
* StringClass
* 既存のstd::stringのラッパークラス
*/

class String : public std::string{
private:
	void set(Number &num);
public:
	String();
	String(const char* str);
	String(Number num);
	String(const std::string& str);
	StringList Split(char delimiter);		//区切り文字で分割
	StringList FrontSplit(char delimiter = ' ');
	StringList EndSplit(char delimiter = ' ');
	void Replace(String from, String to);		//置換
	void FrontStrip(char c = ' ');
	void EndStrip(char c = ' ');
	String Front(); //先頭の一文字
	String End(); //末尾の一文字
	void Find(String str, int index = 0);

	bool is_small_alphabet(int index);//小文字英字
	bool is_large_alphabet(int index);//大文字英字
	bool is_alphabet(int index); //[a-Z]英字
	bool is_number(int index); //[0-9]数字
	bool is_meta(int index);//メタ文字
	bool is_ascii(int index);
	bool is_space(int index);

	void URLencode();

	String operator=(Number num);
	String operator+(Number num);
	String operator+=(Number num);
	String operator+=(String str);

	friend String operator+(Number num, String str);

};

#endif