#include "stdafx.h"
#include "FlockingSystem.h"


FlockingSystem::FlockingSystem()
{
}

void FlockingSystem::Add_Boid(sf::Vector2f pos,sf::Vector2f velocity)
{
	m_boids.push_back(new Boid(pos, velocity));
}

void FlockingSystem::Delete_Boid(int index)
{
	m_boids.erase(m_boids.begin() + index);
}

void FlockingSystem::Update(bool on, float flock_Distance , float Separation_Distance)
{
	flockRange = flock_Distance;
	separationRange = Separation_Distance;
	if (separationRange >= flockRange - 10)
	{
		separationRange = flockRange - 10;
	}


	for (int i = 0; i < m_boids.size(); i++)
	{

		m_boids[i]->Update();
		if (on == true)
		{
			sf::Vector2f alignment = sf::Vector2f(0, 0);
			alignment = ComputeAlignment(i);
			sf::Vector2f cohesion = sf::Vector2f(0, 0);
			cohesion = ComputeCohesion(i);
			sf::Vector2f seperation = sf::Vector2f(0, 0);
			seperation = ComputeSeparation(i);

			sf::Vector2f direction = NormaliseVector(m_boids[i]->getVelocity());

			
	

			float DirX = alignment.x + cohesion.x + seperation.x +direction.x;
			float DirY = alignment.y + cohesion.y + seperation.y +direction.y;

			m_boids[i]->setVelocity(sf::Vector2f(DirX, DirY));

		}


		if (m_boids[i]->getPosition().x > 900 || m_boids[i]->getPosition().x < 0)
		{
			m_boids[i]->setVelocity(sf::Vector2f(-m_boids[i]->getVelocity().x,m_boids[i]->getVelocity().y));
		}
		if (m_boids[i]->getPosition().y > 900 || m_boids[i]->getPosition().y < 0)
		{
			m_boids[i]->setVelocity(sf::Vector2f(m_boids[i]->getVelocity().x, -m_boids[i]->getVelocity().y));
		}
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

sf::Vector2f FlockingSystem::ComputeAlignment(int currentBoid)
{

	sf::Vector2f m_alignment;
	int neighbourcount = 0;


	for (int i = 0; i < m_boids.size(); i++)
	{
		if (i != currentBoid)
		{
			if (CheckRange(flockRange, m_boids[currentBoid]->getPosition(), m_boids[i]->getPosition()) == true && CheckRange(separationRange, m_boids[currentBoid]->getPosition(), m_boids[i]->getPosition()) == false)
			{
				m_alignment += m_boids[i]->getVelocity();
				neighbourcount++;
			}
			
		}
	}

	if (neighbourcount == 0)
	{

		return  sf::Vector2f(0, 0);
	}
	else
	{
		m_alignment.x = m_alignment.x / neighbourcount;
		m_alignment.y = m_alignment.y / neighbourcount;
		m_alignment = NormaliseVector(m_alignment);
		return m_alignment;
	}
}

sf::Vector2f FlockingSystem::ComputeCohesion(int currentBoid)
{
	sf::Vector2f m_Cohesion;
	int neighbourcount = 0;
	for (int i = 0; i < m_boids.size(); i++)
	{
		if (i != currentBoid)
		{
			if (CheckRange(flockRange, m_boids[currentBoid]->getPosition(), m_boids[i]->getPosition()) == true && CheckRange(separationRange, m_boids[currentBoid]->getPosition(), m_boids[i]->getPosition()) == false)
			{
				m_Cohesion += m_boids[i]->getPosition();
				neighbourcount++;
			}
			

		}
	}
	if (neighbourcount == 0)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		m_Cohesion.x = m_Cohesion.x / neighbourcount;
		m_Cohesion.y = m_Cohesion.y / neighbourcount;
		m_Cohesion = m_Cohesion - m_boids[currentBoid]->getPosition();;
		m_Cohesion = NormaliseVector(m_Cohesion);
		return m_Cohesion;

	}
}

sf::Vector2f FlockingSystem::ComputeSeparation(int currentBoid)
{
	sf::Vector2f m_Seperation;
	int neighbourcount = 0;

	for (int i = 0; i < m_boids.size(); i++)
	{
		if (i != currentBoid)
		{
			if (CheckRange(separationRange, m_boids[i]->getPosition(), m_boids[currentBoid]->getPosition()) == true)
			{
				m_Seperation += m_boids[i]->getPosition() - m_boids[currentBoid]->getPosition();
				neighbourcount++;
			}

		}
	}
	if (neighbourcount == 0)
	{
		return sf::Vector2f(0, 0);
	}
	else
	{
		m_Seperation.x = m_Seperation.x / neighbourcount;
		m_Seperation.y = m_Seperation.y / neighbourcount;
		m_Seperation.x *= -1;
		m_Seperation.y *= -1;

		m_Seperation = NormaliseVector(m_Seperation);
	
		return m_Seperation;
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



