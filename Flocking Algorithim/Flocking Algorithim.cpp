// Flocking Algorithim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SFML\Graphics.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Flocking Algorithim");
	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
	
		window.display();
	}

	return 0;
}
