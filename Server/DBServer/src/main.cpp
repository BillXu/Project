#include"main.h"
#include <iostream>
#include "ThreadMod.h"
class A
	:public CThreadT
{
public:
	void __run()
	{
		printf("this function \n") ;
	}
};

int main()
{
	A a ;
	a.Start() ;
	getchar();
	return 0 ; 
}