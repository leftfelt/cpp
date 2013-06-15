#include <iostream>
#include <string>
#include <base/NetworkClass.h>
#include <base/SocketClass.h>
#include <base/HttpClass.h>

//HTTP通信テスト
#define  MESSAGE1  "GET / HTTP/1.0\r\nHOST: www.yahoo.co.jp\r\n\r\n"

int main(){
	Network net;
	Socket client;
	std::string str;

	Http http;
	
	str = http.Request("http://www.geocities.jp/ky_webid/cpp/library/001.html");
	puts(str.c_str());
	str = http.Request("http://www.yahoo.co.jp");
	puts(str.c_str());
	//GETメソッドのテスト
	puts("GET method test");
	str = http.Request("http://test.dev-flet.net/~flet/debug.php?data=aaaa&test=bbbb");
	puts(str.c_str());
	//POSTメソッドのテスト
	puts("POST method test");
	str = http.Request("http://test.dev-flet.net/~flet/sample/javascript/prototype.html","POST","data=aaaa&test=bbbb");
	puts(str.c_str());
	//HEADメソッドのテスト
	puts("HEAD method test");
	str = http.Request("../../debug.php","HEAD");
	puts(str.c_str());
	//HEADメソッドのテスト
	puts("相対パス test");
	str = http.Request("sample/javascript/prototype.html","GET");
	puts(str.c_str());
	//HTTPSのテスト
	puts("HTTPS test");
	str = http.Request("https://www.google.co.jp/search?num=10&hl=ja&site=imghp&tbm=isch&source=hp&biw=1427&bih=709&q=%E4%BA%BA%E9%A1%9E%E3%81%AF%E8%A1%B0%E9%80%80%E3%81%97%E3%81%BE%E3%81%97%E3%81%9F&oq=%E4%BA%BA%E9%A1%9E%E3%81%AF%E8%A1%B0%E9%80%80%E3%81%97%E3%81%BE%E3%81%97%E3%81%9F&gs_l=img.3..0i4l10.5044.10351.0.10593.36.31.5.0.0.2.237.2996.20j10j1.31.0...0.0...1ac.3yrc49BRPDo","GET");
	puts(str.c_str());

	http.setPath("");
	
	try{
		client = net.OpenTCP();
		client.Connect("www.yahoo.co.jp",80);
		client.Send(MESSAGE1);
		client.Recv(str,1024);
		puts(str.c_str());
		puts(str.c_str());
	}catch(char* mes){
		puts(mes);
	}
	return 0;
}
