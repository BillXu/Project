#include "main.h"
#include "LoginApp.h"
int main()
{
	CLoginApp theApp ;
	theApp.Init();
	theApp.MainLoop() ;
	return 0 ;
}