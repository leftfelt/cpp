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
* ������std::string�̃��b�p�[�N���X
*/

class String : public std::string{
private:
	void set(Number &num);
public:
	String();
	String(const char* str);
	String(Number num);
	String(const std::string& str);
	StringList Split(char delimiter);		//��؂蕶���ŕ���
	StringList FrontSplit(char delimiter = ' ');
	StringList EndSplit(char delimiter = ' ');
	void Replace(String from, String to);		//�u��
	void FrontStrip(char c = ' ');
	void EndStrip(char c = ' ');
	String Front(); //�擪�̈ꕶ��
	String End(); //�����̈ꕶ��
	void Find(String str, int index = 0);

	bool is_small_alphabet(int index);//�������p��
	bool is_large_alphabet(int index);//�啶���p��
	bool is_alphabet(int index); //[a-Z]�p��
	bool is_number(int index); //[0-9]����
	bool is_meta(int index);//���^����
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