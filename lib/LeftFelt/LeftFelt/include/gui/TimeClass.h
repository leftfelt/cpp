#ifndef _INCLUDE_TIMECLASS
#define _INCLUDE_TIMECLASS

//windows用
#include <windows.h>
#include <mmsystem.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

class Time{
private:
	TIMECAPS caps;
	DWORD start;	//開始時刻[ms]
	DWORD end;		//終了時刻[ms]
public:
	Time();
	~Time();
	void  Start();	//計測開始
	DWORD End();	//計測終了-かかった時間(End - Start)を返す
};

#endif