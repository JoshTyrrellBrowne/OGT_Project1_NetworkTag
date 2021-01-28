#pragma once
#include "SFML/Graphics.hpp"

class Player
{
	Player();
public:
	void setId(int t_ID) { m_ID = t_ID; }
	void setPosition(sf::Vector2f t_pos) { m_circle.setPosition(t_pos); }
	void setColor(sf::Color t_Color) { m_color = t_Color; }
	void tag();
	void unTag();
	sf::CircleShape getCircle() { return m_circle; };
	int getID() { return m_ID; }

private:
	sf::CircleShape m_circle;//The players circle object
	sf::Color m_color; //The 'real' colour of this player
	bool isTagged; //True if this player is tagged/the chaser, false if not
	int m_ID; // The ID of this player
};