#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject(sf::Vector2f position, sf::Vector2f velocity, sf::Texture  &tex)
{
	m_sprite.setTexture(tex);
	m_sprite.setPosition(position);
	m_position = position;
	m_velocity = velocity;
	angle = 0;
	m_sprite.setScale(sf::Vector2f(0.3, 0.3));
}

GameObject::~GameObject()
{

}

void GameObject::Update(sf::Vector2f newPosition,sf::Vector2f newVelocity)
{
	
	m_position = newPosition;
	m_velocity = newVelocity;

	m_sprite.setPosition(m_position);

	angle = atan2(m_velocity.y, m_velocity.x);
	angle = angle * (180 / 3.14);

	m_sprite.setRotation(angle);
	
}

sf::Vector2f GameObject::getVelocity()
{
	return m_velocity;
}

sf::Vector2f GameObject::getPosition()
{
	return m_position;
}

void GameObject::Draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}


void GameObject::setVelocity(sf::Vector2f newVel)
{
	m_velocity = newVel;
}
