#include <base/StringClass.h>

void main(){
	StringList result;
	String str(30);
	str = "int a = 0, b = 1\n"
		  "test(a,10,20,30)\n"
		  "aaaaaa(a,20,11,3)\n"
		  "bbbbbb(b,20,dldl,as)";

	result = str.Split('\n');

	for(int i = 0 ; i < (int)result.size() ; i++){
		puts(result[i].c_str());
	}

	str = 2.5;
	puts(str.c_str());

	str = 30;
	puts(str.c_str());

	str = str + 30;
	puts(str.c_str());

	str += 30;
	puts(str.c_str());

	str = 2.5 + str;
	puts(str.c_str());

	String URL = "http://www.google.co.jp/hws/search?hl=ja&channel=ssp&client=fenrir-sub&adsafe=off&safe=off&lr=all&q=l—Þ‚ÍŠ‘Þ‚µ‚Ü‚µ‚½";

	URL.URLencode();

	puts(URL.c_str());

	String small_alphabet = "abcdefghijklmnopqrstuvwxyzA";
	String large_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZa";
	String number = "0123456789a";

	for(int i = 0 ; i < small_alphabet.size() ; i++){
		if(!small_alphabet.is_small_alphabet(i)){
			puts("small_alphabet");
		}
		if(!small_alphabet.is_alphabet(i)){
			puts("alphabet");
		}
	}
	for(int i = 0 ; i < large_alphabet.size() ; i++){
		if(!large_alphabet.is_large_alphabet(i)){
			puts("large_alphabet");
		}
		if(!large_alphabet.is_alphabet(i)){
			puts("alphabet");
		}
	}
	for(int i = 0 ; i < number.size() ; i++){
		if(!number.is_number(i)){
			puts("number");
		}
	}

	Number num = (int)40;
	String base = "";

	base = num;
	puts(base.c_str());
}