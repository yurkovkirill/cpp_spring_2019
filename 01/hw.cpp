#include "numbers.dat"
#include  <cstdlib>
#include <iostream>
#include <cmath>
// Data - массив
// Size - размер массива

using namespace std;

int main(int argc, char* argv[])
{
	if(((argc-1)%2!=0)||(argc==1)){
		return -1;
	}
	int i;
    int k=0;    
    int a;  
    int b;
    int g;
    int M = (argc-1)/2; 
    for(g=0;g<M;g++){
	    a = atoi(argv[1+2*g]);   
	    b = atoi(argv[2+2*g]);
	    for(i=0;i<Size;i++){
	    	if(Data[i]==a){
	    		k=1;
	    		a=i;
	    		break;
	    	}
	    }
	    if(k==0){
	    	cout<<0<<endl;
	    	return 0;
	    }
	    k=0;
	    for(i=0;i<Size;i++){
	    	if(Data[i]==b){
	    		k=1;
	    		
	    	}
	    	if((k==1)&&(Data[i]>b)){
	    		b=i-1;
	    		break;
	    	}
	    }
	    if(k==0){
	    	cout<<0<<endl;
	    	return 0;
	    }
	    

	    int n;
	    k=0;
	    int Check;
	    bool IsSimple=true;
	    for(int j=a;j<=b;j++){
	    	n=Data[j];
	    	if(n==1)
	    		continue;
	    	IsSimple=true;
	    	Check=floor(sqrt(n));
		    for(i=2; i<=Check; i++){
		        if (n%i==0){
		        IsSimple=false;
		        break;
		        }
		    }
		    if (IsSimple){
		    	k++;
		    }
	    }
	    //std::cout<<"count of simple "<<k<<std::endl;
	    std::cout<<k<<std::endl;
    }
    return 0;
}
