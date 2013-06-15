//<created by tanakura 2011.12.5>
#ifndef _INCLUDE_SCRIPTCLASS
#define _INCLUDE_SCRIPTCLASS

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/* Script�N���X
*�@���낢��ȈӖ��Ŏ׈��ȋC�����Ă����̂ŁA�폜����i�\��j
*�@�V����string�̃��b�p�[��String�N���X������āAsplit�Ƃ�������
*/

class Script {
private:
	string input;					//�S�̂̕�����
	vector<char> delimiter;			//��؂蕶��
	vector<string>	result;			//��؂��̕�����
	void Split();					//��؂�
public:
	Script(string delimiter);
	Script();
	void Delimiter(char word);		//��؂蕶����ݒ�
	void String(string str);		//�S�̂̕������ݒ�
	int	 Size();					//��؂��̒P�ꐔ��Ԃ�
	string Get(int num);			//��؂��̕�������擾
};

#endif