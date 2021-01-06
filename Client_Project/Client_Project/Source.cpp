//Client 
// Author: Josh Tyrrell Browne

#include "Client.h"
#include "Game.h"
#include <SFML/Graphics.hpp>

int main()
{
	Client myClient("127.0.0.1", 1111); //Create client to localhost ("127.0.0.1")
	Game m_game;


	if (!myClient.Connect())
	{
		std::cout << "Failed to connect to server..." << std::endl;
		system("pause");
		return 1;
	}
	
	std::string userInput; // string buffer to send message
	while (true)
	{
		m_game.update();
		m_game.render();
		std::getline(std::cin, userInput); //Get line if user presses enter and fill buffer
		if (!myClient.SendString(userInput)) //send string: userInput
			break; //If we failed to send string, leave this loop because we have lost connection to server
		Sleep(10);
		
	}
	system("pause");
	return 0;
}