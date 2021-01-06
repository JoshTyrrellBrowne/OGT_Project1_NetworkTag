#include "Game.h"

Game::Game()
{
	m_window = new sf::RenderWindow(sf::VideoMode(200, 200), "SFML works!");

	//initiate player objects
	redPlayer.setFillColor(sf::Color::Red);
	redPlayer.setPosition(100,100);
	redPlayer.setRadius(10);

	bluePlayer.setFillColor(sf::Color::Blue);
	bluePlayer.setPosition(120, 120);
	bluePlayer.setRadius(10);

	greenPlayer.setFillColor(sf::Color::Green);
	greenPlayer.setPosition(80, 80);
	greenPlayer.setRadius(10);
}

void Game::update()
{
}

void Game::render()
{
	m_window->clear();
	m_window->draw(redPlayer);
	m_window->draw(bluePlayer);
	m_window->draw(greenPlayer);
	m_window->display();
}
