#include "IStartServer.h"
#include "Head.h"
//#include "../Share/Common.h"

IStartServer   *gIStartServer=NULL;

extern int  ShutDownNet();

extern IStartServer*  StartServerPtr();

/*static int disable_signal (int sigmin, int sigmax)
{
	sigset_t signal_set;
	if (sigemptyset (&signal_set) == - 1)
	{
		assert(0);
	}
	for (int i = sigmin; i <= sigmax; i++)
	{
		sigaddset (&signal_set, i);
	}

	if (pthread_sigmask (SIG_BLOCK, &signal_set, 0) != 0)
	{
		assert(0);
	}
	return 1;
}*/

void ConsoleProc()
{
	g_cond.Wait();	
}

int main (int argc, const char *argv[])
{
	gIStartServer=StartServerPtr();
	assert(gIStartServer);
	gIStartServer->Run();

	return 0;
}

