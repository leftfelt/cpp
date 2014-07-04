#ifndef _THINKGEARCLIENT_INCLUDE
#define _THINKGEARCLIENT_INCLUDE

#pragma comment(lib,"thinkgear.lib")
#include <thinkgear.h>
#include <algorithm>
#include <string>
#include <BrainWaves.h>

class ThinkGearClient {
private:
	int connection_id;
	std::map<std::string,int> waves_map;
	bool is_start_capture;//�L���v�`�����t���O
	int readPackets(int packet_num);//�w�肵���p�P�b�g�������ǂݎ��
	int enableAutoRead(bool enabled);//�p�P�b�g�̎����ǂݎ���L���ɂ���
	double getValue(int data_type);//�p�P�b�g���P�ǂݍ��݁A�w�肳�ꂽ��`�σf�[�^�^�̒l�ŕԂ�
	bool create();//�V����ID�����蓖�Ă�B
	double getCaptureData(int data_type);//�w�肳�ꂽ��`�σf�[�^�^�̒l��Ԃ�
public:
	ThinkGearClient();
	~ThinkGearClient();
	bool isStartCapture();//�L���v�`�����ł��邩
	int connect(std::string port_name, int bit_rate=TG_BAUD_9600, int data_type=TG_STREAM_PACKETS);//�w�肵���V���A���|�[�g�ɐڑ����s���AID�Ɗ֘A�t����
	void disconnect();//ID�Ɋ֘A�t�����Ă���V���A���|�[�g�Ƃ̐ڑ���ؒf����B
	int streamLog(std::string filename);//�ǂݎ�����o�C�g�������O�t�@�C���֎����I�ɋL�^����
	int dataLog(std::string filename);//��̓f�[�^�����O�t�@�C���֎����I�ɋL�^����
	void startCapture();//�p�P�b�g�̓ǂݎ����J�n����
	void stopCapture();//�p�P�b�g�̓ǂݎ����~���� 
	BrainWaves getWaves();
};


#endif