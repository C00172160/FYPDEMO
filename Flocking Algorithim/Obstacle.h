#ifndef OBSTACLE_H
#define OBSTACLE_H


#include "SFML/Graphics.hpp"


class Obstacle {

public:
	Obstacle(sf::Vector2f position,  float AvoidanceRadius,sf::Texture &tex);
	~Obstacle();
	sf::Vector2f getPosition();
	void Draw(sf::RenderWindow & window);
	float getRadius();

private:
	sf::Vector2f m_position;
	sf::Sprite m_sprite;
	float radius;


};

#endif