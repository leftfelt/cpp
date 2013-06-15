#include "ScriptClass.h"

void Script::Split(){					//区切る
	int i,j;
	int num = 0;

	//inputを更新
	result.clear();
	for(i = 0 ; i < (signed)input.size(); i++){	//文字列のサイズだけ
		for(j = 0 ; j < (signed)delimiter.size();j++){
			if(input.at(i) == delimiter.at(j) && i != num){
				input.at(i) = '\0';
				result.push_back(input.substr(num,(i-num)));//文字列を追加
				num = i+1; //区切り文字の次を設定
			}
		}
	}
	//
	if(i != num) {
		result.push_back(input.substr(num,(i-num)));
	}
}

Script::Script(string delimiter){	//区切り文字の一括設定
	for(int i = 0 ; i < (signed) delimiter.size() ; i++){
		this->delimiter.push_back(delimiter.at(i));
	}
	Split();
}

Script::Script(){
}

void Script::Delimiter(char word){	//区切り文字を追加
	delimiter.push_back(word);
	Split();
}
void Script::String(string str){	//全体の文字列を設定
	input = str;
	Split();
}
int	 Script::Size(){				//区切り後の単語数を返す
	return (int)result.size();
}

string Script::Get(int num){
	return result.at(num);
}