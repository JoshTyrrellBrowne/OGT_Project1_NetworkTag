#include "Player.h"

Player::Player()
{
	isTagged = false;
	m_circle.setRadius(200);
	m_circle.setFillColor(sf::Color::White); //initial color
	m_circle.setOrigin(sf::Vector2f(m_circle.getGlobalBounds().width / 2, m_circle.getGlobalBounds().height / 2));
}

void Player::setPosition(sf::Vector2f t_pos)
{
	m_circle.setPosition(t_pos);
}

void Player::tag()
{
	isTagged = true;
	m_circle.setFillColor(sf::Color::Red); //set to Tag color
}

void Player::unTag()
{
	isTagged = false;
	m_circle.setFillColor(sf::Color::White); //set back to real color
}
