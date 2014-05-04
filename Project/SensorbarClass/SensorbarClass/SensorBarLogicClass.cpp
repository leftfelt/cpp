#include "SensorBarLogicClass.h"

namespace SensorBarLogic{
	CongruentNode::CongruentNode(int score, int index){
			this->score = score;
			this->index = index;
	}
	bool CongruentNode::operator<(const CongruentNode &node) const{
		return this->score < node.score;
	}

	biImage getWorkImage(const biImage &image){
		biImage temp = image;
		temp.Size(
			temp.Width()*SensorBarLogic::Work_Image_Percent,
			temp.Height()*SensorBarLogic::Work_Image_Percent
		); //全体的に処理が重いので画像サイズを小さくする
		return temp;
	}
	//HSV肌色抽出
	void Range(biImage &image){
//*		
		bool H_is_in_range = false;
		bool S_is_in_range = true;
		bool V_is_in_range = true;
	
		int Hlow = 25 , Hhigh = 210;
		int Slow = 3 , Shigh = 55;
		int Vlow = 30 , Vhigh = 80;
		
		ImageUtil::HRange(image,Hlow,Hhigh,H_is_in_range);
		ImageUtil::SRange(image,Slow,Shigh,S_is_in_range);
		ImageUtil::VRange(image,Vlow,Hhigh,V_is_in_range);
		/*/
		Pixel pixel = image.Get(image.Width()/2, image.Height()/2);
		ImageUtil::costomBinalize(image, pixel, Pixel(10,100,100));
		ImageUtil::Binarize(image);
		ImageUtil::Expand(image);
		ImageUtil::Shrink(image);
		//*/
	}
	
	//*細線化処理し、特徴点を抽出する
	Hand SamplingHand(biImage &image){
		biImage temp;
		PointList point_list;
		Area area;
		Hand hand;
		
		//ゴミを取り除く
		ImageUtil::AntiNoise(image,1);
		//オブジェクトごとにラベル（番号）を付ける
		ImageUtil::Labeling(image);
		//一番面積が大きいオブジェクトだけを取り出す
		hand.area = ImageUtil::SamplingLargeObject(image);

//*		temp = image;
		//細線化して、指先らしき部分を取得する
		//ImageUtil::Thinning(image);
		//ImageUtil::BlackHat(image,1);

		point_list = ImageUtil::getNodeList(image,1);
		
		//手のひらの中心を取得
		area = ImageUtil::Scope(image);
		hand.palm_center_point.Pos(
			area.X() + area.Width() / 2,
			area.Y() + area.Height() / 2
		);
		
		//指だけを抽出する
		
		/*biImage::for_each(image,[&](int x,int y){
			if(image.Get(x,y) != Pixel(0)) return;
			Hand *hand_temp = &hand;
			std::for_each(point_list.begin(),point_list.end(),[&](Point point){
				if(point.X() == x && point.Y() == y) hand_temp->finger_point_list.push_back(Point(x,y));
			});
		});
		*/
		hand.finger_point_list = point_list;

		return hand;
	}

	//中心点を取得する
	Point getCenterPoint(PointList point_list){
		Point connected_point(0,0); //中心点

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
		
		//中心点を取得
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
		//接続点
		for(int j=0 ; j<9 ;j++){
			output.Put(connected_point.X()+j%3-1,connected_point.Y()+j/3-1,Pixel(255,0,0));
		}
	}
	
	std::vector<Feature> getOpticalFlow(biImage prev_image, biImage curr_image, PointList prev_nodes, PointList curr_nodes, int find_range){

		auto get_descripters = [](biImage &image, PointList nodes)->std::vector<std::vector<int>>{
			auto get_patch = [](biImage &image, Point node)->std::vector<int>{
				std::vector<int> patch;
				biImage::around_each(image, node,1,[&](int x, int y){
					patch.push_back(image.Get(x,y).Lightness());
				});
				return patch;
			};
			std::vector<std::vector<int>> descripters;
			std::for_each(nodes.begin(), nodes.end(), [&](Point node){
				descripters.push_back(get_patch(image, node));
			});
			return descripters;
		};

		
		auto get_feature = [](std::vector<std::vector<int>> desc1, std::vector<std::vector<int>> desc2, int threshold)->std::vector<Feature>{
			
			auto get_patch_distance = [](std::vector<int> patch)->std::vector<int>{
				double ave = Math::average(patch);
				double sigma = Math::variance(patch);

				for(int i = 0 ; i < patch.size() ; i++){
					patch[i] = (patch[i] - ave)/sigma;
				}
				return patch;
			};

			auto get_ncc = [](std::vector<int> d1, std::vector<int> d2)->float{
				int sum = 0;
				for(int i = 0 ; i < d1.size() ; i++){
					sum += (d1[i]*d2[i]);
				}
				return sum / (float)(d1.size()-1);
			};

			std::vector<std::priority_queue<CongruentNode>> matches(desc1.size());

			for(int i = 0 ; i < desc1.size(); i++){
				for(int j = 0 ; j < desc2.size(); j++){
					std::vector<int> d1 = get_patch_distance(desc1[i]);
					std::vector<int> d2 = get_patch_distance(desc2[j]);
					int ncc_value = get_ncc(d1, d2);
					if(ncc_value > threshold){
						matches[i].push(CongruentNode(ncc_value,i));
					}
				}
			}
		};

		std::vector<std::vector<int>> desc1 = get_descripters(prev_image, prev_nodes);
		std::vector<std::vector<int>> desc2 = get_descripters(curr_image, curr_nodes);

		std::vector<Feature> optical_flow;

		return optical_flow;
	}

	void drawOpticalFlow(biImage &image, std::vector<Feature> optical_flow){
		Line line;
		
		line.Color(Pixel(0,255,0));
		std::for_each(optical_flow.begin(), optical_flow.end(), [&](Feature feature){
			line.Start(feature.prev.X(),feature.prev.Y());
			line.End(feature.curr.X(),feature.curr.Y());
			line.Draw(image);
			image.Put(feature.curr.X(), feature.curr.Y(), Pixel(255,0,0));
		});
	}
}
