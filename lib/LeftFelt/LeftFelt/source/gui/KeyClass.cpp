#include "gui/KeyClass.h"
//�ÓI�����o
char Key::down[256];//�������u��
char Key::up[256];//�������u��

Key::Key(){
	int i;
	for(i = 0 ; i < 256 ; i++){
		keystate[i] = 0;
		up[i] = 0;
		down[i] = 0;
	}
}

//�L�[�̏�Ԃ��擾
bool Key::State(int key){
	if(key == 0x00 || key == 0xFF)return false;
	return ((keystate[key] & 0x80) != 0);
}

//�������u��
bool Key::UpState(int key){
	return (up[key] == 1);
}

//�������u��
bool Key::DownState(int key){
	return (down[key] == 1);
}


//�L�[�̏�Ԃ��X�V
void Key::Update(){
	int i;
	GetKeyboardState(keystate);

	for(i = 0 ; i < 256 ; i++){
		if(State(i)){
			if(down[i] == 0)down[i] = 1;//�������u��
			else if(down[i] == 1)down[i] = 2;
			up[i] = 0;
		}else{
			if(up[i] == 0)up[i] = 1;//�������u��
			else if(up[i] == 1)up[i] = 2;
			down[i] = 0;
		}
	}

}