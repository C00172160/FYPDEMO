#include "stdafx.h"
#include "FlockingSystem.h"


FlockingSystem::FlockingSystem()
{
	//intilise the bools 
	cohere = true;
	separate = true;
	align = true;
	followTarget = false;
}

void FlockingSystem::Add_Boid(sf::Vector2f pos,sf::Vector2f velocity)
{
	m_boids.push_back(new Boid(pos, velocity));//create the boid
	sf::CircleShape debugflockdistanceCircle;//create the flock distance circle
	sf::CircleShape debugseparationdistanceCircle;// create the separation distace circle


	debugflockdistanceCircle.setOutlineColor(sf::Color::Red);
	debugflockdistanceCircle.setOutlineThickness(2);
	debugflockdistanceCircle.setFillColor(sf::Color(255, 0, 0, 10));
	debugFlockDistanceCircles.push_back(debugflockdistanceCircle);

	debugseparationdistanceCircle.setOutlineColor(sf::Color(38, 106, 46, 255));
	debugseparationdistanceCircle.setOutlineThickness(2);
	debugseparationdistanceCircle.setFillColor(sf::Color(38, 106, 46, 10));
	debugSeparationDistanceCircles.push_back(debugseparationdistanceCircle);

}

void FlockingSystem::Delete_Boid(int index)
{
	m_boids.erase(m_boids.begin() + index);
	debugFlockDistanceCircles.erase(debugFlockDistanceCircles.begin() + index);
	debugSeparationDistanceCircles.erase(debugSeparationDistanceCircles.begin() + index);
}

void FlockingSystem::Update(float flock_Distance , float Separation_Distance,float fov_angle)
{
	//update all the ranges
	flockRange = flock_Distance;
	separationRange = Separation_Distance;
	FOV_angle = fov_angle;
	
	for (int i = 0; i < m_boids.size(); i++)//loop through all boids
	{
		alignNeighbourCount = 0;
		cohereNeighbourCount = 0;
		separateNeighbourCount = 0;
		obsticleNeighbourCount = 0;

		flockingVector = sf::Vector2f(0, 0);
		alignment = sf::Vector2f(0, 0);
		cohesion = sf::Vector2f(0, 0);
		seperation = sf::Vector2f(0, 0);
		obstacleAvoidance = sf::Vector2f(0, 0);
		targetDirection = sf::Vector2f(0, 0);

		m_boids[i]->Update();//moves the boids
	     
		for (int j = 0; j <  m_boids.size(); j++)//need a nested loop because every boid has to check all others
		{
			if (align == true)
			{
				ComputeAlignment(i, j);//get the boids alignmet vector
			}
			if (cohere == true)
			{
				ComputeCohesion(i, j);//get the boids cohesion  vector
			}
			if (separate == true)
			{
				ComputeSeparation(i, j);//get the boids separation vector
			}
		}
		for (int j = 0 ; j < obstacleList.size(); j++)//check all boids against the obstacles
		{
			ComputeObsticleAvoidance(i, j);//get the obstacle avoidance vector
		}

		ComputeFlock(i);//this is where the components are combined to get the final flock vector
		checkBounds(i);//check if boid is still within bounds and take approiate action



		debugFlockDistanceCircles[i].setRadius(flock_Distance);//updates the debug circles
		debugFlockDistanceCircles[i].setOrigin(sf::Vector2f(debugFlockDistanceCircles[i].getGlobalBounds().width / 2, debugFlockDistanceCircles[i].getGlobalBounds().height / 2));
		debugFlockDistanceCircles[i].setPosition(m_boids[i]->getPosition());
		
		debugSeparationDistanceCircles[i].setRadius(Separation_Distance);
		debugSeparationDistanceCircles[i].setOrigin(sf::Vector2f(debugSeparationDistanceCircles[i].getGlobalBounds().width / 2, debugSeparationDistanceCircles[i].getGlobalBounds().height / 2));
		debugSeparationDistanceCircles[i].setPosition(m_boids[i]->getPosition());
	
	
		

	}

}

sf::Vector2f FlockingSystem::get_VelocityResult(int index)
{
	return m_boids[index]->getVelocity();
}

sf::Vector2f FlockingSystem::get_PositionResult(int index)
{
	return m_boids[index]->getPosition();
}

void FlockingSystem::ComputeAlignment(int currentBoid,int otherBoid)
{
	//alignment is calcultated by first checking if the is another boid within range, then if the boid is within the first boids fov. once we know that the other boid 
	//is within range we simply add to the neighbor count then add the other boids velocity to the alignment vector. in the Compute Flock method we get the average velociy
	//by dividing the alignment by alignment neighbour count then normalising it.
		if (otherBoid != currentBoid)
		{
			if (CheckRange(flockRange, m_boids[currentBoid]->getPosition(), m_boids[otherBoid]->getPosition()) == true && CheckRange(separationRange, m_boids[currentBoid]->getPosition(), m_boids[otherBoid]->getPosition()) == false)// CheckRange(separationRange, m_boids[currentBoid]->getPosition(), m_boids[i]->getPosition()) == false)
			{
				if (CheckFOV(currentBoid, otherBoid) == true)
				{
					alignment += m_boids[otherBoid]->getVelocity();
					alignNeighbourCount++;
				}
				
			}
			
		}
}

void FlockingSystem::ComputeCohesion(int currentBoid, int otherBoid)
{
	//cohesion is calcultated by first checking if the is another boid within range but not too close, then if the boid is within the first boids fov. once we know that the other boid 
	//is within range we simply add to the neighbor count then add the other boids Postion to the cohesion vector. in the Compute Flock method we get the average position
	//by dividing the cohesion by cohestion neighbour count then normalising it.
		if (otherBoid != currentBoid)
		{
			if (CheckRange(flockRange, m_boids[currentBoid]->getPosition(), m_boids[otherBoid]->getPosition()) == true && CheckRange(separationRange, m_boids[currentBoid]->getPosition(), m_boids[otherBoid]->getPosition()) == false)
			{
				if (CheckFOV(currentBoid, otherBoid) == true)
				{
					cohesion += m_boids[otherBoid]->getPosition() - m_boids[currentBoid]->getPosition();
					cohereNeighbourCount++;
				}
			}
			

		}

}

void FlockingSystem::ComputeSeparation(int currentBoid, int otherBoid)
{

	//separation is calcultated by first checking if the is another boid within separation range , then if the boid is within the first boids fov. once we know that the other boid 
	//is within range we simply add to the neighbor count then add the direction to the other boid to the separation vector. in the Compute Flock method we get the average position
	//by dividing the separation vector by separation neighbour count then normalising it.

		if (otherBoid != currentBoid)
		{
			if (CheckRange(separationRange, m_boids[otherBoid]->getPosition(), m_boids[currentBoid]->getPosition()) == true)
			{
				if (CheckFOV(currentBoid, otherBoid) == true)
				{
					seperation += m_boids[otherBoid]->getPosition() - m_boids[currentBoid]->getPosition();
					separateNeighbourCount++;
			
				}
			}

		}

}

void FlockingSystem::ComputeObsticleAvoidance(int currentBoid, int obstacle)
{
	//this is very similar to separation except that we are checking the obstacles list rather than the oter boids
	
		if (CheckRange(obstacleList[obstacle].avoidRadius, obstacleList[obstacle].position, m_boids[currentBoid]->getPosition()) == true)
		{
			
				obstacleAvoidance += obstacleList[obstacle].position - m_boids[currentBoid]->getPosition();
				obsticleNeighbourCount++;

			
		}

	
}

void FlockingSystem::ComputeFlock(int i)
{
	if (alignNeighbourCount == 0)//if there are no neighbours set alignment to zero
	{
		alignment = sf::Vector2f(0, 0);
	}
	else
	{
		alignment.x = alignment.x / alignNeighbourCount;//get the average velocity
		alignment.y = alignment.y / alignNeighbourCount;
		alignment = NormaliseVector(alignment);//normalise for direction
	}

	if (cohereNeighbourCount == 0)//if there are no neighbours set cohesion to zero
	{
		cohesion = sf::Vector2f(0, 0);
	}
	else
	{
		cohesion.x = cohesion.x / cohereNeighbourCount;
		cohesion.y = cohesion.y / cohereNeighbourCount;//get the average position
		cohesion = NormaliseVector(cohesion);
	}

	if (separateNeighbourCount == 0)//if there are no neighbours set separation to zero
	{
		seperation = sf::Vector2f(0, 0);
	}
	else
	{
		seperation.x = seperation.x / separateNeighbourCount;
		seperation.y = seperation.y / separateNeighbourCount;//get average seperation
		
		seperation.x *= -1;// set to minus so it moves the opposite direction
		seperation.y *= -1;

		seperation = NormaliseVector(seperation);


	}

	if (obsticleNeighbourCount == 0)//repeat for obstacles
	{
		obstacleAvoidance = sf::Vector2f(0, 0);
	}
	else
	{
		obstacleAvoidance.x = obstacleAvoidance.x / obsticleNeighbourCount;
		obstacleAvoidance.y = obstacleAvoidance.y / obsticleNeighbourCount;

		obstacleAvoidance.x *= -1;
		obstacleAvoidance.y *= -1;

		obstacleAvoidance = NormaliseVector(obstacleAvoidance);
	}

	if (followTarget == true)//if the boid should follow a target 
	{
		targetDirection = target - m_boids[i]->getPosition();//get the direction to it 
		targetDirection = NormaliseVector(targetDirection);//normalise for direction
		
	}
	else
	{
		targetDirection = sf::Vector2f(0, 0);//
	}

	direction = NormaliseVector(m_boids[i]->getVelocity());//keeps the boid moving. current direction 
	
	flockingVector.x = alignment.x + cohesion.x + direction.x + seperation.x + targetDirection.x+obstacleAvoidance.x;
	flockingVector.y = alignment.y + cohesion.y + direction.y + seperation.y + targetDirection.y +obstacleAvoidance.y;


	m_boids[i]->setVelocity(sf::Vector2f(flockingVector.x* 2, flockingVector.y * 2));

}



void FlockingSystem::addObstacle(sf::Vector2f pos, float radius)
{
	Obstacle newObstacle;
	newObstacle.position = pos;
	newObstacle.avoidRadius = radius;

	obstacleList.push_back(newObstacle);

	sf::CircleShape debugObstacleSeparationDistanceCircle;
	debugObstacleSeparationDistanceCircle.setRadius(radius);
	debugObstacleSeparationDistanceCircle.setOrigin(radius, radius);
	debugObstacleSeparationDistanceCircle.setOutlineColor(sf::Color(38, 106, 46, 255));
	debugObstacleSeparationDistanceCircle.setOutlineThickness(2);
	debugObstacleSeparationDistanceCircle.setFillColor(sf::Color(38, 106, 46, 10));
	debugObstacleSeparationDistanceCircle.setPosition(pos);
	debugObstacleSeparationDistanceCircles.push_back(debugObstacleSeparationDistanceCircle);
}

bool FlockingSystem::CheckFOV(int FirstBoid, int SecondBoid)
{

	sf::Vector2f currentBoidDirection = NormaliseVector(m_boids[FirstBoid]->getVelocity());
	sf::Vector2f directionToOtherBoid = NormaliseVector(m_boids[SecondBoid]->getPosition() - m_boids[FirstBoid]->getPosition());

	float angle_between = acos((currentBoidDirection.x * directionToOtherBoid.x) + (currentBoidDirection.y * directionToOtherBoid.y));



	if (angle_between > (FOV_angle*3.14/180)/2)
	{
		return false;
	}
	else
	{
    	return true;
	}

}

void FlockingSystem::UpdateObsticlePosition(int index, sf::Vector2f pos)
{
	obstacleList[index].position = pos;
	debugObstacleSeparationDistanceCircles[index].setPosition(pos);
}

void FlockingSystem::UpdateObsticleRadius(int index, float rad)
{
	obstacleList[index].avoidRadius = rad;
	debugObstacleSeparationDistanceCircles[index].setOrigin(rad, rad);
	debugObstacleSeparationDistanceCircles[index].setRadius(rad);
}

void FlockingSystem::AlignOn(bool On)
{
	align = On;
}

void FlockingSystem::CohereOn(bool On)
{
	cohere = On;
}

void FlockingSystem::SeparateOn(bool On)
{
	separate = On;
}

void FlockingSystem::FollowOn(bool On)
{
	followTarget = On;
}

void FlockingSystem::InBoudsOn(bool On)
{
	stayInBounds = On;
}

void FlockingSystem::SetTarget(sf::Vector2f targetPos)
{
	target = targetPos;
}

void FlockingSystem::setBounds(sf::RectangleShape & boundingRect)
{
	bounds = boundingRect;
}

void FlockingSystem::checkBounds(int index)
{
	if (m_boids[index]->getPosition().x > bounds.getPosition().x + bounds.getGlobalBounds().width || m_boids[index]->getPosition().x < bounds.getPosition().x)
	{
	m_boids[index]->setVelocity(sf::Vector2f(-m_boids[index]->getVelocity().x,m_boids[index]->getVelocity().y));
	}
	if (m_boids[index]->getPosition().y > bounds.getPosition().y + bounds.getGlobalBounds().height || m_boids[index]->getPosition().y < bounds.getPosition().y)
	{
	m_boids[index]->setVelocity(sf::Vector2f(m_boids[index]->getVelocity().x, -m_boids[index]->getVelocity().y));
	}

	if (m_boids[index]->getPosition().x > bounds.getPosition().x + bounds.getGlobalBounds().width)
	{
	m_boids[index]->setPosition(sf::Vector2f((bounds.getPosition().x + bounds.getGlobalBounds().width )-1, m_boids[index]->getPosition().y));
	}
	if (m_boids[index]->getPosition().x <  bounds.getPosition().x)
	{
	m_boids[index]->setPosition(sf::Vector2f(bounds.getPosition().x + 1, m_boids[index]->getPosition().y));
	}
	if (m_boids[index]->getPosition().y < bounds.getPosition().y)
	{
	m_boids[index]->setPosition(sf::Vector2f(m_boids[index]->getPosition().x, bounds.getPosition().y+1));
	}
	if (m_boids[index]->getPosition().y > bounds.getPosition().y + bounds.getGlobalBounds().height)
	{
	m_boids[index]->setPosition(sf::Vector2f(m_boids[index]->getPosition().x,( bounds.getPosition().y + bounds.getGlobalBounds().height)-1));
	}
}

void FlockingSystem::DebugDraw(sf::RenderWindow & window)
{
	for (int i = 0; i < debugFlockDistanceCircles.size(); i++)
	{
		if (align == true || cohere == true)
		{
			window.draw(debugFlockDistanceCircles[i]);
		}
		if (separate == true)
		{
			window.draw(debugSeparationDistanceCircles[i]);
		}
	
	}
	for (int i = 0; i < debugObstacleSeparationDistanceCircles.size(); i++)
	{
		if (separate == true)
		{
			window.draw(debugObstacleSeparationDistanceCircles[i]);
		}
	}

}





bool FlockingSystem::CheckRange(float range, sf::Vector2f object1, sf::Vector2f object2)
{
	float distanceX = object1.x - object2.x;
	float distanceY = object1.y - object2.y;
	float distance = sqrt((distanceX * distanceX) + (distanceY * distanceY));

	if (distance <= range)
	{
		return true;
	}
	else
		return false;
	
}

sf::Vector2f FlockingSystem::NormaliseVector(sf::Vector2f v1) {

	float TempLength(sqrt((v1.x * v1.x) + (v1.y * v1.y)));
	if (TempLength != 0)
	{
		sf::Vector2f normalised((v1.x / TempLength), (v1.y / TempLength));
		return normalised;
	}

	return sf::Vector2f(0, 0);

}



