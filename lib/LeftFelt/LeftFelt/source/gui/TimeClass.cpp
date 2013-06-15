#include "gui/TimeClass.h"

Time::Time(){
	timeBeginPeriod(1);	//精度を1msに設定
}

Time::~Time(){
	timeEndPeriod(1); 
}

//計測開始
void Time::Start(){
	start = timeGetTime(); //開始時刻を取得
}

//計測終了
DWORD Time::End(){
	end = timeGetTime(); //終了時刻を取得
	return (end - start); //計測結果を返す
}