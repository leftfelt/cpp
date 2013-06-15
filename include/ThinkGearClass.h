#ifndef _THINKGEARCLASS_INCLUDE
#define _THINKGEARCLASS_INCLUDE

#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
#else
	#pragma comment(lib,"base.lib")
#endif

#pragma comment(lib,"thinkgear.lib")
#include <thinkgear.h>
#include <base/StringClass.h>
#include <map>
#include <algorithm>

//ThinkGear����擾�ł���l���i�[���Ă����N���X
class BrainWaves {
private:
	std::map<String, double> waves;
	std::vector<String> parameters;
public:
	BrainWaves();
	BrainWaves(std::map<String, double> waves);

	std::vector<String> getParameters();
	double getValue(String name);

	std::map<String,double> toMap();

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


class ThinkGear {
private:
	std::map<String,int> waves_map;
	int connection_id;
	bool is_start_capture;//�L���v�`�����t���O
	int ReadPackets(int packet_num);//�w�肵���p�P�b�g�������ǂݎ��
	int EnableAutoRead(bool enabled);//�p�P�b�g�̎����ǂݎ���L���ɂ���
	double getValue(int data_type);//�p�P�b�g���P�ǂݍ��݁A�w�肳�ꂽ��`�σf�[�^�^�̒l�ŕԂ�
	bool Create();//�V����ID�����蓖�Ă�B
	double getCaptureData(int data_type);//�w�肳�ꂽ��`�σf�[�^�^�̒l��Ԃ�
public:
	ThinkGear();
	~ThinkGear();
	bool isStartCapture();//�L���v�`�����ł��邩
	int Connect(String port_name, int bit_rate=TG_BAUD_9600, int data_type=TG_STREAM_PACKETS);//�w�肵���V���A���|�[�g�ɐڑ����s���AID�Ɗ֘A�t����
	void DisConnect();//ID�Ɋ֘A�t�����Ă���V���A���|�[�g�Ƃ̐ڑ���ؒf����B
	int StreamLog(String filename);//�ǂݎ�����o�C�g�������O�t�@�C���֎����I�ɋL�^����
	int DataLog(String filename);//��̓f�[�^�����O�t�@�C���֎����I�ɋL�^����
	void StartCapture();//�p�P�b�g�̓ǂݎ����J�n����
	void StopCapture();//�p�P�b�g�̓ǂݎ����~���� 
	BrainWaves getWaves();
};




#endif