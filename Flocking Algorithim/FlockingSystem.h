#ifndef FLOCKINGSYSTEM_H
#define FLOCKINGSYSTEM_H


#include "SFML/Graphics.hpp"
#include "stdafx.h"
#include "Boid.h"
class FlockingSystem {

public:
	FlockingSystem();	
	void Add_Boid(sf::Vector2f pos,sf::Vector2f velocity);
	void Delete_Boid(int index);
	void Update(bool on, float flock_Distance, float Separation_Distance);
	bool CheckRange(float range, sf::Vector2f object1, sf::Vector2f object2);
	sf::Vector2f NormaliseVector(sf::Vector2f v1);
	sf::Vector2f get_VelocityResult(int index);
	sf::Vector2f get_PositionResult(int index);
	sf::Vector2f ComputeAlignment(int currentBoid);
	sf::Vector2f ComputeCohesion(int currentBoid);
	sf::Vector2f ComputeSeparation(int currentBoid);

private:
	std::vector<Boid*> m_boids;
	float flockRange;
	float separationRange;
	//sf::Vector2f ComputeAlignment();
	//sf::Vector2f ComputeSeperation();
	//sf::Vector2f ComputeCohesion();
	
};

#endif


