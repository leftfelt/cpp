
#include<stdio.h>
#include<base/NumberClass.h>
//*
#include<malloc.h>
#include<string>


Number func(Number num){
	return num;
}



void main(){

	Number a, b, c;
	double d;

	a = 2.5;
	b = 10;
	
	c = a + b;
	d << c;
	printf("%f\n",d);

	c = a - b;
	d << c;
	printf("%f\n",d);

	c = a * b;
	d << c;
	printf("%f\n",d);

	c = a / b;
	d << c;
	printf("%f\n",d);

	
	c = 2 + b;
	d << c;
	printf("%f\n",d);

	c = 2 - b;
	d << c;
	printf("%f\n",d);

	c = 2 * b;
	d << c;
	printf("%f\n",d);

	c = 2 / b;
	d << c;
	printf("%f\n",d);

	c = a + 2;
	d << c;
	printf("%f\n",d);

	c = a - 2;
	d << c;
	printf("%f\n",d);

	c = a * 2;
	d << c;
	printf("%f\n",d);

	c = a / 2;
	d << c;
	printf("%f\n",d);

	func(1);
}
