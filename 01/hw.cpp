#include "numbers.dat"
#include <cstdlib>
#include <iostream>
#include <cmath>
// Data - массив
// Size - размер массива

using namespace std;

int findindex(int& a,int& b,const int mas[],int size){//a=index(a),b=index(b)
	int flag=0;
	int i;
	for(i = 0;i<size;i++){
			if(mas[i]==a){
				flag = 1;
				a = i;
				break;
			}
		}
	if(flag==0){
		cout<<0<<endl;
		return 0;
	}
	flag = 0;
	for(i = 0;i<size;i++){
		if(mas[i]==b){
			flag = 1;
		}
		if((flag==1)&&(mas[i]>b)){
			b = i-1;
			break;
		}
	}
	if(flag==0){
		cout<<0<<endl;
		return 0;
	}
	return 1;
}
bool Issimple(int num){
	if(num==1)
		return false;
	int Check=floor(sqrt(num));
			for(int i = 2; i<=Check; i++){
				if (num%i==0){
				return false;
				}
			}
	return true;
}
int countSimple(int a, int b){
	int count = 0;
	findindex(a,b,Data,Size);
	int num;
	count = 0;
	bool IsSimple = true;
	for(int j = a;j<=b;j++){
		num = Data[j];
		IsSimple = true;
		IsSimple = Issimple(num);
		if (IsSimple){
			count++;
		}
	}
	return count;
}
int main(int argc, char* argv[])
{
	if(((argc-1)%2!=0)||(argc==1)){
		return -1;
	}
	int counter = 0; 
	int beg;
	int end;
	int M = (argc-1)/2; 
	for(int i = 0;i<M;i++){
		beg = atoi(argv[1+2*i]); 
		end = atoi(argv[2+2*i]);
		counter = countSimple(beg,end);
		//std::cout<<"count of simple "<<counter<<std::endl;
		std::cout<<counter<<std::endl;
	}
	return 0;
}
