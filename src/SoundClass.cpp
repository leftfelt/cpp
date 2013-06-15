#include "SoundClass.h"

map<string, MCI_OPEN_PARMS> Sound::open_list; //�X�^�e�B�b�N�ϐ�

Sound::Sound(){
	ms = 0;
}

void Sound::Open(string name){	//�t�@�C�����J��->��ɓǂݍ��ݍς݂̂ق��������Ȃ���΃t�@�C���ǂݍ���

	open.lpstrDeviceType = "waveaudio";
	open.lpstrElementName = name.c_str();
	
	
	
	if(open_list.find(name) == open_list.end()){ //�}�b�v�ɑ��݂��Ȃ�
		if( mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&open ) != 0 ){//�t�@�C�����J��
			MessageBox( NULL, "�t�@�C�����I�[�v���ł��܂���\n", "�G���[", MB_OK );
			exit(1);
		}
		open_list.insert(pair<string,MCI_OPEN_PARMS>(name,open));//�}�b�v�ɒǉ�
	}else{	//�}�b�v�ɑ��݂����ꍇ
		open = open_list.at(name);	//�}�b�v������o��
	}//*/
}
void Sound::Play(){	//�Đ�
	mciSendCommand( open.wDeviceID, MCI_PLAY, 0, (DWORD_PTR)&play );
}
void Sound::Stop(){	//��~
	mciSendCommand( open.wDeviceID,MCI_STOP,0,0);
}
void Sound::Seek(int num){	//�Đ��J�n�ꏊ�̈ړ�
	status.dwItem = MCI_STATUS_POSITION;
	mciSendCommand( open.wDeviceID,MCI_STATUS , MCI_STATUS_ITEM, (DWORD_PTR)&status);//���݂̍Đ��ʒu���擾
	ms = status.dwReturn;
	seek.dwCallback = NULL;
	seek.dwTo = (ms+=num);
	mciSendCommand( open.wDeviceID, MCI_SEEK, MCI_NOTIFY | MCI_TO,(DWORD_PTR)&seek );
}
void Sound::Pause(){	//�ꎞ��~
	mciSendCommand( open.wDeviceID,MCI_PAUSE,0,0);
}
void Sound::Resume(){	//�ꎞ��~����
	mciSendCommand( open.wDeviceID,MCI_RESUME,0,0);
}

void Sound::Volume(short left, short right){
	waveOutSetVolume((HWAVEOUT)open.wDeviceID,MAKELONG(left,right));
}

void Sound::Close(){	//�t�@�C�������
	map<string,MCI_OPEN_PARMS>::iterator i;
	
	for(i = open_list.begin() ; i != open_list.end() ; i++){
		mciSendCommand( i->second.wDeviceID ,MCI_CLOSE,0,(DWORD_PTR)&genericParam);//�t�@�C�������ׂĕ���
	}

	open_list.clear();//���X�g�����ׂč폜
}