#include <DictionaryRepository.h>
#include <ClassifierClass.h>

void main(){
	//’Ç‰Á‚µ‚ÄŽæ“¾
	DictionaryRepository dictionary_repository("test.db","image");
	Dictionary dictionary;
	double dimension_num = 10;
	double min = -1;
	double max = 1;
	Pattern pattern(dimension_num);

	std::function<double()> generateRandomNumber = [&]()->double{
		return min + rand() % (int)(max - min) + (double)rand() / RAND_MAX;
	};

	for(int i = 0 ; i < 10 ; i++){
		std::generate(pattern.begin(), pattern.end(), generateRandomNumber);
		dictionary.push_back(pattern);
	}

	dictionary_repository.setup();
	dictionary_repository.add(dictionary);

	Dictionary result = dictionary_repository.find(1,10, dimension_num);

	std::cout << (dictionary.size() == result.size()) << std::endl;

	for(int i = 0 ; i < dictionary.size() && i < result.size() ; i++){
		for(int j = 0 ; j < dictionary[i].size() && j < result[i].size() ; j++){
			std::cout 
				<< (dictionary[i][j] == result[i][j]) << " : "
				<< dictionary[i][j] << " : " 
				<< result[i][j]
				<<  std::endl;
		}
	}
}