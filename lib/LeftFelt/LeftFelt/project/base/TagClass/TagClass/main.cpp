#include <base/TagClass.h>

void main(){
	String str = 
		"<html>"
		"<head>"
		"test"
		"</head>"
		"<body style='color:red' alt='test'>"
		"aaaa"
		"</body>"
		"</html>";
	Tag tag,tag2;
	
	//�^�O�����������Z�b�g����Ă��邩�̃e�X�g
	tag.setName("a");
	//�v���p�e�B���������Z�b�g����Ă��邩�̃e�X�g
	tag.setProperty("href","http://");
	//�����e�L�X�g���������Z�b�g����Ă��邩�̃e�X�g
	tag.setText("test");
	tag2.setName("div");
	tag2.setText("test");
	tag.setText(tag2.Create());
	
	puts(tag.Create().c_str());//���`�����Ȃ�
}
