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

#include <time.h>
#include <gui/ImageClass.h>
#include <image_util/ImageUtilClass.h>
#include <algorithm>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/assign.hpp>

typedef boost::numeric::ublas::vector<double> Pattern;

//���ފ�̐ݒ�
class ClassifierSetting{
public:
	int cluster_size; //�N���X�^�̐�
	int dimension_num; //������
	int learn_num; //1��̊w�K��
	double data_min; //�f�[�^�̍ŏ���
	double data_max; //�f�[�^�̍ő吔
};

//���ފ�
class Classifier{
private:
	void initialize();
protected:
	std::vector<Pattern> middle;
	ClassifierSetting setting;
public:
	Classifier();
	Classifier(ClassifierSetting setting);

	//�ړI�֐�
	virtual double objective(std::vector<Pattern> pattern_list) = 0;

	//�����o�V�b�v�֐�
	virtual double getMembership(int num, Pattern pattern) = 0;

	//�d�݊֐�
	virtual double getWeight(Pattern pattern) = 0;

	//����
	std::vector<Pattern> classify(std::vector<Pattern> pattern_list);

	//�Q�̃p�^�[���̋���
	double getDistance(Pattern p1, Pattern p2, double power);

	//�N���X�^�̒��S
	Pattern getMiddle(int num);
};

#endif