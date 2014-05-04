#include <stdio.h>
#include <image_util/PixelClass.h>

void main(){
	Pixel<> a,b,c,d;

	a.setRGB(100,150,200);
	b.setRGB(10,20,30);

	d = a + b;

	printf("R:%d G:%d B:%d\n",d.Red(),d.Green(),d.Blue());

	c = 2;
	d = a / c;
	printf("R:%d G:%d B:%d\n",d.Red(),d.Green(),d.Blue());

}