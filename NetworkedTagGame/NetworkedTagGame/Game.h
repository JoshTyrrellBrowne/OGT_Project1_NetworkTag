#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include "Player.h"
#include "Client.h"
#include "Server.h"
#include "PacketStructs.h"
#include "Enums.h"
#include "ChoosingScreen.h"

class Game
{
public:
	Game();
	void init(Client& t_client);
	void update(); //where game components are updated
	void render(); //where we draw 
	void setUpWithID(int t_ID); //this sets up player variables based on the passed ID
	void setPosition(int t_id, sf::Vector2f t_pos);
	void tagPlayer(int idToTag);
	void HandleControls();
	void HandleCollisions();
	static void ListenForNewConnectionThread(Server& t_server);
	void BoundaryCheck();
	void reset(); //reset for new game

private:
	GameState m_gameState;
	PlayerType m_playerType;
	ChoosingScreen* m_chooseScreen;
	Client* m_clientPtr;
	Server* m_serverPtr;
	int m_myPlayerID;
	sf::Font font;
	sf::Text m_playerText;
	sf::Text m_TagText; //text to display when tag occurs
	float m_elapsedTime;
	sf::Clock m_clock;
	std::vector<Player> m_allPlayers;
	bool isFirstTag;

	sf::Event event;
	sf::RenderWindow* m_window;
	int m_playerID; //player ID, (ie first client to join server m_playerID=1, second m_playerID=2)
};