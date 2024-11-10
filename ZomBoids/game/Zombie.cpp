#include "stdafx.h"
#include "Zombie.h"
#include "MyGame.h"

extern CMyGame game;

CVector CZombie::c_attraction;
//bool CZombie::c_bAttraction = false;
//CVector CZombie::lastSeenPlayer;

CZombie::CZombie(Sint16 x, Sint16 y, Uint32 time, CSpriteList& boids, CSpriteList& pObstacles,CSprite& pPlayer)
	: CSprite(x, y, "Zombie_test.bmp", CColor::White() , time)
{
	Horde = &boids;
	player = &pPlayer;
	MapObjects = &pObstacles;
	wanderTarget = CVector(0, 0);
	canSeePlayer = false;
	LoadImage("idle_test.bmp", "idle", CColor::White());
	LoadAnimation("Zombie_Move.png", "move", CSprite::Sheet(17, 1).Row(1).From(0).To(16), CColor::White());
	LoadAnimation("Zombie_Attack.png", "attack", CSprite::Sheet(9, 1).Row(1).From(0).To(8), CColor::White());
	LoadAnimation("Zombie_Death.png", "death", CSprite::Sheet(6, 1).Row(1).From(0).To(5), CColor::White());

}

CZombie::~CZombie()
{
}


bool Intersection(CVector a, CVector b, CVector c, CVector d, float& k1, float& k2)
{
	CVector v1 = b - a;
	CVector v2 = d - c;
	CVector con = c - a;
	float det = v1.m_x * v2.m_y - v1.m_y * v2.m_x;
	if (det != 0)
	{
		k1 = (v2.m_y * con.m_x - v2.m_x * con.m_y) / det;
		k2 = (v1.m_y * con.m_x - v1.m_x * con.m_y) / det;
		return true;
	}
	else
		return false;
}

// returns true is the line segments a-b and c-d intersect
bool Intersection(CVector a, CVector b, CVector c, CVector d)
{
	float k1, k2;
	if (!Intersection(a, b, c, d, k1, k2))
		return false;
	return k1 >= 0 && k1 <= 1.f && k2 >= 0 && k2 <= 1.f;
}

/*bool CZombie::LineOfSight(CVector playerPos) {
	for each (CSprite * pSprite in *MapObjects) {

		bool v1 = Intersection(GetPos(), playerPos, CVector(pSprite->GetLeft(), pSprite->GetTop()), CVector(pSprite->GetRight(), pSprite->GetTop()));
		bool v2 = Intersection(GetPos(), playerPos, CVector(pSprite->GetRight(), pSprite->GetTop()), CVector(pSprite->GetRight(), pSprite->GetBottom()));
		if (v1 || v2) {
			canSeePlayer = false;
            break;
        }
        else {
            canSeePlayer = true;
			lastSeenPlayer = playerPos;
        }
	}
	return canSeePlayer;
}*/

void CZombie::LineOfSight() {
	for each (CSprite * pSprite in *MapObjects) {
		bool v1 = Intersection(GetPos(), player->GetPos(), CVector(pSprite->GetLeft(), pSprite->GetTop()), CVector(pSprite->GetRight(), pSprite->GetTop()));
		bool v2 = Intersection(GetPos(), player->GetPos(), CVector(pSprite->GetRight(), pSprite->GetTop()), CVector(pSprite->GetRight(), pSprite->GetBottom()));
		if (v1 || v2) {
			canSeePlayer = false;
            break;
        }
        else {
            canSeePlayer = true;
        }
	}
}


void CZombie::ChangeState(STATE s) {
	if (state == s) return;

	state = s;

	switch (state) {
	case IDLE:
		SetImage("idle");
		break;
	case ATTACK:
		SetAnimationKeepSize("attack", 24);
		SetSpeed(50);
		break;
	case CHASE:
		SetAnimationKeepSize("move", 12);
		break;
	case WANDER:
		SetAnimationKeepSize("move", 12);	
		break;
	case DEAD:
		SetAnimationKeepSize("death", 12);
		SetSpeed(0);
		break;
	}
}


int CZombie::GetZombiesInRadius() {
	int count = 0; 
	for each (CZombie * pBoid in *Horde) {
		if (pBoid != this && Distance(pBoid) <= 300 && Distance(pBoid) > 0)
		{
			count++;
		}
	}
	zomInRadius = count;
	return zomInRadius;
}

CFVector CZombie::separate(double radius)
{
	CFVector vec;
	int count = 0;
	for each (CZombie * pBoid in *Horde)
		if (pBoid != this && Distance(pBoid) <= radius && Distance(pBoid) > 0)
		{
			vec += CFVector(GetPosition() - pBoid->GetPosition()).SetLength(100);
			count++;
		}

	if (count == 0)
		return CFVector(0, 0);
	else
		return vec / count;
}

CFVector CZombie::align(double radius)
{
	CFVector vec;
	int count = 0;
	for each (CZombie * pBoid in *Horde)
		if (pBoid != this && Distance(pBoid) <= radius && Distance(pBoid) > 0)
		{
			vec += CFVector(pBoid->GetXVelocity(), pBoid->GetYVelocity());
			count++;
		}

	if (count == 0)
		return CFVector(0, 0);
	else
		return vec / count;
}

CFVector CZombie::cohesion(double radius)
{
	CFVector vec;
	int count = 0;
	for each (CZombie * pBoid in *Horde)
		if (pBoid != this && Distance(pBoid) <= radius && Distance(pBoid) > 0)
		{
			vec += pBoid->GetPosition();
			count++;
		}

	if (count == 0)
		return CFVector(0, 0);
	else
		return vec / count - GetPosition();
}

CFVector CZombie::border(double radius)
{
	CFVector vec;
	if (GetX() <= 2 * radius)
		vec.m_x = (2 * radius - GetX()) * abs(GetXVelocity()) / radius;
	if (GetY() <= 2 * radius)
		vec.m_y = (2 * radius - GetY()) * abs(GetYVelocity()) / radius;
	if (GetX() >= 1300 - 2 * radius)
		vec.m_x = (1300 - 2 * radius - GetX()) * abs(GetXVelocity()) / radius;
	if (GetY() >= 1000 - 2 * radius)
		vec.m_y = (1000 - 2 * radius - GetY()) * abs(GetYVelocity()) / radius;
	return vec.Limit(100);
}

CFVector CZombie::obstacle(double radius)
{
	CFVector vec;
	int count = 0;


	CRectangle rect;
	for each (CSprite * pSprite in *MapObjects)
	{
		pSprite->GetBoundingRect(rect);
		rect.Grow(2 * (Uint16)radius);

		if (rect.Contains(GetCenter()))
		{
			if (GetX() < pSprite->GetLeft())
				vec.m_x = (pSprite->GetLeft() - 2 * radius - GetX()) * abs(GetXVelocity()) / radius;
			if (GetX() > pSprite->GetRight())
				vec.m_x = (pSprite->GetRight() + 2 * radius - GetX()) * abs(GetXVelocity()) / radius;
			if (GetY() > pSprite->GetTop())
				vec.m_y = (pSprite->GetTop() + 2 * radius - GetY()) * abs(GetYVelocity()) / radius;
			if (GetY() < pSprite->GetBottom())
				vec.m_y = (pSprite->GetBottom() - 2 * radius - GetY()) * abs(GetYVelocity()) / radius;

			count++;
		}
	}

	if (count == 0)
		return CFVector(0, 0);
	else
		return (vec / count).Limit(100);
}

float random()
{
	return ((float)rand() / (float)RAND_MAX);
}

CVector CZombie::wander(float speed, float wanderDist, float wanderRadius) {
	//wanderTarget = CVector(random() - 0.5f, random() - 0.5f);
	wanderTarget += CVector(random() - 0.5f, random() - 0.5f) * wanderDist;
	wanderTarget = wanderTarget.Normalize() * wanderRadius;
	CVector target = GetPosition() + wanderTarget;
	CVector desired = target - GetPosition();
	desired = desired.Normalize() * speed;
	return desired - GetVelocity();
}

CFVector CZombie::attract(CFVector p)
{
	return (p - GetPosition()).SetLength(100);
}

void CZombie::OnUpdate(Uint32 time, Uint32 deltaTime)
{
	double r = 18;
	CFVector sep = separate(r);
	CFVector ali = align(r * 2);
	CFVector coh = cohesion(r * 2);
	sep *= 0.5;
	ali *= 0.0;
	coh *= 0.0;
	CFVector vec = sep + ali + coh;

	CFVector bor = border(r);
	CFVector obs = obstacle(r);
	bor *= 2.0;
	obs *= 2.0;
	vec += bor + obs;
	CFVector att;
	if (IsAttraction()) att = attract(GetAttraction());
	att *= 0.2;
	vec += att;
	
	if (state == DEAD && GetCurrentAnimationFrame() == 5) {
		game.spawnAmmoBox(GetPos());
	}

	switch (state) {
	case CHASE:
		LineOfSight();
		GetZombiesInRadius();
		if (!canSeePlayer && zomInRadius < 3) {
			ChangeState(WANDER);
		}
		else if (!canSeePlayer && zomInRadius >= 3) {
			SetAttraction(lastSeenPlayer);
		}
		else if (canSeePlayer) {
			SetAttraction(player->GetPos());
			lastSeenPlayer = player->GetPos();
		}
		SetVelocity(GetXVelocity() + vec.GetX(), GetYVelocity() + vec.GetY());
		if (player->GetPos().Distance(GetPos()) < 50&& HitTest(player)) {
			ChangeState(ATTACK);
		}
		break;
	case ATTACK:
		if (GetCurrentAnimationFrame() == 5 && HitTest(player) && game.player.GetHealth() > 0 ) { game.player.SetHealth(game.player.GetHealth() - 25); ChangeState(CHASE); }
		if (GetCurrentAnimationFrame() == 8) {
			ChangeState(CHASE);
		}
		break;
	case WANDER:
		LineOfSight();
		GetZombiesInRadius();
		if (zomInRadius < 3 && !canSeePlayer) {
			c_bAttraction = false;
			steer = wander(100, 100, 10);
			obs *= 1.5;
			bor *= 1.5;
			steer += bor + obs;
			dirVector = CVector(GetXVelocity() + steer.GetX(), GetYVelocity() + steer.GetY());
			SetVelocity(GetXVelocity() + vec.GetX(), GetYVelocity() + vec.GetY());
			Accelerate(dirVector * (float)deltaTime / 1000.0f);
		}
		else {
            ChangeState(CHASE);
        }
		break;
	case DEAD:
		if (GetCurrentAnimationFrame() == 5) {
			
			Delete();
		}
	}
	if (GetSpeed() > 100 && state != ATTACK)
		SetSpeed(110);

	SetRotation(GetDirection());

	CSprite::OnUpdate(time, deltaTime);
}