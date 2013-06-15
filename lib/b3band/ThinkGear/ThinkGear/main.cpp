#include <ThinkGearClass.h>

int main(){
	ThinkGear think_gear;
	BrainWaves waves;

	think_gear.Connect("COM4");
	think_gear.StartCapture();
	waves = think_gear.getWaves();
	puts("Capture TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getRaw());
	}
	think_gear.StopCapture();
	getchar();
	think_gear.StartCapture();
	puts("getBattery TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getBattery());
	}
	getchar();
	puts("getPoorSignal TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getPoorSignal());
	}
	getchar();
	puts("getAttention TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getAttention());
	}
	getchar();
	puts("getMeditation TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getMeditation());
	}
	getchar();
	puts("getRaw TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getRaw());
	}
	getchar();
	puts("getDelta TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getDelta());
	}
	getchar();
	puts("getTheta TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getTheta());
	}
	getchar();
	puts("getAlpha1 TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getAlpha1());
	}
	getchar();
	puts("getAlpha2 TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getAlpha2());
	}
	getchar();
	puts("getBeta1 TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getBeta1());
	}
	getchar();
	puts("getBeta2 TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getBeta2());
	}
	getchar();
	puts("getGamma1 TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getGamma1());
	}
	getchar();
	puts("getGamma2 TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getGamma2());
	}
	getchar();
	puts("getBlinkStrength TEST");
	for(int i = 0 ; i<100 ; i++){
		printf("%f\n",waves.getBlinkStrength());
	}
	getchar();
	think_gear.StopCapture();
	think_gear.DisConnect();
}
