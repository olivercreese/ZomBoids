#pragma once

#include "CFVector.h"
#include "Game.h"

class CZombie : public CSprite
{

public:
	CSpriteList* Horde;
	CSpriteList* MapObjects;
	CSprite* player;
	CSpriteList* Bullets;
	enum STATE {IDLE,ATTACK,CHASE,WANDER,DEAD};

private:
	static CVector c_attraction;
	bool c_bAttraction;
	CVector lastSeenPlayer;
	float distanceToPlayer;
	int zomInRadius;
	CFVector steer;
	CVector dirVector;
	bool canSeePlayer;
	CVector wanderTarget;	// wander target position (relative)
	STATE state;
	int counter;

public:
	CZombie(Sint16 x, Sint16 y, Uint32 time, CSpriteList& boids, CSpriteList& pObstacles, CSprite& pPlayer);
	~CZombie(void);


	void SetAttraction(CVector attr) { c_bAttraction = true; c_attraction = attr; }
	void SetAttraction() { c_bAttraction = false; }
	bool IsAttraction() { return c_bAttraction; }
	float GetDistanceToPlayer(CVector playerPos) {
		distanceToPlayer = GetPosition().Distance(playerPos);
		return distanceToPlayer;
	}
	int GetZombiesInRadius();
	
	//bool LineOfSight(CVector playerPos);
	void LineOfSight();
	CVector GetAttraction() { return c_attraction; }
	CVector wander(float speed, float wanderDist, float wanderRadius);
	STATE GetState() { return state; }
	void ChangeState(STATE s);

	double Distance(CZombie* pZom) { return GetPosition().Distance(pZom->GetPosition()); }

	CFVector separate(double radius);
	CFVector align(double radius);
	CFVector cohesion(double radius);

	CFVector border(double radius);
	CFVector obstacle(double radius);

	CFVector attract(CFVector p);
	virtual void OnUpdate(Uint32 time, Uint32 deltaTime);
};

