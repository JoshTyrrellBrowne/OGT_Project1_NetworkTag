#pragma once
#include "SFML/Graphics.hpp"

class Player
{
public:
	Player();
	void setId(int t_ID) { m_ID = t_ID; }
	void setPosition(sf::Vector2f t_pos);
	//void setColor(sf::Color t_Color) { m_color = t_Color; }
	void tag();
	void unTag();
	sf::CircleShape getCircle() { return m_circle; };
	int getID() { return m_ID; }
	sf::Vector2f getPosition() { return m_circle.getPosition(); }

	bool isTagged; //True if this player is tagged/the chaser, false if not
private:
	sf::CircleShape m_circle;//The players circle object
	sf::Color m_color; //The 'real' colour of this player
	int m_ID; // The ID of this player
};