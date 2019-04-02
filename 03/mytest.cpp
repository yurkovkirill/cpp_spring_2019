#include "hw3.h"
#include <iostream>

using namespace std;

int main(){

	BigInt x(3010000000000000000);
	BigInt y(7101000000000000000);
	BigInt z = x+y;
	for(int i = 0 ; i < 2; i++){
		z-=y;
	}


	cout<<"x = "<<x<<" x.size = "<<x.size<<" x.quote = "<<x.quote<<" x.sign = "<<x.sign<<" x.value1 = "<<x.value[1]<<endl;
	cout<<"y = "<<y<<" y.size = "<<y.size<<" y.quote = "<<y.quote<<" y.sign = "<<y.sign<<" y.value1 = "<<y.value[1]<<endl;
	cout<<"z = "<<z<<" z.size = "<<z.size<<" z.quote = "<<z.quote<<" z.sign = "<<z.sign<<" z.value1 = "<<z.value[1]<<endl;
	
	
	return 0;
}