#ifndef _INCLUDE_TIMECLASS
#define _INCLUDE_TIMECLASS

//windows�p
#include <windows.h>
#include <mmsystem.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

class Time{
private:
	TIMECAPS caps;
	DWORD start;	//�J�n����[ms]
	DWORD end;		//�I������[ms]
public:
	Time();
	~Time();
	void  Start();	//�v���J�n
	DWORD End();	//�v���I��-������������(End - Start)��Ԃ�
};

#endif