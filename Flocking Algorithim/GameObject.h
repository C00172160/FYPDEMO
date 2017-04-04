#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H


#include "SFML/Graphics.hpp"


class GameObject {

public:
	GameObject(sf::Vector2f position, sf::Vector2f velocity,sf::Texture &tex);
	~GameObject();
	void Update(sf::Vector2f newPosition, sf::Vector2f newVelocity);
	sf::Vector2f getVelocity();
	sf::Vector2f getPosition();
	void Draw(sf::RenderWindow & window);
	void setVelocity(sf::Vector2f newVel);

private:
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	sf::Sprite m_sprite;
	float angle;

};

#endif