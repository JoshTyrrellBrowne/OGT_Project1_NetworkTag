#include "Player.h"

Player::Player()
{
	isTagged = false;
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
