#pragma once


#define HORDELIM 40
#include "Player.h"
#include <random>


class CMyGame : public CGame
{
	// my own member functions

void SetupLevel1();
void SetupLevel2();

public:
	CMyGame(void);
	~CMyGame(void);
	Player player;	// player sprite
	CSprite startScreen;
	CSprite gameworld;
	CSpriteList BulletList;
	CSpriteList Zombies;
	CSpriteList CollisionBoxes;
	CSpriteList ammoboxes;
	CSpriteRect PlayerCollisionBox;
	CSprite Bullet;
	CSprite GameOverText;
	int level;
	int spawnTimer;
	float deathTimer;
	float timer;
	vector<CVector> spawnPoints;
	CVector mousePos;
	// Per-Frame Callback Funtions (must be implemented!)
	virtual void OnUpdate();
	virtual void OnDraw(CGraphics* g);
	

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnStartGame();
	virtual void OnStartLevel(Sint16 nLevel);
	virtual void zombieSpawn();
	virtual void OnGameOver();
	virtual void OnTerminate();
	virtual void spawnAmmoBox(CVector pos);

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);
};
