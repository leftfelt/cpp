#include "SoundClass.h"

map<string, MCI_OPEN_PARMS> Sound::open_list; //スタティック変数

Sound::Sound(){
	ms = 0;
}

void Sound::Open(string name){	//ファイルを開く->先に読み込み済みのほうを検索なければファイル読み込み

	open.lpstrDeviceType = "waveaudio";
	open.lpstrElementName = name.c_str();
	
	
	
	if(open_list.find(name) == open_list.end()){ //マップに存在しない
		if( mciSendCommand( 0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&open ) != 0 ){//ファイルを開く
			MessageBox( NULL, "ファイルをオープンできません\n", "エラー", MB_OK );
			exit(1);
		}
		open_list.insert(pair<string,MCI_OPEN_PARMS>(name,open));//マップに追加
	}else{	//マップに存在した場合
		open = open_list.at(name);	//マップから取り出す
	}//*/
}
void Sound::Play(){	//再生
	mciSendCommand( open.wDeviceID, MCI_PLAY, 0, (DWORD_PTR)&play );
}
void Sound::Stop(){	//停止
	mciSendCommand( open.wDeviceID,MCI_STOP,0,0);
}
void Sound::Seek(int num){	//再生開始場所の移動
	status.dwItem = MCI_STATUS_POSITION;
	mciSendCommand( open.wDeviceID,MCI_STATUS , MCI_STATUS_ITEM, (DWORD_PTR)&status);//現在の再生位置を取得
	ms = status.dwReturn;
	seek.dwCallback = NULL;
	seek.dwTo = (ms+=num);
	mciSendCommand( open.wDeviceID, MCI_SEEK, MCI_NOTIFY | MCI_TO,(DWORD_PTR)&seek );
}
void Sound::Pause(){	//一時停止
	mciSendCommand( open.wDeviceID,MCI_PAUSE,0,0);
}
void Sound::Resume(){	//一時停止解除
	mciSendCommand( open.wDeviceID,MCI_RESUME,0,0);
}

void Sound::Volume(short left, short right){
	waveOutSetVolume((HWAVEOUT)open.wDeviceID,MAKELONG(left,right));
}

void Sound::Close(){	//ファイルを閉じる
	map<string,MCI_OPEN_PARMS>::iterator i;
	
	for(i = open_list.begin() ; i != open_list.end() ; i++){
		mciSendCommand( i->second.wDeviceID ,MCI_CLOSE,0,(DWORD_PTR)&genericParam);//ファイルをすべて閉じる
	}

	open_list.clear();//リストをすべて削除
}