#ifndef FLOCKINGSYSTEM_H
#define FLOCKINGSYSTEM_H


#include "SFML/Graphics.hpp"
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
	void ComputeFlock(int i);//adds up all the factors that contribute to the flock algorithim
	void addObstacle(sf::Vector2f pos, float radius);//adds obstacles to the vector of obstacles
	bool CheckFOV(int FirstBoid,int SecondBoid);//checks if a boid ca "see" another boid
	void UpdateObsticlePosition(int index,sf::Vector2f pos);//allows the user to update an obstacles position
	void UpdateObsticleRadius(int index, float rad);//allows the user to change the avoidance radius on an obstacle
	void AlignOn(bool On);//set weither the flocking algorithim will compute the alignment component
	void CohereOn(bool On);//set weither the flocking algorithim will compute the cohesion component
	void SeparateOn(bool On);//set weither the flocking algorithim will compute the separate component
	void FollowOn(bool On);////set weither the boids follow a target.
	void InBoudsOn(bool On);//sets weither the boids will stay witin a boundry
	void SetTarget(sf::Vector2f targetPos);//sets the target to follow
	void setBounds(sf::RectangleShape & boundingRect);//sets the containing boundry for the boids
	void checkBounds(int index);//check if the boids are within the bondry and takes approiate action
	void DebugDraw(sf::RenderWindow & window);//draw all the circles that let you visualise the flocking distance,separation disance
private:

	struct Obstacle {//holds the info for an obstacle
		sf::Vector2f position;
		float avoidRadius;
	};

	std::vector<Boid*> m_boids;//holds the boids
	float flockRange;//the max distance from a boid that another boid needs to be considered for flocking 
	float separationRange;//the max distance that a boid will stay away from another boid
	bool cohere, align, separate,followTarget,stayInBounds;
	float FOV_angle;//the angle in witch a boid can "see" other boids
	sf::RectangleShape bounds;//a rectangle representing the "container" in witch boids move around
	sf::Vector2f alignment;//the alignment component of the flocking algorithim
	sf::Vector2f cohesion;//the cohesion component of the flocking algorithim
	sf::Vector2f seperation;//the separation component of the flocking algorithim
	sf::Vector2f direction;//the direction the boid is travelling in. it is added to the flocking algorithim to keep the boids moving
	sf::Vector2f obstacleAvoidance;//added to the flocking algorithim to get the boids to avoid obstacles. similar to the separation component except using obstacles instead of other boids
	sf::Vector2f target;//the position of the target to follow
	sf::Vector2f targetDirection;//direction from target to boid. added to the flocking algorithim to get boids to follow a target
	sf::Vector2f flockingVector;//the combined result of all components of the algorithim

	int cohereNeighbourCount;//counts how many boids are within cohesion distance,used to find the average direction to move towards
	int alignNeighbourCount;//counts how many boids are within alignment distance
	int separateNeighbourCount;//counts how many boids are within separate  distance
	int obsticleNeighbourCount;//counts how many obstacles are within avoidace distance

	std::vector<sf::CircleShape> debugFlockDistanceCircles;	//holds the circles that represent flocking distance
	std::vector<sf::CircleShape> debugSeparationDistanceCircles;//holds the circles that represent the separation distace 
	std::vector<sf::CircleShape> debugObstacleSeparationDistanceCircles;//holds the circles that represent avoidace radius of obstacles
	std::vector<Obstacle> obstacleList;//olds all obstacles

	
};

#endif


