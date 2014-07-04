#ifndef _BRAINWAVES_INCLUDE
#define _BRAINWAVES_INCLUDE

#include <map>
#include <vector>
#include <algorithm>
#include <string>

//ThinkGearから取得できる値を格納しておくクラス
class BrainWaves {
private:
	std::map<std::string, double> waves;
	std::vector<std::string> parameters;
public:
	BrainWaves();
	BrainWaves(std::map<std::string, double> waves);

	std::vector<std::string> getParameters();
	double getValue(std::string name);

	std::map<std::string,double> toMap();

	double getBattery();//バッテリー？
	double getPoorSignal();//信号品質？
	double getAttention();//集中度(0-100)
	double getMeditation();//リラックス度(0-100)
	double getRaw();//生波形(12bit)
	double getDelta();//デルタ波(0.5-2.75Hz)
	double getTheta();//シータ波(3.5-6.75Hz)
	double getAlpha1();//低アルファ波(7.5-9.25Hz)
	double getAlpha2();//高アルファ波(10-11.75Hz)
	double getBeta1();//低ベータ波(13-16.75Hz)
	double getBeta2();//高ベータ波(18-29.75Hz)
	double getGamma1();//低ガンマ波(31-39.75Hz)
	double getGamma2();//中ガンマ波(41-49.75Hz)
	double getBlinkStrength();//まばたき？

	BrainWaves operator+(BrainWaves waves);
	BrainWaves operator-(BrainWaves waves);
	BrainWaves operator*(BrainWaves waves);
	BrainWaves operator/(BrainWaves waves);
	
	BrainWaves operator+=(BrainWaves waves);
	BrainWaves operator-=(BrainWaves waves);
	BrainWaves operator*=(BrainWaves waves);
	BrainWaves operator/=(BrainWaves waves);
};

#endif