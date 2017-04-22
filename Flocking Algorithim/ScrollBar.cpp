#include "stdafx.h"
#include "ScrollBar.h"

ScrollBar::ScrollBar(sf::Texture & barTex, sf::Texture & CursorTex, sf::Vector2f barPos, sf::Vector2f cursorPos, float max, float min)//the texture for the bar the cursor, position and the minimum and maximum values on the bar
{
	maxNumber = max;
	minNumber = min;

	m_barSprite.setTexture(barTex);
	m_barSprite.setOrigin(m_barSprite.getGlobalBounds().width / 2, m_barSprite.getGlobalBounds().height / 2);
	m_barSprite.setPosition(barPos);

	m_CursorSprite.setTexture(CursorTex);
	m_CursorSprite.setOrigin(m_CursorSprite.getGlobalBounds().width / 2, m_CursorSprite.getGlobalBounds().height / 2);
	m_CursorSprite.setPosition(cursorPos);

	cursorPosition = m_CursorSprite.getPosition().x - (m_barSprite.getPosition().x - m_barSprite.getGlobalBounds().width / 2);
	percentage = cursorPosition / m_barSprite.getLocalBounds().width * 100;
	result = ((maxNumber / 100) * percentage) + minNumber;

}

float ScrollBar::Update(sf::RenderWindow & window)
{
	if (sf::Mouse::getPosition(window).x > m_barSprite.getPosition().x - m_barSprite.getGlobalBounds().width / 2
		&& sf::Mouse::getPosition(window).x < m_barSprite.getPosition().x + m_barSprite.getGlobalBounds().width / 2
		&& sf::Mouse::getPosition(window).y > m_barSprite.getPosition().y - m_barSprite.getGlobalBounds().height / 2
		&& sf::Mouse::getPosition(window).y < m_barSprite.getPosition().y + m_barSprite.getGlobalBounds().height / 2)//check if the mouse is over the box
	{
		m_CursorSprite.setScale(1.1, 1.1);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))//if the mouse is clicked, 
		{
			m_CursorSprite.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x, m_CursorSprite.getPosition().y));//move the cursor to the mousepos
			cursorPosition = m_CursorSprite.getPosition().x - (m_barSprite.getPosition().x - m_barSprite.getGlobalBounds().width / 2);
		    percentage = cursorPosition / m_barSprite.getLocalBounds().width * 100;//find out where on the bar the cursor is now. what percent of the bar it is on 
			result = ((maxNumber / 100) * percentage) + minNumber;//multiply the percentage by one percennt of the difference between min and max to get the new number
		}
	}
	else
	{
		m_CursorSprite.setScale(1, 1);
	}

	if (maxNumber - result <= 5)//if the number is high enough set the number to the max
	{
		result = maxNumber;
	}
	else if (result - minNumber <= 5)//if the number is low enough set the number to the min
	{
		result = minNumber;
	}

	return result;
}

void ScrollBar::Draw(sf::RenderWindow & window)
{
	window.draw(m_barSprite);
	window.draw(m_CursorSprite);
}
