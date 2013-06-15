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