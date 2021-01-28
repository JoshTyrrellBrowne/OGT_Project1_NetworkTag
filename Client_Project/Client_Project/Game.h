#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"

class Game
{
public:
	Game();

	void update(); //where game components are updated
	void render(); //where we draw 
	void setUpWithID(int t_ID); //this sets up player variables based on the passed ID
	void setPosition(int t_id, sf::Vector2f t_pos);
	void tagThisPlayer();
	void tagEnemyPlayer(int t_IdToTag);

private:
	Player m_player;
	std::vector<Player> m_enemyPlayers;

	sf::RenderWindow* m_window;
	int m_playerID; //player ID, (ie first client to join server m_playerID=1, second m_playerID=2)
};