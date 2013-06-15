#ifndef TAGCLASASINCLUDE
#define TAGCLASASINCLUDE

#include <base/StringClass.h>
#include <map>
#include <vector>

#define TagList std::vector<Tag>

//Tag Class
class Tag{
private:
	String name;  //�^�O��
	String text;  //������
	std::map<String,String> propertys;  //����
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

	//�e�L�X�g��Ԃ��̂�object���p�������N���X�Ɉړ����Ăc���������ɍs�����Ƃɂ��悤
	String Create();	//�ʃN���X�Ɉړ�
};

#endif