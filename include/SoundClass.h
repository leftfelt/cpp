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
	string name;		//�T�E���h��
	MCI_OPEN_PARMS open;	//�f�o�C�XID�ۑ��p
	MCI_PLAY_PARMS play;	//
	MCI_SEEK_PARMS seek;
	MCI_STATUS_PARMS status;	//�Đ��ʒu
	MCI_GENERIC_PARMS genericParam;
	DWORD ms;
	static map<string,MCI_OPEN_PARMS> open_list;
public:
	Sound();
	void Open(string name);	//�t�@�C�����J��->��ɓǂݍ��ݍς݂̂ق��������Ȃ���΃t�@�C���ǂݍ���
	void Play();	//�Đ�
	void Stop();	//��~
	void Seek(int num);	//�Đ��J�n�ꏊ�̈ړ�
	void Pause();	//�ꎞ��~
	void Resume();	//�ꎞ��~����
	void Volume(short left, short right); //���ʂ�ύX
	void Close();	//�t�@�C�������
};

#endif