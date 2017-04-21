#include "stdafx.h"
#include "ScrollBar.h"

ScrollBar::ScrollBar(sf::Texture & barTex, sf::Texture & CursorTex, sf::Vector2f barPos, sf::Vector2f cursorPos, float max, float min)
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
		&& sf::Mouse::getPosition(window).y < m_barSprite.getPosition().y + m_barSprite.getGlobalBounds().height / 2)
	{
		m_CursorSprite.setScale(1.1, 1.1);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			m_CursorSprite.setPosition(sf::Vector2f(sf::Mouse::getPosition(window).x, m_CursorSprite.getPosition().y));
			cursorPosition = m_CursorSprite.getPosition().x - (m_barSprite.getPosition().x - m_barSprite.getGlobalBounds().width / 2);
		    percentage = cursorPosition / m_barSprite.getLocalBounds().width * 100;
			result = ((maxNumber / 100) * percentage) + minNumber;
		}
	}
	else
	{
		m_CursorSprite.setScale(1, 1);
	}

	if (maxNumber - result <= 5)
	{
		result = maxNumber;
	}
	else if (result - minNumber <= 5)
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
