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
			a.Stop();//Stop‚ğ‚µ‚Ä‚©‚çSeek‚ğ‚µ‚ÄAPlay‚µ‚È‚¢‚Æ”½‰f‚³‚ê‚È‚¢
			a.Seek(9000);
			a.Play();//Ä¶
		}

		if(fram == 120000){ //ˆê’â~
			a.Pause();
			
		}
		if(fram == 200000){ //ÄŠJ
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