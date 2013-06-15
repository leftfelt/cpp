#ifndef KEYCLASS_INCLUDE
#define KEYCLASS_INCLUDE

#include <windows.h>

//キーボードの情報
class Key{
private:
	unsigned char keystate[256];//各キーの状態
	static char down[256];//押した瞬間
	static char up[256];//離した瞬間
public:
	Key();
	bool State(int key);
	bool UpState(int key);
	bool DownState(int key);
	void Update();
};

#endif