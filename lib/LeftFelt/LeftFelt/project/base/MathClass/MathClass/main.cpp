#include <iostream>
#include <base/MathClass.h>

void main(){
	int a = 4;
	int b = 0;
	double c;
	c = Math::aSin(Math::Sin(90));

	std::vector<double> v1;
	std::vector<double> v2;
	v1.push_back(0);
	v1.push_back(0);
	v2.push_back(1);
	v2.push_back(1);

	std::cout << Math::eucDistance(v1,v2) << std::endl;

	v1.clear();v2.clear();
	v1.push_back(0.789);
	v1.push_back(0.515);
	v1.push_back(0.335);
	v1.push_back(0);
	v2.push_back(0.832);
	v2.push_back(0.555);
	v2.push_back(0);
	v2.push_back(0);

	std::cout << Math::cosSimilarity(v1,v2) << std::endl;

	getchar();
}