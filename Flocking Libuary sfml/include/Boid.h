#ifndef BOID_H
#define BOID_H


#include "SFML/Graphics.hpp"


class Boid {

public:
	Boid(sf::Vector2f pos,sf::Vector2f velocity);
	~Boid();
	sf::Vector2f getVelocity();
	sf::Vector2f getPosition();
	void Update();
	void setPosition(sf::Vector2f newPos);
	void setVelocity(sf::Vector2f newVel);

private:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;

};

#endif