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

//分類器の設定
class ClassifierSetting{
public:
	int cluster_size; //分類するクラスタの数
	int dimension_num; //次元数
	int classification_num; //1回の分類回数
	double data_min; //データの最小数
	double data_max; //データの最大数
	double permissible_error; //直前の目的関数との差がこの値以下なら分類を終了する
};

//分類器
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

	//目的関数
	virtual double objective(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list) = 0;

	/**
	 * メンバシップ関数
	 * @return double >= 0
	 * Σ[num=0..N] getMembership(num, patternM) = 1, N=cluster_num M=pattern_num
	 */
	virtual double getMembership(int num, Pattern pattern, std::vector<Pattern> middle_list) = 0;

	/**
	 * 重み関数
	 * @return double > 0
	 */
	virtual double getWeight(Pattern pattern) = 0;

	//分類
	std::vector<Pattern> classify(std::vector<Pattern> pattern_list);

	std::vector<Pattern> classify(std::vector<Pattern> pattern_list, std::vector<Pattern> middle_list);

	//２つのパターンの距離
	double getDistance(Pattern p1, Pattern p2, double power);
	
	//設定を取得
	ClassifierSetting getSetting();
};

#endif