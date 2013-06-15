#include <base/TagClass.h>

//�R���X�g���N�^
Tag::Tag(){
	name = "";
	text = "";
}

//�^�O����ݒ�
void Tag::setName(String name){
	this->name = name;
}
//������ݒ�
void Tag::setProperty(String property, String value){
	this->propertys[property] = value;
}
//�����e�L�X�g��ݒ�
void Tag::setText(String text){
	Tag tag;
	tag.text = text;
}

String Tag::getName(){
	return this->name;
}
String Tag::getProperty(String property){
	return this->propertys[property];
}
String Tag::getText(){
	return this->text;
}

void Tag::clear(){
	name = "";
	text = "";
	propertys.clear();
}


bool Tag::isStartTag(){
	if(this->name != "") return true;
	return false;
}

bool Tag::isEndTag(){
	std::size_t index = this->name.find('/');
	if(index != std::string::npos){
		if(this->name.substr(index,this->name.size()-index) != "") return true;
	}
	return false;
}

bool Tag::isText(){
	if(this->isStartTag() == false && this->isEndTag() == false){
		if(this->text != "") return true;
	}
	return false;
}

/**
* Tag�I�u�W�F�N�g��XML�EHTML�`���̃e�L�X�g�ŏo��
* @return	XML�EHTML�`���̃e�L�X�g
**/
String Tag::Create(){
	String textTag = "";
	
	if(name != ""){
		//�J�n�^�O
		textTag += "<" + name;
		//����
		if(!this->propertys.empty()){
			std::map<String,String>::iterator itr;
			
			for(itr = this->propertys.begin() ; itr != this->propertys.end() ; itr++){
				textTag += 
					" "
					+ itr->first	//������
					+ "=\""
					+ itr->second	//�l
					+ "\"";
			}
		}
		textTag += ">";
	}else if(text != ""){
		//�e�L�X�g��\��
		textTag += this->text;
	}

	return textTag;
}
