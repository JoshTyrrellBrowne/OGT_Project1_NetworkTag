#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
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
	void tagThisPlayer();
	void tagEnemyPlayer(int t_IdToTag);
	void HandleControls();
	static void ListenForNewConnectionThread(Server& t_server);

private:
	GameState m_gameState;
	PlayerType m_playerType;
	ChoosingScreen* m_chooseScreen;
	Client* m_clientPtr;
	Server* m_serverPtr;
	int m_myPlayerID;
	std::vector<Player> m_allPlayers;

	sf::Event event;
	sf::RenderWindow* m_window;
	int m_playerID; //player ID, (ie first client to join server m_playerID=1, second m_playerID=2)
};