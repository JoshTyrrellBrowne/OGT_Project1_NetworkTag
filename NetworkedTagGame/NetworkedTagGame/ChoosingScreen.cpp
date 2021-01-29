#include "ChoosingScreen.h"

ChoosingScreen::ChoosingScreen(sf::RenderWindow& t_rendWind)
{
	m_rendWindow = &t_rendWind;

	//Load Font
	m_font.loadFromFile("Lato-Regular.ttf");
	/*{
		std::cout << "Failed to load font" << std::endl;
	}*/
	//Textual data setup
	m_Title.setFont(m_font);
	m_Title.setString("Host a game? Join a game? Your choice, choose wisely.");
	m_Title.setCharacterSize(50);
	m_Title.setPosition(sf::Vector2f(1500, 200));
	m_Title.setOrigin(m_Title.getGlobalBounds().width / 2, m_Title.getGlobalBounds().height / 2);

	//SetupObjects
	m_hostButton.setPosition(sf::Vector2f(1100, 1000));
	m_hostButton.setSize(sf::Vector2f(400, 200));
	m_hostButton.setFillColor(sf::Color::Blue);
	m_hostButton.setOrigin(m_hostButton.getGlobalBounds().width / 2, m_hostButton.getGlobalBounds().height / 2);
	m_hostText.setFont(m_font);
	m_hostText.setString("Host");
	m_hostText.setCharacterSize(30);
	m_hostText.setPosition(m_hostButton.getPosition());
	m_hostText.setOrigin(m_hostText.getGlobalBounds().width / 2, m_hostText.getGlobalBounds().height / 2);

	m_joinButton.setPosition(sf::Vector2f(1900, 1000));
	m_joinButton.setSize(sf::Vector2f(400, 200));
	m_joinButton.setFillColor(sf::Color::Blue);
	m_joinButton.setOrigin(m_joinButton.getGlobalBounds().width / 2, m_joinButton.getGlobalBounds().height / 2);
	m_joinText.setFont(m_font);
	m_joinText.setString("Join");
	m_joinText.setCharacterSize(30);
	m_joinText.setPosition(m_joinButton.getPosition());
	m_joinText.setOrigin(m_joinText.getGlobalBounds().width / 2, m_joinText.getGlobalBounds().height / 2);
}

int ChoosingScreen::update()
{
	while (m_rendWindow->pollEvent(event))
	{
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				//if click inside host button
				if (m_hostButton.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*m_rendWindow)))
				{
					return 1;
				}
				//if click inside join button
				if (m_joinButton.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*m_rendWindow)))
				{
					return 2;
				}
			}
		}
	}
	return 0;
}

void ChoosingScreen::render()
{
	m_rendWindow->draw(m_Title);
	m_rendWindow->draw(m_hostButton);
	m_rendWindow->draw(m_joinButton);
	m_rendWindow->draw(m_hostText);
	m_rendWindow->draw(m_joinText);
}
