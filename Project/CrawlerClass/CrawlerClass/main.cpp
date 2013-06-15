#pragma comment(lib,"../../../lib/FLib.lib")
#include <HttpClass.h>
#include <XMLClass.h>
#include <TagClass.h>

int main(){
	String url,response;
	Http http;
	TagList api,image_tag_list,link_tag_list;
	StringList image_list,link_list;
	StringList def_image;
	def_image.push_back("png");
	def_image.push_back("gif");
	def_image.push_back("jpg");
	def_image.push_back("jpeg");
	def_image.push_back("jpe");
	def_image.push_back("bmp");
	/*
	url = "https://www.google.co.jp/search?num=10&hl=ja&site=imghp&tbm=isch&source=hp&biw=1427&bih=709&q=%E4%BA%BA%E9%A1%9E%E3%81%AF%E8%A1%B0%E9%80%80%E3%81%97%E3%81%BE%E3%81%97%E3%81%9F&oq=%E4%BA%BA%E9%A1%9E%E3%81%AF%E8%A1%B0%E9%80%80%E3%81%97%E3%81%BE%E3%81%97%E3%81%9F&gs_l=img.3..0i4l10.5044.10351.0.10593.36.31.5.0.0.2.237.2996.20j10j1.31.0...0.0...1ac.3yrc49BRPDo";
	url.URLencode();
	response = http.Request("http://test.dev-flet.net/~flet/trunk_public/api/imageSetNextURL.php","POST","url="+url);
	//*/
	//（アクセス先を取得する）
	response = http.Request("http://test.dev-flet.net/~flet/image_crawler/api/imageGetNextURL.php");
	XML api_xml(response);
	api = api_xml.getElementsByTagName("api");
	if(api[0].getProperty("status") == "success"){
		url = api[0].getProperty("url");
	}else{
		//アクセス先を取得できなかったとき
		return 0;
	}

	puts("complete get NextURL");
	//ページにアクセスする
	response = http.Request(url);
	//画像URLとリンク先URLを抽出する
	puts("complete NextURL Request");
	XML html(response);
	image_tag_list = html.getElementsByTagName("img");
	link_tag_list = html.getElementsByTagName("a");
	puts("complete parse HTML");
	//画像URLを登録する
	for(int i = 0 ; i < (signed)image_tag_list.size() ; i++){
		url = image_tag_list[i].getProperty("src");
		if( (url.find("http://") == 0 )||(url.find("https://") == 0) ){
			//絶対パス
		}else{
			//相対パス
			http.setPath(url);
			url = http.getURL();
		}
		int j;
		for(j = 0 ; j < (signed)def_image.size() ; j++){
			if( def_image.at(j) == url.EndSplit('.')[1] ){
				break;
			}
		}
		//if(j == (signed)def_image.size())break;
		url.URLencode();
		image_list.push_back(url);
	}
	puts("complete registr imageURL");
	//リンク先を抽出する
	for(int i = 0 ; i < (signed)link_tag_list.size() ; i++){
		url = link_tag_list[i].getProperty("href");
		if( (url.find("http://") == 0 )||(url.find("https://") == 0) ){
			//絶対パス
		}else{
			//相対パス
			http.setPath(url);
			url = http.getURL();
		}
		url.URLencode();
		link_list.push_back(url);
	}
	puts("complete registr linkURL");

	//画像URLを登録する
	for(int i = 0 ; i < (signed)image_list.size() ; i++){
		url = image_list.at(i);
		http.Request("http://test.dev-flet.net/~flet/image_crawler/api/addImageURL.php","POST","url="+url);
	}
	puts("complete request imageURL");

	//リンク先を抽出する
	for(int i = 0 ; i < (signed)link_list.size() ; i++){
		url = link_list.at(i);
		http.Request("http://test.dev-flet.net/~flet/image_crawler/api/imageSetNextURL.php","POST","url="+url);
	}
	puts("complete request linkURL");

	return 0;
}
