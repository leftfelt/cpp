#include <base/TagClass.h>

//コンストラクタ
Tag::Tag(){
	name = "";
	text = "";
}

//タグ名を設定
void Tag::setName(String name){
	this->name = name;
}
//属性を設定
void Tag::setProperty(String property, String value){
	this->propertys[property] = value;
}
//内部テキストを設定
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
* TagオブジェクトをXML・HTML形式のテキストで出力
* @return	XML・HTML形式のテキスト
**/
String Tag::Create(){
	String textTag = "";
	
	if(name != ""){
		//開始タグ
		textTag += "<" + name;
		//属性
		if(!this->propertys.empty()){
			std::map<String,String>::iterator itr;
			
			for(itr = this->propertys.begin() ; itr != this->propertys.end() ; itr++){
				textTag += 
					" "
					+ itr->first	//属性名
					+ "=\""
					+ itr->second	//値
					+ "\"";
			}
		}
		textTag += ">";
	}else if(text != ""){
		//テキストを表示
		textTag += this->text;
	}

	return textTag;
}
