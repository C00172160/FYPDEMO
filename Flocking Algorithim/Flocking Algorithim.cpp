// Flocking Algorithim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "FlockingSystem.h"
#include "GameObject.h"
#include "SFML\Graphics.hpp"
#include <time.h>


int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(900, 900), "Flocking Algorithim");
	window.setFramerateLimit(120);
	float flockDistance = 200;
	float separationDistance = 10;
	FlockingSystem flockingsys;//initilise the flocking system
	sf::Texture boidtexture;//texture for the boids
	boidtexture.setSmooth(true);
	boidtexture.loadFromFile("Dependencies/assets/boid.png");
	int maxObjects = 50;
	std::vector<GameObject> gameObjects;
	bool flock_on = false;
	
	for (int i = 0; i < maxObjects; i++)
	{
		int a = rand() % 100 + 1;
	
		GameObject temp(sf::Vector2f(rand() % 900 + 1, rand() % 900 + 1), sf::Vector2f(rand() % 2 + 1, rand() % 2 + 1), boidtexture);
		if (a % 2 == 0)
		{
			temp.setVelocity(sf::Vector2f(-temp.getVelocity().x, temp.getVelocity().y));
		}

		gameObjects.push_back(temp);
		flockingsys.Add_Boid(gameObjects[i].getPosition(), gameObjects[i].getVelocity());
	}

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O) == true)
		{
			flock_on = true;
		}
	
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) == true)
		{
			flock_on = false;
		}


		flockingsys.Update(flock_on,flockDistance,separationDistance);

		for (int i = 0; i < maxObjects; i++)
		{
			
			gameObjects[i].Update(flockingsys.get_PositionResult(i), flockingsys.get_VelocityResult(i));
		   
		}
	
		window.clear(sf::Color::White);
		
		for (int i = 0; i < maxObjects; i++)
		{
			gameObjects[i].Draw(window);
		}
	
		window.display();
	}

	return 0;
}
