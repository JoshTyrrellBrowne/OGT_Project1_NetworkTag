#include "Game.h"

Game::Game()
{
	m_window = new sf::RenderWindow(sf::VideoMode(4000, 2000), "SFML works!");

	//initiate player objects
	m_myPlayer.setFillColor(sf::Color::Red);
	m_myPlayer.setPosition(100, 100);
	m_myPlayer.setRadius(100);



	/*redPlayer.setFillColor(sf::Color::Red);
	redPlayer.setPosition(100,100);
	redPlayer.setRadius(100);

	bluePlayer.setFillColor(sf::Color::Blue);
	bluePlayer.setPosition(120, 120);
	bluePlayer.setRadius(100);

	greenPlayer.setFillColor(sf::Color::Green);
	greenPlayer.setPosition(80, 80);
	greenPlayer.setRadius(100);*/
}

void Game::update()
{




}

void Game::render()
{
	m_window->clear();
	m_window->draw(m_player.getCircle());
	for (auto enemy : m_enemyPlayers)
	{
		m_window->draw(enemy.getCircle());
	}
	m_window->display();
}

void Game::setUpWithID(int t_ID)
{
	m_player.setId(t_ID);
	m_player.getCircle().setRadius(100);
	switch (t_ID)	
	{
	case 0: //Player 1 (host)
		m_player.setColor(sf::Color::Green);
		m_player.setPosition(sf::Vector2f(200,200));
		break;
	case 1: //Player 2
		m_player.setColor(sf::Color::Blue);
		m_player.setPosition(sf::Vector2f(3800, 200));
		break;
	case 3: //Player 3
		m_player.setColor(sf::Color::White);
		m_player.setPosition(sf::Vector2f(2000, 1800));
		break;
	default:
		break;
	}
}

void Game::setPosition(int id, sf::Vector2f t_pos)
{
	if (m_player.getID() == id) //check if for player
	{
		m_player.setPosition(t_pos);
		return;
	}
	for (Player enemy : m_enemyPlayers) // check enemies
	{
		if (enemy.getID() == id)
		{
			enemy.setPosition(t_pos);
		}
	}
}

void Game::tagThisPlayer()
{
	m_player.tag();
	for (Player enemy : m_enemyPlayers) //untag enemies
	{
		enemy.unTag();
	}
}

void Game::tagEnemyPlayer(int t_IdToTag)
{
	m_player.unTag();
	for (Player enemy : m_enemyPlayers) //tag the tagged enemy, untag other
	{
		if (enemy.getID() == t_IdToTag)
			enemy.tag();
		else
			enemy.unTag();
	}
}
