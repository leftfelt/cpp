#include <ClassifierClass.h>
#include <Converter.h>
#include <fuzzyCMeansClassifierClass.h>
#include <KMeansClassifierClass.h>
#include <time.h>

void main(){
	int pattern_num = 100;
	ClassifierSetting setting;
	setting.classification_num = 10; //•ª—Ş‰ñ”
	setting.permissible_error = 0.01;
	setting.cluster_size = 10; //•ª—Ş”
	setting.data_max = pattern_num + pattern_num;
	setting.data_min = 0;
	setting.dimension_num = 2;

	Classifier *classifier = new KMeansClassifier(setting);
	std::vector<Pattern> pattern_list;
	pattern_list.resize(pattern_num);
	int count = 0;
	
	std::generate(pattern_list.begin(), pattern_list.end(), [&]()->Pattern{
		Pattern pattern(setting.dimension_num);
		for(int i = 0 ; i < (signed)pattern.size() ; i++){
			pattern[i] = count + count;
		}
		count++;
		return pattern;
	});

	Dictionary dictionary = classifier->classify(pattern_list);

	std::for_each(dictionary.begin(), dictionary.end(),[](Pattern pattern){
		std::cout << "[ ";
		for(int i = 0 ; i < pattern.size() ; i++){
			std::cout << pattern[i] << " ";
		}
		std::cout << "]" << std::endl;
	});
	
	std::cout << "==================================================" << std::endl;

	Pattern pattern(setting.dimension_num);
	pattern_list.clear();
	pattern[0] = pattern_num * 3;
	pattern[1] = pattern_num * 3;
	pattern_list.push_back(pattern);
	pattern[0] = pattern_num * 4;
	pattern[1] = pattern_num * 4;
	pattern_list.push_back(pattern);
	pattern[0] = pattern_num * 5;
	pattern[1] = pattern_num * 5;
	pattern_list.push_back(pattern);
	
	dictionary = classifier->classify(pattern_list, dictionary);

	std::for_each(dictionary.begin(), dictionary.end(),[](Pattern pattern){
		std::cout << "[ ";
		for(int i = 0 ; i < pattern.size() ; i++){
			std::cout << pattern[i] << " ";
		}
		std::cout << "]" << std::endl;
	});
	

	getchar();
}