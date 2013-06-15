#ifndef TAGCLASASINCLUDE
#define TAGCLASASINCLUDE

#include <base/StringClass.h>
#include <map>
#include <vector>

#define TagList std::vector<Tag>

//Tag Class
class Tag{
private:
	String name;  //タグ名
	String text;  //文字列
	std::map<String,String> propertys;  //属性
public:
	Tag();
	void setName(String name);
	void setProperty(String property, String value);
	void setText(String text);

	String getName();
	String getProperty(String property);
	String getText();

	bool isStartTag();
	bool isEndTag();
	bool isText();

	void clear();

	//テキストを返すのでobjectを継承したクラスに移動してＤｒａｗ時に行うことにしよう
	String Create();	//別クラスに移動
};

#endif