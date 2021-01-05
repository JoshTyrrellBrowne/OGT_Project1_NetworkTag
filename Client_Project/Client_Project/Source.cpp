//Client 
// Author: Josh Tyrrell Browne

#include "Client.h"

int main()
{
	Client myClient("127.0.0.1", 1111); //Create client to localhost ("127.0.0.1")

	if (!myClient.Connect())
	{
		std::cout << "Failed to connect to server..." << std::endl;
		system("pause");
		return 1;
	}
	
	std::string userInput; // string buffer to send message
	while (true)
	{
		std::getline(std::cin, userInput); //Get line if user presses enter and fill buffer
		if (!myClient.SendString(userInput)) //send string: userInput
			break; //If we failed to send string, leave this loop because we have lost connection to server
		Sleep(10);
	}
	system("pause");
	return 0;
}