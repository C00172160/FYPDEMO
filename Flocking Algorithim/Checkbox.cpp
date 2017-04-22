#include "stdafx.h"
#include "Checkbox.h"




Checkbox::Checkbox(sf::Texture & clickedTex, sf::Texture & unclickedTex, sf::Vector2f position)
{
	unclicked = unclickedTex;
	clicked = clickedTex;
	m_sprite.setTexture(clicked);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width/2,m_sprite.getGlobalBounds().height/2);
	m_sprite.setPosition(position);
	pos = position;
	OptionOn = false;

}

bool Checkbox::Update(sf::RenderWindow & window )
{
	


	if (sf::Mouse::getPosition(window).x > m_sprite.getPosition().x - m_sprite.getGlobalBounds().width/2
	&&  sf::Mouse::getPosition(window).x < m_sprite.getPosition().x + m_sprite.getGlobalBounds().width/2
	&&  sf::Mouse::getPosition(window).y > m_sprite.getPosition().y - m_sprite.getGlobalBounds().height / 2
	&&  sf::Mouse::getPosition(window).y < m_sprite.getPosition().y + m_sprite.getGlobalBounds().height/2)//check if the mouse is over the box
	{
		m_sprite.setScale(sf::Vector2f(1.2, 1.2));//increase the size of the box to show it is selected
		if (mouseClicked == false && sf::Mouse::isButtonPressed(sf::Mouse::Left))//when the box is clicked turn it on
		{
			OptionOn = !OptionOn;

		}
	}
	else
	{
		m_sprite.setScale(sf::Vector2f(1, 1));
	}

	if (OptionOn == true)
	{
		m_sprite.setTexture(clicked);//change the texture to the on texture
	}
	else
	{
		m_sprite.setTexture(unclicked);
	}


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		mouseClicked = true;
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) == false)
	{
		mouseClicked = false;
	}
	return OptionOn;
}

void Checkbox::Draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

sf::Vector2f Checkbox::getPosition()
{
	return pos;
}

void Checkbox::setOn(bool on)
{
	OptionOn = on;
}


