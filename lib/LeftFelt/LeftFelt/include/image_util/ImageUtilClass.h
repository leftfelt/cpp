#ifndef IMAGEUTILCLASS_INCLUDE
#define IMAGEUTILCLASS_INCLUDE

//OpenCVの依存をなくす
#include <map>
#include <image_util/biImageClass.h>
#include <base/MathClass.h>
#include <base/AreaClass.h>
#include <base/PointClass.h>
//*

class ImageUtil{
private:
	float static laplacian_filter[9];
	int static min_calc(int *p);
	int static Lookup_update(int i,int * label);
	void static filtering(biImage &image, float *filter, int filter_size[2], int direction[2]);
	float static convolution(biImage &image, float *filter, int x, int y, int filter_size[2], int direction[2]);
	float static derivativesGaussian(int x, int y, float sigma); //1回微分ガウシアン
	float static Gaussian(int x, int y, float sigma);
public:
	void static Shrink(biImage &image);//白領域を圧縮
	void static Expand(biImage &image);//白領域を膨張
	void static Opening(biImage &image, int level);//オープニング（白のノイズを除去する）
	void static Closing(biImage &image, int level);//クロージング（黒のノイズを除去する）
	void static TopHat(biImage &image,int level);//トップハット
	void static BlackHat(biImage &image,int level);//ブラックハット
	int static Thinning(biImage &image);//細線化
	void static UnSharpMasking(biImage &image,int level);//鮮鋭化
	void static LowResolution(biImage &image,int level);//低解像度化
	void static colorRegionSplit(biImage &image, int threshold = 100);//色領域分割
	void static getHistogram(biImage &image, int *histogram);
	int static getThreshold(biImage &image);//判別分析法
	void static toGrayScale(biImage &image);//グレイスケールに変換
	int static Binarize(biImage &image, int threshold = -1);//二値化
	void static Incline(biImage &image, double R,double G,double B);//傾きをかける
	void static Sobel(biImage &image);//ゾーベルフィルタ
	void static Laplacian(biImage &image);//ラプラシアンフィルタ
	void static Gaussian(biImage &image, float sigma);//ガウシアンフィルタ
	PointList static Harris(biImage &image, float sigma = 1, float threshold = 0.1);
	void static AntiNoise(biImage &image, unsigned int level = 1);//ノイズ除去
	void static Contrast(biImage &image);//コントラスト改善
	void static YIQRange(biImage &image, int Ylow, int Yhigh,int Ilow = 0, int Ihigh = 0, int Qlow = 0, int Qhigh = 0);//色抽出
	void static HSVRange(biImage &image, int Hlow, int Hhigh,int Slow = 0, int Shigh = 100, int Vlow = 0, int Vhigh = 100);//色抽出
	void static HRange(biImage &image, int low, int high, bool in_range = true); //色相が指定範囲の画素を抽出
	void static SRange(biImage &image, int low, int high, bool in_range = true); //彩度が指定範囲の画素を抽出
	void static VRange(biImage &image, int low, int high, bool in_range = true); //明度が指定範囲の画素を抽出
	void static GammaCorrection(biImage &image, double gamma);//ガンマ補正
	void static Brightness(biImage &image, int threshold=128);//明るさ調整
	Area static Scope(biImage &image);//有効範囲を取得する(255がある範囲)
	Point static TemplateMatching(biImage &image, biImage temp, int threshold = -1);//テンプレートマッチング
	Point static TemplateMatching(biImage &image, int x, int y, int width, int height, biImage temp, int threshold = -1);
	int static Matching(biImage &image, int x,int y,biImage temp);
	PointList static getNodeList(biImage &image,int connected_num);//接続数がconnected_numのノードを取得する
	int static getConnectedNum(biImage &image,int x,int y);//周囲8マスにあるPixel(255)の数を取得
	int static Labeling(biImage &image);//ラベリング 番号は1から開始
	int static SamplingObject(biImage &image, int label);//ラベル番号からオブジェクトを抽出する。ついでに面積も返す。
	int static SamplingLargeObject(biImage &image);//ラベリング済み画像から面積が最大のオブジェクトを抽出する
	double static getBulr(biImage &image); //ぼやけ度を取得する
	void static costomBinalize(biImage &image, Pixel attention, Pixel sigma); // <-- TODO:名前適当だからかえる！
	/** ImageIOに移動させる bitmapではなくてもいい ppm形式とか
	bool static Save(biImage &image, std::string filename, short bitCount = 24, int dataoffset = 54);
	bool static Load(biImage &image, std::string filename);
	*/
};

#endif