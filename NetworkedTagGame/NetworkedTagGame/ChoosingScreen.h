#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class ChoosingScreen
{
public:
	ChoosingScreen(sf::RenderWindow& t_rendWind);
	int update();
	void render();

private:
	sf::RenderWindow* m_rendWindow;
	sf::Event event;
	sf::Font m_font;
	sf::Text m_Title;
	sf::RectangleShape m_hostButton;
	sf::RectangleShape m_joinButton;
	sf::Text m_hostText;
	sf::Text m_joinText;
};