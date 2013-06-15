#ifndef _INCLUDE_SOUNDCLASS
#define _INCLUDE_SOUNDCLASS

#include <windows.h>
#include <iostream>
#include <MMSystem.h>
#include <map> 
#include <string>
#pragma comment(lib, "winmm.lib")

using namespace std;

class Sound {
private:
	string name;		//サウンド名
	MCI_OPEN_PARMS open;	//デバイスID保存用
	MCI_PLAY_PARMS play;	//
	MCI_SEEK_PARMS seek;
	MCI_STATUS_PARMS status;	//再生位置
	MCI_GENERIC_PARMS genericParam;
	DWORD ms;
	static map<string,MCI_OPEN_PARMS> open_list;
public:
	Sound();
	void Open(string name);	//ファイルを開く->先に読み込み済みのほうを検索なければファイル読み込み
	void Play();	//再生
	void Stop();	//停止
	void Seek(int num);	//再生開始場所の移動
	void Pause();	//一時停止
	void Resume();	//一時停止解除
	void Volume(short left, short right); //音量を変更
	void Close();	//ファイルを閉じる
};

#endif