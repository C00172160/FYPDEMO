#include "stdafx.h"
#include "FlockingSystem.h"


FlockingSystem::FlockingSystem()
{
	
	cohere = true;
	separate = true;
	align = true;
}

void FlockingSystem::Add_Boid(sf::Vector2f pos,sf::Vector2f velocity)
{
	m_boids.push_back(new Boid(pos, velocity));
	sf::CircleShape debugflockdistanceCircle;
	sf::CircleShape debugseparationdistanceCircle;


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
	flockRange = flock_Distance;
	separationRange = Separation_Distance;
	FOV_angle = fov_angle;
	
	for (int i = 0; i < m_boids.size(); i++)
	{

	

		alignNeighbourCount = 0;
		cohereNeighbourCount = 0;
		separateNeighbourCount = 0;
		obsticleNeighbourCount = 0;

		alignment = sf::Vector2f(0, 0);
		cohesion = sf::Vector2f(0, 0);
		seperation = sf::Vector2f(0, 0);
		obstacleAvoidance = sf::Vector2f(0, 0);

		m_boids[i]->Update();//moves the boids
	     


		for (int j = 0; j <  m_boids.size(); j++)
		{
			if (align == true)
			{
				ComputeAlignment(i, j);
			}
			if (cohere == true)
			{
				ComputeCohesion(i, j);
			}
			if (separate == true)
			{
				ComputeSeparation(i, j);
			}
		}
		for (int j = 0 ; j < obstacleList.size(); j++)
		{
			ComputeObsticleAvoidance(i, j);
		}

		ComputeFlock(i);
	

		if (m_boids[i]->getPosition().x > 1100 || m_boids[i]->getPosition().x < 200)
		{
			m_boids[i]->setVelocity(sf::Vector2f(-m_boids[i]->getVelocity().x,m_boids[i]->getVelocity().y));
		}
		if (m_boids[i]->getPosition().y > 900 || m_boids[i]->getPosition().y < 0)
		{
			m_boids[i]->setVelocity(sf::Vector2f(m_boids[i]->getVelocity().x, -m_boids[i]->getVelocity().y));
		}

		if (m_boids[i]->getPosition().x > 1100)
		{
			m_boids[i]->setPosition(sf::Vector2f(1099, m_boids[i]->getPosition().y));
		}
		if (m_boids[i]->getPosition().x < 200)
		{
			m_boids[i]->setPosition(sf::Vector2f(201, m_boids[i]->getPosition().y));
		}
		if (m_boids[i]->getPosition().y < 0)
		{
			m_boids[i]->setPosition(sf::Vector2f(m_boids[i]->getPosition().x,1));
		}
		if (m_boids[i]->getPosition().y > 900)
		{
			m_boids[i]->setPosition(sf::Vector2f(m_boids[i]->getPosition().x, 899));
		}

		debugFlockDistanceCircles[i].setRadius(flock_Distance);
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

		if (otherBoid != currentBoid)
		{
			if (CheckRange(flockRange, m_boids[currentBoid]->getPosition(), m_boids[otherBoid]->getPosition()) == true )// CheckRange(separationRange, m_boids[currentBoid]->getPosition(), m_boids[i]->getPosition()) == false)
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
		if (otherBoid != currentBoid)
		{
			if (CheckRange(flockRange, m_boids[currentBoid]->getPosition(), m_boids[otherBoid]->getPosition()) == true && CheckRange(separationRange+10, m_boids[currentBoid]->getPosition(), m_boids[otherBoid]->getPosition()) == false)
			{
				if (CheckFOV(currentBoid, otherBoid) == true)
				{
					cohesion += m_boids[otherBoid]->getPosition();
					cohereNeighbourCount++;
				}
			}
			

		}

}

void FlockingSystem::ComputeSeparation(int currentBoid, int otherBoid)
{
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
		if (CheckRange(obstacleList[obstacle].avoidRadius, obstacleList[obstacle].position, m_boids[currentBoid]->getPosition()) == true)
		{
			
				obstacleAvoidance += obstacleList[obstacle].position - m_boids[currentBoid]->getPosition();
				obsticleNeighbourCount++;

			
		}

	
}

void FlockingSystem::ComputeFlock(int i)
{
	if (alignNeighbourCount == 0)
	{

		alignment = sf::Vector2f(0, 0);
	}
	else
	{
		alignment.x = alignment.x / alignNeighbourCount;
		alignment.y = alignment.y / alignNeighbourCount;
		alignment = NormaliseVector(alignment);
	}

	if (cohereNeighbourCount == 0)
	{
		cohesion = sf::Vector2f(0, 0);
	}
	else
	{
		cohesion.x = cohesion.x / cohereNeighbourCount;
		cohesion.y = cohesion.y / cohereNeighbourCount;
		cohesion = cohesion - m_boids[i]->getPosition();;
		cohesion = NormaliseVector(cohesion);
	}

	if (separateNeighbourCount == 0)
	{
		seperation = sf::Vector2f(0, 0);
	}
	else
	{
		seperation.x = seperation.x / separateNeighbourCount;
		seperation.y = seperation.y / separateNeighbourCount;
		seperation.x *= -1;
		seperation.y *= -1;

		seperation = NormaliseVector(seperation);


	}

	if (obsticleNeighbourCount == 0)
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

	direction = NormaliseVector(m_boids[i]->getVelocity());


	float DirX = alignment.x + cohesion.x + direction.x + seperation.x + obstacleAvoidance.x;
	float DirY = alignment.y + cohesion.y + direction.y + seperation.y + obstacleAvoidance.y;


	m_boids[i]->setVelocity(sf::Vector2f(DirX * 2, DirY * 2));

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



