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

namespace SensorBarLogic{
	const double Work_Image_Percent = 0.5; //��Ɖ摜�̔䗦

	//��̃f�[�^�N���X
	class Hand{
		public:
			Point palm_center_point; //��̂Ђ�̒��S
			PointList finger_point_list; //�w�̍��W
			int area; //�ʐ�
	};
	class Feature{
		public:
			double bulr; //�ڂ₯�x
			double area; //�ʐϔ�
			int point_num; //�����_�̐�
			PointList point_list;
			//��v�����擾����
			double getMatchRate(Feature feature);
			//�ގ��x���擾����
			Feature operator*(Feature feature);
	};

	void Range(biImage &image);//HSV���F���o
	Hand SamplingHand(biImage &image);//*�א����������A�����_�𒊏o����
	Point getCenterPoint(PointList point_list);//���S�_���擾����
	void DrawPointList(biImage &image, Hand hand, biImage &output);
}