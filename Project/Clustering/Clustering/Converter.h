#ifndef INCLUDE_CONVERTER
#define INCLUDE_CONVERTER

#ifdef _DEBUG
	#pragma comment(lib,"image_utild.lib")
#else
	#pragma comment(lib,"image_util.lib")
#endif

#include <image_util/ImageUtilClass.h>
#include "ClassifierClass.h"

//�Ή��������f�[�^�`���̓��ꉻ�����N���X���`���Ă���
//�ǂ������x�N�g���ň��������L�q����
template<typename T>
class Converter{
public:
	std::vector<double> convert(T data);
};

template<> 
class Converter<biImage>{
public:
	std::vector<double> convert(biImage &image);
};

template<>
class Converter<std::vector<float>>{
public:
	Pattern convert(std::vector<float> data);
};

template<>
class Converter<Pattern>{
public:
	std::vector<double> convert(Pattern pattern);
};

#endif