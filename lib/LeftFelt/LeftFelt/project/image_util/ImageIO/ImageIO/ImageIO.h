#ifndef IMAGEIO_INCLUDE
#define IMAGEIO_INCLUDE

#include <image_util/biImageClass.h>
#include <string>
#include <iostream>
#include <fstream>

class ImageIO {
private:
	bool static LoadBitmap(biImage &image, std::string filename);
public:
	bool static Load(biImage &image, std::string filename);
	bool static LoadPPM(biImage &image, std::string filename);
	bool static Save(biImage &image, std::string filename);
};

#endif