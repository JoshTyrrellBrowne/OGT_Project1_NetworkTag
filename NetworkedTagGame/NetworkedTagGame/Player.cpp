#include "Player.h"

Player::Player()
{
	isTagged = false;
	m_circle.setRadius(200);
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
	m_circle.setFillColor(m_color); //set back to real color
}
