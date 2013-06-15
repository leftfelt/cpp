//<created by tanakura 2011.12.5>
#ifndef _INCLUDE_SCRIPTCLASS
#define _INCLUDE_SCRIPTCLASS

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/* Scriptクラス
*　いろいろな意味で邪悪な気がしてきたので、削除する（予定）
*　新しくstringのラッパーでStringクラスを作って、splitとかを実装
*/

class Script {
private:
	string input;					//全体の文字列
	vector<char> delimiter;			//区切り文字
	vector<string>	result;			//区切り後の文字列
	void Split();					//区切る
public:
	Script(string delimiter);
	Script();
	void Delimiter(char word);		//区切り文字を設定
	void String(string str);		//全体の文字列を設定
	int	 Size();					//区切り後の単語数を返す
	string Get(int num);			//区切り後の文字列を取得
};

#endif