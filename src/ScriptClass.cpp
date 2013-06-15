#include "ScriptClass.h"

void Script::Split(){					//��؂�
	int i,j;
	int num = 0;

	//input���X�V
	result.clear();
	for(i = 0 ; i < (signed)input.size(); i++){	//������̃T�C�Y����
		for(j = 0 ; j < (signed)delimiter.size();j++){
			if(input.at(i) == delimiter.at(j) && i != num){
				input.at(i) = '\0';
				result.push_back(input.substr(num,(i-num)));//�������ǉ�
				num = i+1; //��؂蕶���̎���ݒ�
			}
		}
	}
	//
	if(i != num) {
		result.push_back(input.substr(num,(i-num)));
	}
}

Script::Script(string delimiter){	//��؂蕶���̈ꊇ�ݒ�
	for(int i = 0 ; i < (signed) delimiter.size() ; i++){
		this->delimiter.push_back(delimiter.at(i));
	}
	Split();
}

Script::Script(){
}

void Script::Delimiter(char word){	//��؂蕶����ǉ�
	delimiter.push_back(word);
	Split();
}
void Script::String(string str){	//�S�̂̕������ݒ�
	input = str;
	Split();
}
int	 Script::Size(){				//��؂��̒P�ꐔ��Ԃ�
	return (int)result.size();
}

string Script::Get(int num){
	return result.at(num);
}