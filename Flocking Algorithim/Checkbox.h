#ifndef CHECKBOX_H
#define CHECKBOX_H


#include "SFML/Graphics.hpp"
#include "stdafx.h"

class Checkbox {

public:
	Checkbox(sf::Texture & clickedTex, sf::Texture & unclickedTex,sf::Vector2f position);

	bool Update(sf::RenderWindow & window);
	void Draw(sf::RenderWindow & window);
	sf::Vector2f getPosition();
	void setOn(bool on);

private:
	bool OptionOn;
	bool mouseClicked;
	sf::Vector2f pos;
	sf::Sprite m_sprite;
	sf::Texture clicked;
	sf::Texture unclicked;
};

#endif

