#include "stdafx.h"
#include "Boid.h"

Boid::Boid(sf::Vector2f pos, sf::Vector2f velocity)
{
	m_position = pos;
	m_velocity = velocity;
}

Boid::~Boid()
{
}



sf::Vector2f Boid::getVelocity()
{
	return m_velocity;
}

sf::Vector2f Boid::getPosition()
{
	return m_position;
}

void Boid::Update()
{
	m_position += m_velocity;//mpve the boid 
}

void Boid::setPosition(sf::Vector2f newPos)
{
	m_position = newPos;
}

void Boid::setVelocity(sf::Vector2f newVel)
{
	m_velocity = newVel;
}
