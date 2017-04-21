// Flocking Algorithim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "FlockingSystem.h"
#include "Checkbox.h"
#include "ScrollBar.h"
#include "GameObject.h"
#include "SFML\Graphics.hpp"
#include <time.h>
#include "Obstacle.h"

int main()
{
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Flocking Algorithim");//,sf::Style::Fullscreen);
	window.setKeyRepeatEnabled(false);
	window.setFramerateLimit(60);

	sf::Texture boidtexture;//texture for the boids
	boidtexture.setSmooth(true);
	sf::Texture unclickedBox;//texture for the checkbox
	unclickedBox.setSmooth(true);
	sf::Texture clickedBox;//texture for the checkbox
	clickedBox.setSmooth(true);
	sf::Texture scrollBar;//texture for the checkbox
	scrollBar.setSmooth(true);
	sf::Texture scrollBarCursor;//texture for the checkbox
	scrollBarCursor.setSmooth(true);
	sf::Texture buttonTex;//texture for the checkbox
	buttonTex.setSmooth(true);
	sf::Texture obstacleTex;//texture for the checkbox
	obstacleTex.setSmooth(true);

	FlockingSystem flockingsys;//initilise the flocking system
	float flockDistance = 100;//the max distance between two boids that they will consider for  flocking
	float separationDistance = 30;//how far from the position of a boid the other boids will seperate by
	float fovAngle = 180;
	int maxObjects = 50;//number of boids/objects
	int maxObstacles = 10;
	int maxFlockingDistance = 500;
	int minFlockingDistance = 40;
	int maxSeparationDistance = 100;
	int minSeparationDistance = 10;
	
	int maxFOV = 360;
	int minFOV = 20;


	obstacleTex.loadFromFile("Dependencies/assets/Obstacle.png");
	clickedBox.loadFromFile("Dependencies/assets/clicked.png");
	unclickedBox.loadFromFile("Dependencies/assets/unclicked.png");
	boidtexture.loadFromFile("Dependencies/assets/boid.png");
	scrollBar.loadFromFile("Dependencies/assets/bar.png");
	scrollBarCursor.loadFromFile("Dependencies/assets/barcursor.png");
	buttonTex.loadFromFile("Dependencies/assets/button.png");

	sf::Sprite flockButton;
	flockButton.setTexture(buttonTex);
	flockButton.setOrigin(flockButton.getGlobalBounds().width / 2, flockButton.getGlobalBounds().height / 2);
	flockButton.setPosition(sf::Vector2f(1700, 200));

	ScrollBar flockingDistanceBar(scrollBar,scrollBarCursor,sf::Vector2f(1650,500),sf::Vector2f(1560,500),maxFlockingDistance,minFlockingDistance);
	ScrollBar separationDistanceBar(scrollBar, scrollBarCursor, sf::Vector2f(1650, 600), sf::Vector2f(1530, 600), maxSeparationDistance, minSeparationDistance);
	ScrollBar fovBar(scrollBar, scrollBarCursor, sf::Vector2f(1650, 700), sf::Vector2f(1650, 700), maxFOV, minFOV);

	Checkbox alignBox(clickedBox, unclickedBox, sf::Vector2f(1500, 100));
	Checkbox CohereBox(clickedBox, unclickedBox, sf::Vector2f(1500, 200));
	Checkbox SeparateBox(clickedBox, unclickedBox, sf::Vector2f(1500, 300));
	Checkbox debugBox(clickedBox, unclickedBox, sf::Vector2f(1500, 400));
	
	sf::RectangleShape border;
	border.setSize(sf::Vector2f(900, 900));
	border.setOutlineColor(sf::Color::Black);
	border.setOutlineThickness(5);
	border.setPosition(200, 0);

	sf::Font arial;
	// Load it from a file
	if (!arial.loadFromFile("Dependencies/assets/arial.ttf"))
	{
		std::cout << "error loding font" << std::endl;
	}
	sf::Text alignText;
	alignText.setFillColor(sf::Color::Red);
	alignText.setString("Alignment");
	alignText.setFont(arial);
	alignText.setOrigin(alignText.getGlobalBounds().width / 2, alignText.getGlobalBounds().height / 2);
	alignText.setPosition(sf::Vector2f(alignBox.getPosition().x - alignText.getGlobalBounds().width + 30, alignBox.getPosition().y));

	sf::Text coheseText;
	coheseText.setFillColor(sf::Color::Blue);
	coheseText.setString("Cohesion");
	coheseText.setFont(arial);
	coheseText.setOrigin(coheseText.getGlobalBounds().width / 2, coheseText.getGlobalBounds().height / 2);
	coheseText.setPosition(sf::Vector2f(CohereBox.getPosition().x - coheseText.getGlobalBounds().width + 30, CohereBox.getPosition().y));

	sf::Text separateText;
	separateText.setString("Separation");
	separateText.setFillColor(sf::Color(38, 106, 46, 255));
	separateText.setFont(arial);
	separateText.setOrigin(separateText.getGlobalBounds().width / 2, separateText.getGlobalBounds().height / 2);
	separateText.setPosition(sf::Vector2f(SeparateBox.getPosition().x - separateText.getGlobalBounds().width + 30, SeparateBox.getPosition().y));

	sf::Text debugText;
	debugText.setFillColor(sf::Color::Black);
	debugText.setString("Debug Circles");
	debugText.setFont(arial);
	debugText.setOrigin(debugText.getGlobalBounds().width / 2, debugText.getGlobalBounds().height / 2);
	debugText.setPosition(sf::Vector2f(debugBox.getPosition().x - debugText.getGlobalBounds().width + 30, debugBox.getPosition().y));

	sf::Text fovText;
	fovText.setFillColor(sf::Color::Black);
	fovText.setString("FOV Angle = " + std::to_string((int)fovAngle)+ "°");
	fovText.setFont(arial);
	fovText.setOrigin(fovText.getGlobalBounds().width / 2, fovText.getGlobalBounds().height / 2);
	fovText.setPosition(sf::Vector2f(1300,700));

	sf::Text FlockDistanceText;
	FlockDistanceText.setFillColor(sf::Color::Black);
	FlockDistanceText.setString("Alignment/Cohere Distance = " + std::to_string((int)flockDistance)+"px");
	FlockDistanceText.setFont(arial);
	FlockDistanceText.setOrigin(FlockDistanceText.getGlobalBounds().width / 2, FlockDistanceText.getGlobalBounds().height / 2);
	FlockDistanceText.setPosition(sf::Vector2f(1400, 500));

	sf::Text separateDisatanceText;
	separateDisatanceText.setFillColor(sf::Color::Black);
	separateDisatanceText.setString("Separation Distance = " + std::to_string((int)separationDistance)+"px");
	separateDisatanceText.setFont(arial);
	separateDisatanceText.setOrigin(separateDisatanceText.getGlobalBounds().width / 2, separateDisatanceText.getGlobalBounds().height / 2);
	separateDisatanceText.setPosition(sf::Vector2f(1300, 600));

	sf::Text flockButtonText;
	flockButtonText.setFillColor(sf::Color::Black);
	flockButtonText.setString("FLOCK!");
	flockButtonText.setFont(arial);
	flockButtonText.setOrigin(flockButtonText.getGlobalBounds().width / 2, flockButtonText.getGlobalBounds().height / 2);
	flockButtonText.setPosition(sf::Vector2f(1790, 195));




	sf::Vertex AlignmentLine[] =
	{
		sf::Vertex(sf::Vector2f(alignBox.getPosition().x + 15, alignBox.getPosition().y),sf::Color::Red),
		sf::Vertex(sf::Vector2f(flockButton.getPosition().x -15, flockButton.getPosition().y),sf::Color::Black)
	};
	sf::Vertex CohesionLine[] =
	{
		sf::Vertex(sf::Vector2f(CohereBox.getPosition().x + 15, CohereBox.getPosition().y),sf::Color::Blue),
		sf::Vertex(sf::Vector2f(flockButton.getPosition().x - 15, flockButton.getPosition().y),sf::Color::Black)
	};
	sf::Vertex SeparationLine[] =
	{
		sf::Vertex(sf::Vector2f(SeparateBox.getPosition().x + 15, SeparateBox.getPosition().y),sf::Color(38,106,46,255)),
		sf::Vertex(sf::Vector2f(flockButton.getPosition().x - 15, flockButton.getPosition().y),sf::Color::Black)
	};



	std::vector<GameObject> gameObjects;//holds the objects to be flocked. we will pass the positions and velocity form these objects to the flocking system and get back the flocked result
	std::vector<Obstacle> obsticales;

	for (int i = 0; i < maxObjects; i++)//initilise the vector of game objects and add them to the flocking system
	{
		int a = rand() % 100 + 1;
	
		GameObject temp(sf::Vector2f(rand() % 900 + 200, rand() % 900 + 1), sf::Vector2f(rand() % 2 + 1, rand() % 2 + 1), boidtexture);
		if (a % 2 == 0)
		{
			temp.setVelocity(sf::Vector2f(-temp.getVelocity().x, temp.getVelocity().y));
		}

		gameObjects.push_back(temp);
		flockingsys.Add_Boid(gameObjects[i].getPosition(), gameObjects[i].getVelocity());
	}


	for (int i =0; i <maxObstacles;i++)
	{
		Obstacle ob(sf::Vector2f(rand() % 900 + 200, rand() % 900 + 1), 30, obstacleTex);
		flockingsys.addObstacle(ob.getPosition(), ob.getRadius());
		obsticales.push_back(ob);
	}




	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::K)
				{
					gameObjects.erase(gameObjects.begin() + 4);
					flockingsys.Delete_Boid(4);
				}
			
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}


			
		
		}

		flockDistance = flockingDistanceBar.Update(window);
		separationDistance = separationDistanceBar.Update(window);
		fovAngle = fovBar.Update(window);

		FlockDistanceText.setString("Flock Distance = " + std::to_string((int)flockDistance)+"px");
		fovText.setString("FOV Angle = " + std::to_string((int)fovAngle)+"°");
		separateDisatanceText.setString("Separation Distance = " + std::to_string((int)separationDistance)+"px");

		if (gameObjects.size() > 0)
		{
			flockingsys.Update( flockDistance, separationDistance,fovAngle);
		
			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i].Update(flockingsys.get_PositionResult(i), flockingsys.get_VelocityResult(i));
			}
		}
	
		if (sf::Mouse::getPosition(window).x > flockButton.getPosition().x - flockButton.getGlobalBounds().width / 2
			&& sf::Mouse::getPosition(window).x < flockButton.getPosition().x + flockButton.getGlobalBounds().width / 2
			&& sf::Mouse::getPosition(window).y > flockButton.getPosition().y - flockButton.getGlobalBounds().height / 2
			&& sf::Mouse::getPosition(window).y < flockButton.getPosition().y + flockButton.getGlobalBounds().height / 2)
		{
			flockButton.setScale(sf::Vector2f(1.2, 1.2));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				alignBox.setOn(true);
				CohereBox.setOn(true);
				SeparateBox.setOn(true);

			}
		}
	


		if (alignBox.Update(window) == true)
		{
			flockingsys.AlignOn(true);
		}
		else
		{
			flockingsys.AlignOn(false);
		}
		if (CohereBox.Update(window) == true)
		{
			flockingsys.CohereOn(true);
		}
		else
		{
			flockingsys.CohereOn(false);
		}
		if (SeparateBox.Update(window) == true)
		{
			flockingsys.SeparateOn(true);
		}
		else
		{
			flockingsys.SeparateOn(false);
		}
	

		
	

		window.clear(sf::Color::White);
		
		window.draw(border);

		if (gameObjects.size() > 0)
		{
			if (debugBox.Update(window) == true)
			{
				flockingsys.DebugDraw(window);
			}
			

			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i].Draw(window);
			}
		}
		for (int i = 0; i < obsticales.size(); i++)
		{
			obsticales[i].Draw(window);
		}
		alignBox.Draw(window);
		CohereBox.Draw(window);
		SeparateBox.Draw(window);
		debugBox.Draw(window);
		window.draw(flockButton);
		window.draw(alignText);
		window.draw(coheseText);
		window.draw(separateText);
		window.draw(flockButtonText);
		window.draw(debugText);
		window.draw(FlockDistanceText);
		window.draw(separateDisatanceText);
		window.draw(fovText);
		flockingDistanceBar.Draw(window);
		separationDistanceBar.Draw(window);
		fovBar.Draw(window);
		window.draw(AlignmentLine, 2, sf::Lines);
		window.draw(SeparationLine, 2, sf::Lines);
		window.draw(CohesionLine, 2, sf::Lines);
	
		window.display();
	
	}

	return 0;
}
