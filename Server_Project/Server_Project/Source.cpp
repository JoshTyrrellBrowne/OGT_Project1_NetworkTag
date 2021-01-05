// Server
// Author: Josh Tyrrell Browne

#include "Server.h"




int main()
{
	Server myServer(1111);
	for (int i = 0; i < 100; i++)
	{
		myServer.ListenForNewConnection(); //Accept new connections (if someone trying to connect)
	}


	system("pause");
	return 0;
}