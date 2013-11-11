#include <iostream>
#include <string>
#include <base/MathClass.h>

void main(){

	std::string bar = "=================================";
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

	std::cout << Math::distance(Point(0,0), Point(1,1)) << std::endl;

	v1.clear();
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	v1.push_back(4);
	v1.push_back(5);

	std::cout << Math::average(v1) << std::endl;
	std::cout << Math::variance(v1) << std::endl;
	
    std::random_device rd;
	std::mt19937 rng(rd());

	std::cout << bar << std::endl;

	for(int i=0 ; i<10 ; i++){
		std::cout << std::generate_canonical<double, 10>(rng) << std::endl;
	}
	
	std::cout << bar << std::endl;

	for(int i=0 ; i<10 ; i++){
		std::cout << Math::generateUniform(rng) << std::endl;
	}

	std::cout << bar << std::endl;

	for(int i=0 ; i<10 ; i++){
		std::cout << Math::generateRandomNumber(-1, 1, rng) / pow(10.0, Math::generateRandomNumber(0, 6, rng)) << std::endl;
	}

	getchar();
}