#include <stdio.h>
#include <base/XMLClass.h>

int main(){
	String source = 
		"<html>"
		"<head>"
		"<title>asdfasdf</title>"
		"</head>"
		"<body tes style = 'width:100px;' background='test'>"
		"</body>"
		"<test url=\"http://www.google.co.jp/hws/search?hl=ja&channel=ssp&client=fenrir-sub&adsafe=off&safe=off&lr=all&q=l—Þ‚ÍŠ‘Þ‚µ‚Ü‚µ‚½\" checked>"
		"<description />"
		"<test test=\"a'aa\">"
		"</test>"
		"</html>";

	

	XML xml(source);
	TagList tag_list;
	tag_list = xml.getElementsByTagName("body");

	for(int i = 0 ; i < (signed)tag_list.size() ; i++){
		puts(tag_list[i].Create().c_str());
	}

	return 0;
}
