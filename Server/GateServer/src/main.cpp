#include "GateServer.h"
int main()
{
	CGateServer  theApp ;
	theApp.Init() ;
	theApp.RunLoop() ;
}