#include <iostream>
#include <stdio.h>
#include "SoundClass.h"

using namespace std;

void main(){
	Sound a,b;
	long fram=0;

	a.Open("001.wav");

	a.Play();

	while(1){
		fram++;

		if(fram == 100000){
			a.Stop();//StopをしてからSeekをして、Playしないと反映されない
			a.Seek(9000);
			a.Play();//再生
		}

		if(fram == 120000){ //一時停止
			a.Pause();
			
		}
		if(fram == 200000){ //再開
			a.Resume();
		}

		if(fram == 250000){
			a.Stop();
			a.Seek(-9000);
			a.Volume(10,10);
			a.Play();
		}

		if(fram == 500000){
			a.Close();
		}


		printf("%d\n",fram);//
	}


	
}