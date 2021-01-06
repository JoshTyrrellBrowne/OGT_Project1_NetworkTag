#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Game
{
public:
	Game();
	//Game::Game~();

	void update(); //where game components are updated
	void render(); //where we draw 
private:
	sf::CircleShape redPlayer;
	sf::CircleShape bluePlayer;
	sf::CircleShape greenPlayer;

	sf::RenderWindow* m_window;
};