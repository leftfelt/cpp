#ifndef INCLUDE_CLASSIFIER
#define INCLUDE_CLASSIFIER

#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
#endif

#include <base/MathClass.h>
#include <gui/ImageClass.h>
#include <image_util/ImageUtilClass.h>
#include <algorithm>
#include <DictionaryRepository.h>
#include <PatternRepository.h>
#include <random>

//���ފ�̐ݒ�
class ClassifierSetting{
public:
	int cluster_size; //���ނ���N���X�^�̐�
	int dimension_num; //������
	int classification_num; //1��̕��މ�
	double data_min; //�f�[�^�̍ŏ���
	double data_max; //�f�[�^�̍ő吔
	double permissible_error; //���O�̖ړI�֐��Ƃ̍������̒l�ȉ��Ȃ番�ނ��I������
};

//���ފ�
class Classifier{
private:
	Pattern getMiddle(int num, std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list);
protected:
	ClassifierSetting setting;
public:
	Classifier();
	Classifier(ClassifierSetting setting);
	
	std::vector<Pattern> getRandomMiddleList(double min, double max, int size=0);
	std::vector<Pattern> getRandomMiddleList(std::vector<Pattern> data_list);

	//�ړI�֐�
	virtual double objective(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list) = 0;

	/**
	 * �����o�V�b�v�֐�
	 * @return double >= 0
	 * ��[num=0..N] getMembership(num, patternM) = 1, N=cluster_num M=pattern_num
	 */
	virtual double getMembership(int num, Pattern pattern, std::vector<Pattern> middle_list) = 0;

	/**
	 * �d�݊֐�
	 * @return double > 0
	 */
	virtual double getWeight(Pattern pattern) = 0;

	//����
	std::vector<Pattern> classify(std::vector<Pattern> pattern_list);

	std::vector<Pattern> classify(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list);

	//�Q�̃p�^�[���̋���
	double getDistance(Pattern p1, Pattern p2, double power);
	
	//�ݒ���擾
	ClassifierSetting getSetting();
};

#endif