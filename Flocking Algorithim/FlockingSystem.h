#ifndef FLOCKINGSYSTEM_H
#define FLOCKINGSYSTEM_H


#include "SFML/Graphics.hpp"
#include "stdafx.h"
#include "Boid.h"
class FlockingSystem {

public:
	FlockingSystem();	
	void Add_Boid(sf::Vector2f pos,sf::Vector2f velocity);//add a boid to the internal system for processing
	void Delete_Boid(int index);//removes the boid
	void Update( float flock_Distance, float Separation_Distance, float fov_angle);//moves all the boids and preforms the flocking algorthom on them
	bool CheckRange(float range, sf::Vector2f object1, sf::Vector2f object2);//checks how if a point is a certain distance from another
	sf::Vector2f NormaliseVector(sf::Vector2f v1);
	sf::Vector2f get_VelocityResult(int index);// returns the new velocity of the "flocked" velocity
	sf::Vector2f get_PositionResult(int index);//returns the position of the flocked position
	void ComputeAlignment(int currentBoid,int otherBoid);//computes the alignment portion of the flocking algorithim
	void ComputeCohesion(int currentBoid, int otherBoid);//computes the Cohesion portion of the flocking algorithim
	void ComputeSeparation(int currentBoid, int otherBoid);//computes the Separation portion of the flocking algorithim
	void ComputeObsticleAvoidance(int currentBoid, int obstacle);//computes the Separation portion of the flocking algorithim
	void ComputeFlock(int i);
	void addObstacle(sf::Vector2f pos, float radius);
	bool CheckFOV(int FirstBoid,int SecondBoid);
	void UpdateObsticlePosition(int index,sf::Vector2f pos);
	void UpdateObsticleRadius(int index, float rad);
	void AlignOn(bool On);
	void CohereOn(bool On);
	void SeparateOn(bool On);
	void FollowOn(bool On);
	void SetTarget(sf::Vector2f targetPos);

	void DebugDraw(sf::RenderWindow & window);
private:

	struct Obstacle {
		sf::Vector2f position;
		float avoidRadius;
	};

	std::vector<Boid*> m_boids;
	float flockRange;
	float separationRange;
	bool cohere, align, separate,followTarget;
	float FOV_angle = 180;

	sf::Vector2f alignment;
	sf::Vector2f cohesion;
	sf::Vector2f seperation;
	sf::Vector2f direction;
	sf::Vector2f obstacleAvoidance;
	sf::Vector2f target;
	sf::Vector2f targetDirection;
	int cohereNeighbourCount;
	int alignNeighbourCount;
	int separateNeighbourCount;
	int obsticleNeighbourCount;

	std::vector<sf::CircleShape> debugFlockDistanceCircles;	
	std::vector<sf::CircleShape> debugSeparationDistanceCircles;
	std::vector<sf::CircleShape> debugObstacleSeparationDistanceCircles;
	std::vector<Obstacle> obstacleList;

	
};

#endif


