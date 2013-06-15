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
	
	//タグ名が正しくセットされているかのテスト
	tag.setName("a");
	//プロパティが正しくセットされているかのテスト
	tag.setProperty("href","http://");
	//内部テキストが正しくセットされているかのテスト
	tag.setText("test");
	tag2.setName("div");
	tag2.setText("test");
	tag.setText(tag2.Create());
	
	puts(tag.Create().c_str());//整形処理なし
}
