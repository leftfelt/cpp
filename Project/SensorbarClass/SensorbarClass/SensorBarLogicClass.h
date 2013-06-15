#ifdef _DEBUG
	#pragma comment(lib,"Based.lib")
	#pragma comment(lib,"Flibd.lib")
	#pragma comment(lib,"Flib_wind.lib")
#else
	#pragma comment(lib,"Base.lib")
	#pragma comment(lib,"Flib.lib")
	#pragma comment(lib,"Flib_win.lib")
#endif

#include <ImageUtilClass.h>
#include <SquareClass.h>
#include <algorithm>

namespace SensorBarLogic{
	const double Work_Image_Percent = 0.5; //作業画像の比率

	//手のデータクラス
	class Hand{
		public:
			Point palm_center_point; //手のひらの中心
			PointList finger_point_list; //指の座標
			int area; //面積
	};
	class Feature{
		public:
			double bulr; //ぼやけ度
			double area; //面積比
			int point_num; //特徴点の数
			PointList point_list;
			//一致率を取得する
			double getMatchRate(Feature feature);
			//類似度を取得する
			Feature operator*(Feature feature);
	};

	void Range(biImage &image);//HSV肌色抽出
	Hand SamplingHand(biImage &image);//*細線化処理し、特徴点を抽出する
	Point getCenterPoint(PointList point_list);//中心点を取得する
	void DrawPointList(biImage &image, Hand hand, biImage &output);
}