#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f position,float AvoidanceRadius, sf::Texture & tex)
{
	m_position = position;
	m_sprite.setTexture(tex);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
	m_sprite.setPosition(position);
	radius = AvoidanceRadius;
}

Obstacle::~Obstacle()
{
}

sf::Vector2f Obstacle::getPosition()
{
	return m_position;
}

void Obstacle::Draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

float Obstacle::getRadius()
{
	return radius;
}
