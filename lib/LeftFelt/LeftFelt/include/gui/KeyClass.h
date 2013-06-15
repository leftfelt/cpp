#ifndef KEYCLASS_INCLUDE
#define KEYCLASS_INCLUDE

#include <windows.h>

//�L�[�{�[�h�̏��
class Key{
private:
	unsigned char keystate[256];//�e�L�[�̏��
	static char down[256];//�������u��
	static char up[256];//�������u��
public:
	Key();
	bool State(int key);
	bool UpState(int key);
	bool DownState(int key);
	void Update();
};

#endif