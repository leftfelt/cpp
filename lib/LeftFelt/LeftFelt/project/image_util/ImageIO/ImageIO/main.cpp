#include <iostream>

#include "ImageIO.h"

void main() {
	biImage image;

	ImageIO::LoadPPM(image, "lenna3.ppm");
	ImageIO::Save(image, "lenna4.ppm");
}