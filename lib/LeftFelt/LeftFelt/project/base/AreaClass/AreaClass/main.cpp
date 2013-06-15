#include <stdio.h>
#include <base/AreaClass.h>

void main(){
	Area a1(0,0,100,100);
	Area a2(0,99,100,100);

	if(a1.HitCheck(a2) == true){
		printf("“–‚½‚è");
	}else{
		printf("‚Í‚¸‚ê");
	}

	getchar();

	return;
}