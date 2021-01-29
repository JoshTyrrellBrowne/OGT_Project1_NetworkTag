//Client 
// Author: Josh Tyrrell Browne

#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

#include <SFML/Graphics.hpp>
//#include "Client.h"
#include "Game.h"

int main()
{
	Game m_game;
	
	//If Client create client (DO THIS IN GAME NOW)
	//Client myClient("127.0.0.1", 1111, m_game); //Create client to localhost ("127.0.0.1")
	//m_game.init(myClient); // set game to client
	
	
	


	/*if (!myClient.Connect())
	{
		std::cout << "Failed to connect to server..." << std::endl;
		system("pause");
		return 1;
	}*/
	
	std::string userInput; // string buffer to send message
	while (true)
	{
		m_game.update();
		m_game.render();
		//std::getline(std::cin, userInput); //Get line if user presses enter and fill buffer
		//if (!myClient.SendString(userInput)) //send string: userInput
		//	break; //If we failed to send string, leave this loop because we have lost connection to server
		//Sleep(10);
	}
	system("pause");
	return 0;
}