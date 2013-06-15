#ifndef INCLUDE_CONVERTER
#define INCLUDE_CONVERTER

#ifdef _DEBUG
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"image_util.lib")
#endif

#include <image_util/ImageUtilClass.h>
#include <ClassifierClass.h>

//Converterの設定
class ConverterSetting{
public:
	int convert_image_threshold;
	int sampling_rate;
};

//Converterの基底クラス　TODO：名前微妙
class ConverterBase{
protected:
	ConverterSetting setting;
public:
	ConverterBase();
	void setSetting(ConverterSetting setting);
	template<typename T> std::vector<T> sampling(std::vector<T> data_list){
		if(this->setting.sampling_rate > 100 || this->setting.sampling_rate < 0) throw std::exception("sampling_rate is invalid");
		std::vector<T> result;
		int size = (int)(data_list.size() * this->setting.sampling_rate / 100.0);
		int rate = data_list.size() / size;
		for(int i = 0 ; i < (signed)data_list.size() ; i+=rate){
			result.push_back(data_list[i]);
		}
		return result;
	}
};

//対応したいデータ形式の特殊化したクラスを定義していく
//どういうベクトルで扱うかを記述する
template<typename T>
class Converter{
public:
	std::vector<double> convert(T data);
};

template<>
class Converter<biImage> : public ConverterBase{
public:
	std::vector<double> convert(biImage &image);
};

template<>
class Converter<std::vector<float>> : public ConverterBase{
public:
	Pattern convert(std::vector<float> data);
};

template<>
class Converter<std::vector<double>> : public ConverterBase{
public:
	Pattern convert(std::vector<double> data);
};

template<>
class Converter<Pattern> : public ConverterBase{
public:
	std::vector<double> convert(Pattern pattern);
};


//TODO:これStringUtlかなんか作って移動する
template<>
class Converter<std::string> : public ConverterBase{
public:
	int convert(std::string data);
};

#endif