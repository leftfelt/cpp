#include <base/StringClass.h>

//�R���X�g���N�^
String::String(){
}
String::String(const std::string& str){
	this->assign(str);
}
String::String(const char* str){
	this->assign(str);
}

String::String(Number num){
	this->set(num);
}

//���l�𕶎���ɕϊ����đ��
void String::set(Number &num){
	std::stringstream ss;

	if(num.isDouble()) {
		double Num;
		Num << num;
		ss << Num;
	}else if(num.isFloat()) {
		float Num;
		Num << num;
		ss << Num;
	}else if(num.isLong()) {
		long Num;
		Num << num;
		ss << Num;
	}else if(num.isInt()) {
		int Num;
		Num << num;
		ss << Num;
	}else if(num.isShort()) {
		short Num;
		Num << num;
		ss << Num;
	}else if(num.isChar()) {
		char Num;
		Num << num;
		ss << Num;
	}else{
		double Num;
		Num << num;
		ss << Num;
	}
	
	*this = ss.str();
}

//��؂蕶���ŕ����񕪊�
StringList String::Split(char delimiter){
	StringList list;

	int i = 0;
	int num = 0;

	list.clear();
	for(i = 0 ; i < (signed)this->size(); i++){	//������̃T�C�Y����
		if(this->at(i) == delimiter){
			if(i != num){
				String word="";
				word.assign(this->substr(num,(i-num)));
				word.erase(word.length(),1);//
				list.push_back(word);//�������ǉ�
				num = i+1; //��؂蕶���̎���ݒ�
			}else{
				num++;
			}
		}
	}
	//
	if(i != num) {
		String word="";
		word.assign(this->substr(num,(i-num)));
		list.push_back(word);
	}

	return list;
}

//�擪�����؂蕶���܂Ő؂���
StringList String::FrontSplit(char c){
	std::string::size_type index=0;
	StringList result;
	index = this->find(c);
	if(index == std::string::npos)index = this->size();
	result.push_back(this->substr(0,index));
	result.push_back(this->substr(index));
	return result;
}

//���������؂蕶���܂Ő؂���
StringList String::EndSplit(char c){
	std::string::size_type index=0;
	StringList result;
	index = this->rfind(c);
	if(index == std::string::npos)index = 0;
	result.push_back(this->substr(0,index));
	result.push_back(this->substr(index));
	return result;
}

//�u��
void String::Replace(String from, String to){
	std::string::size_type index=0;
	while(true){
		index = this->find(from,index);
		if(index == std::string::npos)break;
		this->replace(index,from.size(),to.c_str(),to.size());
		index += to.size();
	}
}

//�擪���폜
void String::FrontStrip(char c){
	std::string::size_type index=0;
	if((signed)this->size() == 0)return;
	for(index = 0 ; index < (signed)this->size() ; index++){
		if(this->at(index) != c)break;
	}
	*this = this->substr(index);
}

//�������폜
void String::EndStrip(char c){
	std::string::size_type index=0;
	if((signed)this->size() == 0)return;
	for(index = (signed)this->size()-1 ; index >= 0  ; index--){
		if(this->at(index) != c)break;
	}
	*this = this->substr(0,index+1);
}

//���K�\���Ō���
void String::Find(String str, int index){
}

//���p��
bool String::is_small_alphabet(int index){
	if('a' <= this->at(index) && this->at(index) <= 'z')return true;
	return false;
}
//��p��
bool String::is_large_alphabet(int index){
	if('A' <= this->at(index) && this->at(index) <= 'Z')return true;
	return false;
}
//�p��
bool String::is_alphabet(int index){
	if(this->is_small_alphabet(index) || this->is_large_alphabet(index))return true;
	return false;
}
//����
bool String::is_number(int index){
	if('0' <= this->at(index) && this->at(index) <= '9')return true;
	return false;
}
//���^����(���K�\���p)
bool String::is_meta(int index){
	String meta = "[(.^$*+?|)]";
	if(meta.find(this->at(index) != std::string::npos))return true;
	return false;
}
//ascii�R�[�h����(URL�G���R�[�h�p)
bool String::is_ascii(int index){
	String ascii = "'.-*)(_";
	if(this->is_alphabet(index) || this->is_number(index) ||
		(ascii.find(this->at(index)) != std::string::npos) ) return true;
	return false;
}
//���p�X�y�[�X
bool String::is_space(int index){
	if(this->at(index) == ' ')return true;
	return false;
}

//URL�G���R�[�h
//ASCII�R�[�h�ȊO��2�����ƂɁu%16�i������v�ɕϊ�����
void String::URLencode(){
	String result = "";
	unsigned char num;
	for(int i = 0 ; i < (signed)this->size() ; i++){
		if(this->is_ascii(i)){
 			result += this->at(i);
		}else{
			std::stringstream ss;
			num = this->at(i);
			ss << std::hex << (int)num;
			result += '%';
			result += ss.str();
		}
	}
	*this = result;
}

//�擪�̈ꕶ��
String String::Front(){
	int size = this->size();
	if(size == 0) return "";
	return (String)this->at(0);
}
//�����̈ꕶ��
String String::End(){
	int size = this->size()-1;
	if(size < 0) return "";
	return (String)this->at(size);
}



//���Z�q===================================================================-
String String::operator=(Number num){
	this->set(num);
	return *this;
}

String String::operator+(Number num){
	String str = num;

	str = *this + str;

	return str;
}

String String::operator+=(Number num){
	String str = num;

	*this = *this + str;

	return *this;
}

String String::operator+=(String str){
	*this = *this + str;

	return *this;
}

String operator+(Number num, String str){
	String number = num;

	str = number + str;

	return str;
}
