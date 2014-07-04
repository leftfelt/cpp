#ifndef _BRAINWAVES_INCLUDE
#define _BRAINWAVES_INCLUDE

#include <map>
#include <vector>
#include <algorithm>
#include <string>

//ThinkGear����擾�ł���l���i�[���Ă����N���X
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

	double getBattery();//�o�b�e���[�H
	double getPoorSignal();//�M���i���H
	double getAttention();//�W���x(0-100)
	double getMeditation();//�����b�N�X�x(0-100)
	double getRaw();//���g�`(12bit)
	double getDelta();//�f���^�g(0.5-2.75Hz)
	double getTheta();//�V�[�^�g(3.5-6.75Hz)
	double getAlpha1();//��A���t�@�g(7.5-9.25Hz)
	double getAlpha2();//���A���t�@�g(10-11.75Hz)
	double getBeta1();//��x�[�^�g(13-16.75Hz)
	double getBeta2();//���x�[�^�g(18-29.75Hz)
	double getGamma1();//��K���}�g(31-39.75Hz)
	double getGamma2();//���K���}�g(41-49.75Hz)
	double getBlinkStrength();//�܂΂����H

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