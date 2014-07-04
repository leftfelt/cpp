#ifdef _DEBUG
	#pragma comment(lib,"guid.lib")
	#pragma comment(lib,"image_utild.lib")
	#pragma comment(lib,"testamentd.lib")
#else
	#pragma comment(lib,"gui.lib")
	#pragma comment(lib,"image_util.lib")
	#pragma comment(lib,"testament.lib")
#endif

#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>
#include <gui/Application.h>
#include <image_util/ImageUtilClass.h>
#include <Testament.h>
#include <Converter.h>
#include <ClusterRepository.h>
#include <RelationDataRepository.h>
#include <fuzzyCMeansClassifierClass.h>
#include <KMeansClassifierClass.h>

class Ukbench : public Application {
private:
	int file_num;
	String filename;
	biImage image;
	int count;
	Testament *testament;
	TestamentSetting setting;
	ClassifierSetting classifier_setting;
public:
	Ukbench(){
		filename = "ukbench";
		file_num = 10200;
		//���ފ�ݒ�
		classifier_setting.cluster_size = 8;
		classifier_setting.dimension_num = 64;
		classifier_setting.classification_num = 30;
		classifier_setting.permissible_error = 0.001;
		classifier_setting.data_min = -0.1;
		classifier_setting.data_max = 0.1;
		//�ϊ���ݒ�
		setting.converter_setting.convert_image_threshold = 2000;
		setting.converter_setting.sampling_rate = 100;
		//�w�K���u�ݒ�
		setting.learn_num = 8; //1��̊w�K�Ŏg���p�^�[���̐�
		setting.equivocation = 99;
		setting.relation_range = 24; //���̓f�[�^�Ƃ̊֘A���������擾���邩
		setting.data_type = "image";
		setting.db_name = "knowledge.db";
		testament = new Testament(setting, new KMeansClassifier(classifier_setting));
	}

	void main(){
			/*
			this->testament->truncate();
			this->learn(0, 16);
			//*/
			this->bench(0, 16);
			//*/ 

	}

	void bench(int start, int end){
		//�摜�ǂݍ���
		std::vector<int> score_list;
		for(int i = start ; i < end ; i++){
			this->getImage(i);

			std::vector<std::string> results = this->testament->search(this->image,4);
			
			if(results.empty()){
				score_list.push_back(0);
			}else{
				//�X�R�A�̌v�Z
				//���x����4�Ŋ������l�ƃC���f�b�N�X��4�Ŋ������l�����������𐔂���
				score_list.push_back(this->getScore(i, results));
				std::for_each(results.begin(), results.end(), [&](std::string str){
					std::cout << str << " ,";
				});
			}
			std::cout << std::endl << this->getFilename(i) << " score finished" << std::endl;
		}

		int sum = 0;
		std::cout << "Score : ";
		std::for_each(score_list.begin(), score_list.end(), [&](int score){
			std::cout << score << ", ";
			sum += score;
		});
		std::cout << std::endl << "Average : " << ((double)sum/score_list.size()) << std::endl;
	}
	
	//�X�R�A�̌v�Z
	//���x����4�Ŋ������l�ƃC���f�b�N�X��4�Ŋ������l���������𐔂���
	int getScore(int current_index, std::vector<std::string> score_list){
		int score = 0;
		std::for_each(score_list.begin(), score_list.end(), [&](std::string data){
			int label;
			std::istringstream iss;
			iss.str(data);
			iss >> label;
			if((int)(label/4) == (int)(current_index/4)) score++;
		});
		return score;
	}	

	void learn(int start, int end){
		for(int i = start ; i < end ; i++){
			std::ostringstream ostr;
			ostr << i;
			this->getImage(i);
			this->testament->toLink(this->image, ostr.str());
			this->testament->learn();

			std::cout << this->getFilename(i) << " learn finished" << std::endl;
		}
	}



	//�t�@�C�����擾
	std::string getFilename(int num){
		std::ostringstream ostr;

		ostr << "../../../data/ukbench/bmp/" << this->filename << std::setw(5) << std::setfill('0') << num << ".bmp";

		return ostr.str();
	}

	//���̉摜��
	void getImage(int num){
		image.Delete();
		ImageUtil::Load(this->image,this->getFilename(num));
	}

};

void main(){
	Ukbench ukbench;
	ukbench.main();
	
	getchar();
}