#include "gui/KeyClass.h"
//静的メンバ
char Key::down[256];//押した瞬間
char Key::up[256];//離した瞬間

Key::Key(){
	int i;
	for(i = 0 ; i < 256 ; i++){
		keystate[i] = 0;
		up[i] = 0;
		down[i] = 0;
	}
}

//キーの状態を取得
bool Key::State(int key){
	if(key == 0x00 || key == 0xFF)return false;
	return ((keystate[key] & 0x80) != 0);
}

//押した瞬間
bool Key::UpState(int key){
	return (up[key] == 1);
}

//離した瞬間
bool Key::DownState(int key){
	return (down[key] == 1);
}


//キーの状態を更新
void Key::Update(){
	int i;
	GetKeyboardState(keystate);

	for(i = 0 ; i < 256 ; i++){
		if(State(i)){
			if(down[i] == 0)down[i] = 1;//押した瞬間
			else if(down[i] == 1)down[i] = 2;
			up[i] = 0;
		}else{
			if(up[i] == 0)up[i] = 1;//離した瞬間
			else if(up[i] == 1)up[i] = 2;
			down[i] = 0;
		}
	}

}