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
	const double Work_Image_Percent = 0.50; //��Ɖ摜�̔䗦

	//��̃f�[�^�N���X
	class Hand{
		public:
			Point palm_center_point; //��̂Ђ�̒��S
			PointList finger_point_list; //�w�̍��W
			int area; //�ʐ�
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
	void Range(biImage &image);//HSV���F���o
	Hand SamplingHand(biImage &image);//*�א����������A�����_�𒊏o����
	Point getCenterPoint(PointList point_list);//���S�_���擾����
	void DrawPointList(biImage &image, Hand hand, biImage &output);

	std::vector<Feature> getOpticalFlow(biImage prev_image, biImage curr_image, PointList prev_nodes, PointList curr_nodes, int find_range);
	void drawOpticalFlow(biImage &image, std::vector<Feature> optical_flow);
	
}