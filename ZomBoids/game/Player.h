#pragma once
#include "Game.h"


class Player : public CSprite
{
public:
	enum STATE { RELOAD, IDLE, MOVE, SHOOT, NONE};
	enum WEAPON {PISTOL,RIFLE,SHOTGUN};
	float SPEED = 100;
	CSpriteList* Horde;
	CSpriteList* MapObjects;
	CSpriteList* Bullets;

private:
	STATE state;
	WEAPON weapon;
	float health;
	int pistol_ammo;
	int rifle_ammo;
	int shotgun_ammo;
	int pistol_clip;
	int rifle_clip;
	int shotgun_clip;
	CVector front = CVector(0, 1);
	CVector back = CVector(0, -1);
	CVector left = CVector(-1, 0);
	CVector right = CVector(1, 0);
	int playerSpeed = 100;
	bool isReloading;
	bool isMoving;
	bool canSprint;
	float stamina;

public:
	Player() : CSprite() {

	}
	//~Player(void);

	//void OnUpdate(Uint32 time, Uint32 deltaTime);
	void OnStart();
	void OnUpdate(Uint32 time, Uint32 deltaTime);
	void FireWeapon();
	void Reload();
	int GetWeaponAmmo();
	void reset();
	STATE GetState() { return state; }
	void ChangeState(STATE s);
	void ChangeWeapon(WEAPON w) { if (w == weapon) return;  weapon = w;  }
	int getWeaponClip();
	int getStamina() { return stamina; }
	WEAPON GetWeapon() { return weapon; }
	void setReloading(){ isReloading = true; }
	bool getReloading(){ return isReloading; }
	float GetHealth() { return health; }
	int GetPistolAmmo() { return pistol_ammo + pistol_clip; }
	int GetRifleAmmo() { return rifle_ammo + rifle_clip; }
	int GetShotgunAmmo() { return shotgun_ammo + shotgun_clip; }
	void SetHealth(float h) { health = h; }
	void addPistolAmmo(int a) { pistol_ammo += a; }
	void addRifleAmmo(int a) { rifle_ammo += a; }
	void addShotgunAmmo(int a) { shotgun_ammo += a; }
};
