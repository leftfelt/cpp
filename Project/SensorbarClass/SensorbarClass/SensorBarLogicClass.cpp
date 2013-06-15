#include "SensorBarLogicClass.h"

namespace SensorBarLogic{
	Feature Feature::operator*(Feature feature){
		Feature result;
		result.bulr = this->bulr * feature.bulr;
		result.area = this->area * feature.area;
		result.point_num = this->point_num * feature.point_num;

		return result;
	}
	//��v�����擾����
	double Feature::getMatchRate(Feature feature){
		Feature xy_feature = *this * feature;
		Feature xx_feature = *this * *this;
		Feature yy_feature = feature * feature;

		double xy = xy_feature.bulr + xy_feature.area + xy_feature.point_num;
		double xx = xx_feature.bulr + xx_feature.area + xx_feature.point_num;
		double yy = yy_feature.bulr + yy_feature.area + yy_feature.point_num;

		//�R�T�C���ގ��x
		return ( xy ) / ( sqrt(xx) * sqrt(yy) );
	}

	//HSV���F���o
	void Range(biImage &image){
		bool H_is_in_range = false;
		bool S_is_in_range = true;
		bool V_is_in_range = true;
	
		int Hlow = 25 , Hhigh = 210;
		int Slow = 3 , Shigh = 55;
		int Vlow = 30 , Vhigh = 80;
		
		ImageUtil::HRange(image,Hlow,Hhigh,H_is_in_range);
		ImageUtil::SRange(image,Slow,Shigh,S_is_in_range);
		ImageUtil::VRange(image,Vlow,Hhigh,V_is_in_range);
		ImageUtil::Binarize(image,1);
	}
	
	//*�א����������A�����_�𒊏o����
	Hand SamplingHand(biImage &image){
		biImage temp;
		PointList point_list;
		Area area;
		Hand hand;
		
		//�S�~����菜��
		ImageUtil::AntiNoise(image,1);
		//�I�u�W�F�N�g���ƂɃ��x���i�ԍ��j��t����
		ImageUtil::Labeling(image);
		//��Ԗʐς��傫���I�u�W�F�N�g���������o��
		hand.area = ImageUtil::SamplingLargeObject(image);
		temp = image;
		//���x���g��k�����J��Ԃ��āA��̂Ђ�̕������擾
		ImageUtil::AntiNoise(temp,5);
		//�א������āA�w��炵���������擾����
		ImageUtil::Thinning(image);
		point_list = ImageUtil::getNodeList(image,1);
		
		//��̂Ђ�̒��S���擾
		area = ImageUtil::Scope(temp);
		hand.palm_center_point.Pos(
			area.X() + area.Width() / 2,
			area.Y() + area.Height() / 2
		);
		
		//�w�����𒊏o����
		biImage::for_each(temp,[&](int x,int y){
			if(temp.Get(x,y) != Pixel(0)) return;
			Hand *hand_temp = &hand;
			std::for_each(point_list.begin(),point_list.end(),[&](Point point){
				if(point.X() == x && point.Y() == y) hand_temp->finger_point_list.push_back(Point(x,y));
			});
		});

		return hand;
	}

	//���S�_���擾����
	Point getCenterPoint(PointList point_list){
		Point connected_point(0,0); //���S�_

		std::for_each( point_list.begin(), point_list.end(),[&](Point point){
			connected_point.Pos(
				(point.X()+connected_point.X()),
				(point.Y()+connected_point.Y())
			);
		});

		if(point_list.size() != 0)
			connected_point = connected_point / (point_list.size());
		return connected_point;
	}

	void DrawPointList(biImage &image, Hand hand, biImage &output){
		Point connected_point = hand.palm_center_point;
		PointList point_list = hand.finger_point_list;
		Line line;
		
		//���S�_���擾
		connected_point *= (1.0/Work_Image_Percent);

		line.Color(Pixel(0,255,0));
		line.Start(connected_point.X(),connected_point.Y());
		std::for_each( point_list.begin(), point_list.end(),[&](Point point){
			point *= (1.0/Work_Image_Percent);
			line.End(point.X(),point.Y());
			line.Draw(output);
			for(int j=0 ; j<9 ;j++){
				output.Put(point.X()+j%3-1,point.Y()+j/3-1,Pixel(0,0,255));
			}
		});
		//�ڑ��_
		for(int j=0 ; j<9 ;j++){
			output.Put(connected_point.X()+j%3-1,connected_point.Y()+j/3-1,Pixel(255,0,0));
		}
	}
}
