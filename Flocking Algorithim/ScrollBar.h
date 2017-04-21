#ifndef SCROLLBAR_H
#define SCROLLBAR_H


#include "SFML/Graphics.hpp"
#include "stdafx.h"

class ScrollBar {

public:
	ScrollBar(sf::Texture & barTex, sf::Texture & CursorTex, sf::Vector2f barPos,sf::Vector2f cursorPos,float max,float min);

	float Update(sf::RenderWindow & window);
	void Draw(sf::RenderWindow & window);


private:
	float maxNumber;
	float minNumber;
	float cursorPosition;
	float percentage;
	float result;
	sf::Sprite m_barSprite;
	sf::Sprite m_CursorSprite;
	sf::Texture barTex;
	sf::Texture CrsorTex;
};

#endif

