#include "gui/TimeClass.h"

Time::Time(){
	timeBeginPeriod(1);	//���x��1ms�ɐݒ�
}

Time::~Time(){
	timeEndPeriod(1); 
}

//�v���J�n
void Time::Start(){
	start = timeGetTime(); //�J�n�������擾
}

//�v���I��
DWORD Time::End(){
	end = timeGetTime(); //�I���������擾
	return (end - start); //�v�����ʂ�Ԃ�
}