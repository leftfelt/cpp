#include <Converter.h>

ConverterBase::ConverterBase(){
	this->setting.convert_image_threshold = 500;
	this->setting.sampling_rate = 100;
}

void ConverterBase::setSetting(ConverterSetting setting){
	this->setting = setting;
}

template<typename T> std::vector<double> Converter<T>::convert(T data){
	throw std::exception("‚±‚ÌŒ^‚Í•ÏŠ·o—ˆ‚Ü‚¹‚ñB");
	return NULL;
};

std::vector<double> Converter<biImage>::convert(biImage &image){
	std::vector<double> data_list;
	std::vector<SurfFeature> surf_list;
	surf_list = ImageUtil::getSurf(image,this->setting.convert_image_threshold);
	surf_list = this->sampling(surf_list);
	std::for_each(surf_list.begin(), surf_list.end(), [&](SurfFeature surf){
		for(int i = 0 ; i < (signed)surf.features.size() ; i++){
			data_list.push_back(surf.features.at(i));
		}
	});
	return data_list;
}

Pattern Converter<std::vector<float>>::convert(std::vector<float> data){
	Pattern pattern(data.size());

	for(int i = 0 ; i < (signed)data.size() ; i++) pattern[i] = data[i];

	return pattern;
}

Pattern Converter<std::vector<double>>::convert(std::vector<double> data){
	Pattern pattern(data.size());

	for(int i = 0 ; i < (signed)data.size() ; i++) pattern[i] = data[i];

	return pattern;
}


std::vector<double> Converter<Pattern>::convert(Pattern pattern){
	std::vector<double> data_list;

	std::for_each(pattern.begin(),pattern.end(), [&](double value){data_list.push_back(value);});

	return data_list;
}


int Converter<std::string>::convert(std::string data){
	int num;
	std::istringstream iss;
	iss.str(data);
	iss >> num;
	return num;
}