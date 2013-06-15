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

//分類器の設定
class ClassifierSetting{
public:
	int cluster_size; //クラスタの数
	int dimension_num; //次元数
	int learn_num; //1回の学習回数
	double data_min; //データの最小数
	double data_max; //データの最大数
};

//分類器
class Classifier{
private:
	void initialize();
protected:
	std::vector<Pattern> middle;
	ClassifierSetting setting;
public:
	Classifier();
	Classifier(ClassifierSetting setting);

	//目的関数
	virtual double objective(std::vector<Pattern> pattern_list) = 0;

	//メンバシップ関数
	virtual double getMembership(int num, Pattern pattern) = 0;

	//重み関数
	virtual double getWeight(Pattern pattern) = 0;

	//分類
	std::vector<Pattern> classify(std::vector<Pattern> pattern_list);

	//２つのパターンの距離
	double getDistance(Pattern p1, Pattern p2, double power);

	//クラスタの中心
	Pattern getMiddle(int num);
};

#endif