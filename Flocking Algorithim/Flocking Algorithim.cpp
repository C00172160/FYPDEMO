// Flocking Algorithim.cpp 
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

GameObject creategameObject(sf::Texture &tex,sf::RectangleShape & border);
int main()
{
	srand(time(NULL));//for seeding the random gen
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Flocking Algorithim",sf::Style::Fullscreen);//,sf::Style::Fullscreen);
	
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
	

	int noOfObjects = 2;//number of boids/objects
	int maxObjects = 100;// min number of obsticles
	int minObjects = 2;//min number of obsticles
	int previousNoOfObjects;//store the number of objects there was in the last frame
	int differenceInObjects; //this used so for finding out how many onjects to add or remove

	int maxObstacles = 10;//max number of obsticles

	float flockDistance = 0;//the  distance between two boids that they will consider for  flocking
	int maxFlockingDistance = 500;// max distance in pixels that boids can be from each other to flock
	int minFlockingDistance = 40;//min distance in pixels that boid can be from each other to flock

	float separationDistance = 0;//how far from the position of a boid the other boids will seperate by
	int maxSeparationDistance = 100; //max distance in pixels that boids will separate from each other
	int minSeparationDistance = 10;//max distance in pixels that boids will separate from each other

	float fovAngle = 0;//the angle that each boid can "see" ie feild of view 
	int maxFOV = 360;//maximum feild of view 360 ie full circle 
	int minFOV = 20;//minimun feild of view
	
	obstacleTex.loadFromFile("Dependencies/assets/Obstacle.png");//load Texture
	clickedBox.loadFromFile("Dependencies/assets/clicked.png");//load Texture
	unclickedBox.loadFromFile("Dependencies/assets/unclicked.png");//load Texture
	boidtexture.loadFromFile("Dependencies/assets/boid.png");//load Texture
	scrollBar.loadFromFile("Dependencies/assets/bar.png");//load Texture
	scrollBarCursor.loadFromFile("Dependencies/assets/barcursor.png");//load Texture
	buttonTex.loadFromFile("Dependencies/assets/button.png");//load Texture

	sf::Sprite flockButton;//sprite for a button that will turn on the flocking algotithim
	flockButton.setTexture(buttonTex);
	flockButton.setOrigin(flockButton.getGlobalBounds().width / 2, flockButton.getGlobalBounds().height / 2);
	flockButton.setPosition(sf::Vector2f(1700, 200));

	ScrollBar flockingDistanceBar(scrollBar,scrollBarCursor,sf::Vector2f(1650,500),sf::Vector2f(1560,500),maxFlockingDistance,minFlockingDistance);//used to vary the flocking distance at runtime
	ScrollBar separationDistanceBar(scrollBar, scrollBarCursor, sf::Vector2f(1650, 600), sf::Vector2f(1530, 600), maxSeparationDistance, minSeparationDistance);//used to vary the separation distance at runtime
	ScrollBar fovBar(scrollBar, scrollBarCursor, sf::Vector2f(1650, 700), sf::Vector2f(1650, 700), maxFOV, minFOV);//used to vary the Feild of view at runtime
	ScrollBar boidsBar(scrollBar, scrollBarCursor, sf::Vector2f(1650, 800), sf::Vector2f(1510, 800), maxObjects, minObjects);//used to vary the number of boids at runtime

	Checkbox alignBox(clickedBox, unclickedBox, sf::Vector2f(1500, 100));// tick box for toggling the alignment component of the flocking algorithim on and off 
	Checkbox cohereBox(clickedBox, unclickedBox, sf::Vector2f(1500, 200));// tick box for toggling the Cohesion component of the flocking algorithim on and off 
	Checkbox SeparateBox(clickedBox, unclickedBox, sf::Vector2f(1500, 300));// tick box for toggling the Separation component of the flocking algorithim on and off 
	Checkbox debugBox(clickedBox, unclickedBox, sf::Vector2f(1500, 400));// tick box for toggling the Debug info on and off 
	Checkbox followBox(clickedBox, unclickedBox, sf::Vector2f(1500, 900));//// tick box for toggling weither the flock will follow the mouse
	
	sf::RectangleShape border; // a rectangle representing the area in witch the boids will flock
	border.setSize(sf::Vector2f(900, 900));
	border.setOutlineColor(sf::Color::Black);
	border.setOutlineThickness(5);
	border.setPosition(200, 100);

	flockingsys.InBoudsOn(true);//lets the flocking system know that the boids will have a boundry to stay in
	flockingsys.setBounds(border);//tells the flocking system the position and size of the boundry by passing in a reference to a rectangle. in this case its the same as the border rectangle

	sf::Font arial; //loading a font for text
	// Load it from a file
	if (!arial.loadFromFile("Dependencies/assets/arial.ttf"))
	{
		std::cout << "error loding font" << std::endl;
	}

	sf::Text alignText;//sets up a text for indicating witch button is the toggle for alignment
	alignText.setFillColor(sf::Color::Red);
	alignText.setString("Alignment");
	alignText.setFont(arial);
	alignText.setOrigin(alignText.getGlobalBounds().width / 2, alignText.getGlobalBounds().height / 2);
	alignText.setPosition(sf::Vector2f(alignBox.getPosition().x - alignText.getGlobalBounds().width + 30, alignBox.getPosition().y));

	sf::Text coheseText;//sets up a text for indicating witch button is the toggle for cohesion
	coheseText.setFillColor(sf::Color::Blue);
	coheseText.setString("Cohesion");
	coheseText.setFont(arial);
	coheseText.setOrigin(coheseText.getGlobalBounds().width / 2, coheseText.getGlobalBounds().height / 2);
	coheseText.setPosition(sf::Vector2f(cohereBox.getPosition().x - coheseText.getGlobalBounds().width + 30, cohereBox.getPosition().y));

	sf::Text separateText;//sets up a text for indicating witch button is the toggle for separation
	separateText.setString("Separation");
	separateText.setFillColor(sf::Color(38, 106, 46, 255));
	separateText.setFont(arial);
	separateText.setOrigin(separateText.getGlobalBounds().width / 2, separateText.getGlobalBounds().height / 2);
	separateText.setPosition(sf::Vector2f(SeparateBox.getPosition().x - separateText.getGlobalBounds().width + 30, SeparateBox.getPosition().y));

	sf::Text debugText;//sets up a text for indicating witch button is the toggle for the debug information
	debugText.setFillColor(sf::Color::Black);
	debugText.setString("Debug Circles");
	debugText.setFont(arial);
	debugText.setOrigin(debugText.getGlobalBounds().width / 2, debugText.getGlobalBounds().height / 2);
	debugText.setPosition(sf::Vector2f(debugBox.getPosition().x - debugText.getGlobalBounds().width + 30, debugBox.getPosition().y));

	sf::Text fovText;//sets up a text displaying the feild of view in degrees to the user and pointing out witch scroll bar is the fov bar
	fovText.setFillColor(sf::Color::Black);
	fovText.setString("FOV Angle = " + std::to_string((int)fovAngle)+ "°");
	fovText.setFont(arial);
	fovText.setOrigin(fovText.getGlobalBounds().width / 2, fovText.getGlobalBounds().height / 2);
	fovText.setPosition(sf::Vector2f(1300,700));

	sf::Text boidText;//sets up a text displaying the number of boid to the user and pointing out witch scroll bar is the boid slider
	boidText.setFillColor(sf::Color::Black);
	boidText.setString("No of Boids = " + std::to_string((int)noOfObjects));
	boidText.setFont(arial);
	boidText.setOrigin(boidText.getGlobalBounds().width / 2, boidText.getGlobalBounds().height / 2);
	boidText.setPosition(sf::Vector2f(1300, 800));

	sf::Text flockDistanceText;//sets up a text displaying the flock distance in pixels to the user and pointing out witch scroll bar is the slider for flock distance
	flockDistanceText.setFillColor(sf::Color::Black);
	flockDistanceText.setString("Alignment/Cohere Distance = " + std::to_string((int)flockDistance)+"px");
	flockDistanceText.setFont(arial);
	flockDistanceText.setOrigin(flockDistanceText.getGlobalBounds().width / 2, flockDistanceText.getGlobalBounds().height / 2);
	flockDistanceText.setPosition(sf::Vector2f(1400, 500));

	sf::Text separateDisatanceText;//sets up a text displaying the flock distance in pixels to the user and pointing out witch scroll bar is the slider for separation distance
	separateDisatanceText.setFillColor(sf::Color::Black);
	separateDisatanceText.setString("Separation Distance = " + std::to_string((int)separationDistance)+"px");
	separateDisatanceText.setFont(arial);
	separateDisatanceText.setOrigin(separateDisatanceText.getGlobalBounds().width / 2, separateDisatanceText.getGlobalBounds().height / 2);
	separateDisatanceText.setPosition(sf::Vector2f(1300, 600));

	sf::Text flockButtonText;//sets up a text displaying to the user where the button for turning on the flock is located
	flockButtonText.setFillColor(sf::Color::Black);
	flockButtonText.setString("FLOCK!");
	flockButtonText.setFont(arial);
	flockButtonText.setOrigin(flockButtonText.getGlobalBounds().width / 2, flockButtonText.getGlobalBounds().height / 2);
	flockButtonText.setPosition(sf::Vector2f(1790, 195));

	sf::Text followMouseText;//sets up a text displaying to the user where the button for getting the flock to follow the mouse is located;
	followMouseText.setFillColor(sf::Color::Black);
	followMouseText.setString("Follow Mouse");
	followMouseText.setFont(arial);
	followMouseText.setOrigin(followMouseText.getGlobalBounds().width / 2, followMouseText.getGlobalBounds().height / 2);
	followMouseText.setPosition(sf::Vector2f(1300, 900));


	sf::Vertex AlignmentLine[] =//lies to be draw for asthetic reasons. a lie from the alignent box to the flock button
	{
		sf::Vertex(sf::Vector2f(alignBox.getPosition().x + unclickedBox.getSize().x / 2, alignBox.getPosition().y),sf::Color::Red),
		sf::Vertex(sf::Vector2f(flockButton.getPosition().x - unclickedBox.getSize().x / 2, flockButton.getPosition().y),sf::Color::Black),

	};
	sf::Vertex CohesionLine[] =//same as above but for cohesion box
	{
		sf::Vertex(sf::Vector2f(cohereBox.getPosition().x + unclickedBox.getSize().x/2, cohereBox.getPosition().y),sf::Color::Blue),
		sf::Vertex(sf::Vector2f(flockButton.getPosition().x - unclickedBox.getSize().x / 2, flockButton.getPosition().y),sf::Color::Black)
	};
	sf::Vertex SeparationLine[] =//same as above but for Separation box
	{
		sf::Vertex(sf::Vector2f(SeparateBox.getPosition().x + unclickedBox.getSize().x / 2, SeparateBox.getPosition().y),sf::Color(38,106,46,255)),
		sf::Vertex(sf::Vector2f(flockButton.getPosition().x - unclickedBox.getSize().x / 2, flockButton.getPosition().y),sf::Color::Black)
	};



	std::vector<GameObject> gameObjects;//holds the objects to be flocked. we will pass the positions and velocity form these objects to the flocking system and get back the flocked result
	std::vector<Obstacle> obsticales;//list of obstacles 

	for (int i = 0; i < noOfObjects; i++)//initilise the vector of game objects and add them to the flocking system
	{
		GameObject temp = creategameObject(boidtexture,border);//create a boid
		gameObjects.push_back(temp);//send it into the gam objects vector
		flockingsys.Add_Boid(temp.getPosition(),temp.getVelocity());//add it to the flocking sys.
	}

	for (int i =0; i <maxObstacles;i++)//create obstacles and add them to the flockig system.
	{
		int maxX = border.getPosition().x + border.getGlobalBounds().width;
		int minX = border.getPosition().x;
		int maxY = border.getPosition().y + border.getGlobalBounds().height;
		int minY = border.getPosition().y;
		Obstacle ob(sf::Vector2f((rand() % (maxX - minX)+minX), (rand() % (maxY - minY) + minY)), 30, obstacleTex,true);
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
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}


			
		
		}

		flockDistance = flockingDistanceBar.Update(window);//update the flocking distance to what the scroll bar is on
		separationDistance = separationDistanceBar.Update(window);//update the separation distance to what the scroll bar is on
		fovAngle = fovBar.Update(window);//update the fov angle to what the scroll bar is on


		previousNoOfObjects = noOfObjects;//store the number of boids in the last frame
		noOfObjects = boidsBar.Update(window);//update the number of boids  to what the scroll bar is on
		differenceInObjects = noOfObjects - previousNoOfObjects;// how many objects are to be added or removed

		if (differenceInObjects != 0)//if there is a change in the number objects. either add more of remove some
		{
			if (differenceInObjects > 0)// if we need to add more boids
			{
				for (int i = 0; i < differenceInObjects; i++)
				{
					GameObject temp = creategameObject(boidtexture,border);
					gameObjects.push_back(temp);
					flockingsys.Add_Boid(temp.getPosition(), temp.getVelocity());
				}
			}
			if (differenceInObjects < 0)// if we need to remove some boids
			{
				differenceInObjects = -differenceInObjects;
				for (int i = 0; i < differenceInObjects; i++)
				{
					gameObjects.erase(gameObjects.begin() + 0);
					flockingsys.Delete_Boid(0);
				
				}
			}

		}
		

		flockDistanceText.setString("Flock Distance = " + std::to_string((int)flockDistance)+"px");//update all the strings
		fovText.setString("FOV Angle = " + std::to_string((int)fovAngle)+"°");//update all the strings
		separateDisatanceText.setString("Separation Distance = " + std::to_string((int)separationDistance)+"px");//update all the strings
		boidText.setString("No of Boids = " + std::to_string((int)noOfObjects));//update all the strings


		if (gameObjects.size() > 0)
		{
			flockingsys.Update( flockDistance, separationDistance,fovAngle);//update the flocking system
		
			for (int i = 0; i < gameObjects.size(); i++)
			{
				gameObjects[i].Update(flockingsys.get_PositionResult(i), flockingsys.get_VelocityResult(i));//update the game objects
			}
		}
	
		if (sf::Mouse::getPosition(window).x > flockButton.getPosition().x - flockButton.getGlobalBounds().width / 2
			&& sf::Mouse::getPosition(window).x < flockButton.getPosition().x + flockButton.getGlobalBounds().width / 2
			&& sf::Mouse::getPosition(window).y > flockButton.getPosition().y - flockButton.getGlobalBounds().height / 2
			&& sf::Mouse::getPosition(window).y < flockButton.getPosition().y + flockButton.getGlobalBounds().height / 2)//check if the mouse is within the bouds of the flock button
		{
			flockButton.setScale(sf::Vector2f(1.2, 1.2));//when the mouse is over the button, make it bigger to indicate it is clickable
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))//if the button is pressed
			{
				//turn on all flocking componets
				alignBox.setOn(true);
				cohereBox.setOn(true);
				SeparateBox.setOn(true);

			}
		}
	


		if (alignBox.Update(window) == true)//if the button is pressed
		{
			flockingsys.AlignOn(true);//turn on the component
		}
		else
		{
			flockingsys.AlignOn(false);//if not on,turn off component
		}

		if (cohereBox.Update(window) == true)//same as above
		{
			flockingsys.CohereOn(true);
		}
		else
		{
			flockingsys.CohereOn(false);
		}

		if (SeparateBox.Update(window) == true)//same as above
		{
			flockingsys.SeparateOn(true);
		}
		else
		{
			flockingsys.SeparateOn(false);
		}


		if (followBox.Update(window) == true)//the follow mouse button is on
		{
			if (sf::Mouse::getPosition(window).x > border.getPosition().x
				&& sf::Mouse::getPosition(window).x <  border.getPosition().x + border.getGlobalBounds().width
				&& sf::Mouse::getPosition(window).y > border.getPosition().y
				&& sf::Mouse::getPosition(window).y <  border.getPosition().y + border.getGlobalBounds().height)//check if mouse is in the flocking "arena"/within the border
			{
				
				flockingsys.FollowOn(true);//tell the flocking system it needs to follow a target
				flockingsys.SetTarget(sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y));//set the target to the mouse position
			}
			else
			{
				flockingsys.FollowOn(false);//dont follow if mouse isnt in the bounds
			}
		}
		else
		{
			flockingsys.FollowOn(false);
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
		cohereBox.Draw(window);
		SeparateBox.Draw(window);
		debugBox.Draw(window);
		followBox.Draw(window);
		window.draw(flockButton);
		window.draw(alignText);
		window.draw(coheseText);
		window.draw(separateText);
		window.draw(flockButtonText);
		window.draw(debugText);
		window.draw(flockDistanceText);
		window.draw(separateDisatanceText);
		window.draw(followMouseText);
		window.draw(fovText);
		window.draw(boidText);
		flockingDistanceBar.Draw(window);
		separationDistanceBar.Draw(window);
		fovBar.Draw(window);
		boidsBar.Draw(window);
		window.draw(AlignmentLine, 2, sf::Lines);
		window.draw(SeparationLine, 2, sf::Lines);
		window.draw(CohesionLine, 2, sf::Lines);
	
		window.display();
	
	}

	return 0;
}

GameObject creategameObject(sf::Texture &tex , sf::RectangleShape & border)
{
	int a = rand() % 100 + 1;

	int maxX = border.getPosition().x + border.getGlobalBounds().width;
	int minX = border.getPosition().x;
	int maxY = border.getPosition().y + border.getGlobalBounds().height;
	int minY = border.getPosition().y;

	GameObject temp(sf::Vector2f((rand() % (maxX - minX) + minX), (rand() % (maxY - minY) + minY)), sf::Vector2f(rand() % 2 + 1, rand() % 2 + 1), tex);
	if (a % 2 == 0)
	{
		temp.setVelocity(sf::Vector2f(-temp.getVelocity().x, temp.getVelocity().y));
	}

	return temp;
}
