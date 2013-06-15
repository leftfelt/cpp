#include <iostream>
#include "TimeClass.h"

using namespace std;

void main(){
	Time time;

	time.Start();

	for(int i = 0 ; i < 50000000 ; i++);

	cout << time.End() << "[ms]" << endl;

}