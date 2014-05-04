#ifdef _DEBUG
	#pragma comment(lib,"based.lib")
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"base.lib")
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
#endif

#include <image_util/ImageUtilClass.h>
#include <gui/SquareClass.h>
#include <algorithm>
#include <queue>

namespace SensorBarLogic{
	const double Work_Image_Percent = 0.50; //作業画像の比率

	//手のデータクラス
	class Hand{
		public:
			Point palm_center_point; //手のひらの中心
			PointList finger_point_list; //指の座標
			int area; //面積
	};
	class Feature{
		public:
			int angle;
			double speed;
			Point prev;
			Point curr;
	};

	class CongruentNode{
		public:
			int score;
			int index;
			CongruentNode(int score, int index);
			bool operator<(const CongruentNode &node) const;
	};


	biImage getWorkImage(const biImage &image);
	void Range(biImage &image);//HSV肌色抽出
	Hand SamplingHand(biImage &image);//*細線化処理し、特徴点を抽出する
	Point getCenterPoint(PointList point_list);//中心点を取得する
	void DrawPointList(biImage &image, Hand hand, biImage &output);

	std::vector<Feature> getOpticalFlow(biImage prev_image, biImage curr_image, PointList prev_nodes, PointList curr_nodes, int find_range);
	void drawOpticalFlow(biImage &image, std::vector<Feature> optical_flow);
	
}