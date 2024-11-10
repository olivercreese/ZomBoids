#include "stdafx.h"
#include "MyGame.h"
#include "Zombie.h"
#include <iostream>

CMyGame::CMyGame(void) :
	startScreen(GetWidth(), GetHeight(), "NewStartScreen.png", 0),PlayerCollisionBox(0, 0, 20, 20,CColor::White(), 0)
{
	player.OnStart();
}

CMyGame::~CMyGame(void)
{
}

/////////////////////////////////////////////////////
// Per-Frame Callback Funtions (must be implemented!)

void CMyGame::OnUpdate()
{
	// do not run game logic when in menu mode
	if (IsMenuMode()) return;
	if (IsGameOverMode()) return;
	Uint32 t = GetTime();
	for each (CZombie * pZom in Zombies) {
		pZom->Update(t); // zombie update code is run 
		for (CSprite* pBullet : BulletList)
		{
			if (pZom->HitTest(pBullet)) {
                pZom->ChangeState(CZombie::DEAD); // zombie is killed
                pBullet->Delete();
            }
        }
	} 
	Zombies.delete_if(deleted); // delete dead zombies
	zombieSpawn();// zombie spawner function
	if (player.GetHealth() <= 0) { // player death
		player.SetImage("playerDead"); 
		player.SetRotation(0); 
		player.SetSize(100, 100); 
		deathTimer+= (float)GetDeltaTime() / 1000; // game runs for a few more seconds after player death
		if (deathTimer > 5) {
            NewGame(); // game restarts
            Zombies.clear(); // zombies are cleared
            deathTimer = 0;
			player.reset(); // player stats reset
        }
    }
	else timer += (float)GetDeltaTime() / 1000; // game timer in seconds

	for (CSprite* pBullet : BulletList)
	{
        pBullet->Update(t); // bullet update code is run
		if (pBullet->GetX() > 1600 || pBullet->GetX() < 0 || pBullet->GetY() > 1000 || pBullet->GetY() < 0)
		{
			pBullet->Delete();
		}
		for (CSprite* pbox : CollisionBoxes)
		{
			if (pBullet->HitTest(pbox)) {
                pBullet->Delete();
            }
		} // bullets deleted when they hit a wall or go out of bounds
    }
	BulletList.delete_if(deleted);

	for (CSprite* pBox : ammoboxes) {
        if (pBox->HitTest(&PlayerCollisionBox)) {
            if (pBox->GetStatus() == 1) {
                player.addPistolAmmo(10); // ammo box gives player ammo on collision
            }
            else if (pBox->GetStatus() == 2) {
                player.addShotgunAmmo(5);
            }
            else if (pBox->GetStatus() == 3) {
                player.addRifleAmmo(5);
            }
            pBox->Delete();
        }
    }
	ammoboxes.delete_if(deleted); // ammo boxes deleted after collision

	PlayerCollisionBox.SetPos(player.GetPos()); // player collision box follows player position
	PlayerCollisionBox.Update(GetTime()); 
	if (player.GetHealth() > 0)player.Update(t); // player update code is run only if player is alive
}


void CMyGame::OnDraw(CGraphics* g)
{
	// draw menu when in menu mode
	if (IsMenuMode())
	{
		startScreen.Draw(g);
		startScreen.SetPos(GetWidth() / 2, GetHeight() / 2);
		startScreen.SetSize(1200, 1000);
		return;
	}

	gameworld.Draw(g);
	for (CSprite* pBullet : BulletList)
        pBullet->Draw(g);
	for (CSprite* pbox : CollisionBoxes)
		//pbox->Draw(g);

	for (CSprite* pbox : ammoboxes)
		pbox->Draw(g);


	player.Draw(g);

	*g << font(28) << color(CColor::Red()) << xy(1200, 50) << player.getWeaponClip();
	*g << font(18) << color(CColor::Red()) << xy(1200, 20) << player.GetWeaponAmmo();
	*g << font(30) << color(CColor::Blue()) << xy(1200, 950) << (int)timer;
	*g << font(20) << color(CColor::Red()) << xy(70, 5) << "HEALTH:";
	*g << font(20) << color(CColor::Red()) << xy(700, 5) << "STAMINA:";

	int healthbarsize = player.GetHealth() * 3; // health bar size is proportional to player health

	CSpriteRect HealthBar(200, 12, healthbarsize, 20, CColor::Blue(), CColor::Black(), 0);
	HealthBar.SetX(200 + healthbarsize / 2); //keeps left edge of health bar at 200
	HealthBar.Draw(g);

	int staminabarsize = player.getStamina() * 3; 

	CSpriteRect StaminaBar(800, 12, staminabarsize, 20, CColor::Green(), CColor::Black(), 0);
	StaminaBar.SetX(800 + staminabarsize / 2);
	StaminaBar.Draw(g);

	for each (CZombie * pZomBoid in Zombies) {
		pZomBoid->Draw(g);
	}

	if (player.GetHealth() <= 0) { // game over screen
		GameOverText.Draw(g);
		GameOverText.SetPos(GetWidth() / 2, GetHeight() / 2);
		*g << font(40) << color(CColor::Red()) << xy(450, 400) << "You Survived " << ((int)timer/60) <<"m "<<((int)timer%60) << "s ";
	}


}

void CMyGame::OnInitialize()
{
	// loading the game map
	gameworld.LoadImage("map1.png", "map1");
	gameworld.SetImage("map1"); gameworld.SetPosition(GetWidth() / 2, GetHeight() / 2);
	gameworld.SetSize(1300, 1000);
	GameOverText.LoadImage("GameOver.png", "GameOver",CColor::Black());
	GameOverText.SetImage("GameOver");
	Bullet.LoadImage("bullet.bmp", "bullet", CColor::Green());
	Bullet.SetImage("bullet");
	spawnPoints.push_back(CVector(0, 500)); //vector of spawn points for zombies
	spawnPoints.push_back(CVector(170, 1000));
	spawnPoints.push_back(CVector(1296, 671));
	spawnPoints.push_back(CVector(800, 0));
	for (int i = 0; i < HORDELIM; i++) //pre initialising zombies to avoid lag
	{
		int randSpawnIndex = 0 + rand() % 3;
		CVector zomSpawn = spawnPoints[randSpawnIndex];
		CZombie* pzombie = new CZombie(zomSpawn.m_x, zomSpawn.m_y, GetTime(), Zombies, CollisionBoxes, player);
		pzombie->SetSize(50, 50);
		pzombie->SetDirection(rand() % 360);
		pzombie->SetSpeed(100);
		pzombie->ChangeState(CZombie::CHASE);
		Zombies.push_back(pzombie);
	}
	CSprite* pSprite; //collision boxes for the map

	pSprite = new CSpriteRect(985, 260, 90, 200, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1122, 361, 30, 80, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1227, 357, 30, 80, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1123, 209, 30, 80, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1228, 190, 30, 110, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1128, 69, 30, 80, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1235, 69, 100, 26, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(696, 123, 55, 120, CColor::White(), 0);
	pSprite->SetRotation(-20);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(619, 276, 49, 16, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(502, 235, 11, 20, CColor::White(), 0);
	pSprite->SetRotation(45);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(550, 181, 45, 13, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(215, 72, 90, 5, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(639, 488, 97, 30, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(415, 340, 60, 60, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(307, 386, 24, 83, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(469, 432, 85, 25, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(131, 189, 35, 97, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(13, 51, 26, 100, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(13, 316, 26, 249, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(22, 508, 5, 90, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(335, 508, 5, 90, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(537, 698, 5, 90, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(336, 699, 5, 90, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1084, 505, 5, 80, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1286, 506, 5, 90, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(988, 635, 148, 45, CColor::White(), 0);
	pSprite->SetRotation(-15);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1131, 805, 62, 37, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1180, 944, 225, 140, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(1031, 940, 54, 10, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(860, 924, 114, 26, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(550, 904, 455, 235, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(884, 799, 90, 5, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(886, 970, 90, 5, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(358, 230, 96, 30, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(278, 894, 48, 48, CColor::White(), 0);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(211, 613, 45, 70, CColor::White(), 0);
	pSprite->SetRotation(50);
	CollisionBoxes.push_back(pSprite);
	pSprite = new CSpriteRect(151, 748, 24, 70, CColor::White(), 0);
	pSprite->SetRotation(-17);
	CollisionBoxes.push_back(pSprite);
}


void CMyGame::OnStartGame()
{
	//initialize player stats
    player.SetPosition(100, 50);
	player.ChangeState(Player::IDLE);
	player.SetHealth(100);
	player.SetSize(50, 50);
	player.SetMotion(0, 0);
	timer = 0;
}

void CMyGame::zombieSpawn() { // zombie spawner function
	if (Zombies.size() < HORDELIM) {
		spawnTimer++; // spawn timer which spawns zombies every 60 frames
		if (spawnTimer > 60) {
			int randSpawnIndex = 0 + rand() % 3;
			CVector zomSpawn = spawnPoints[randSpawnIndex];
			CZombie* pzombie = new CZombie(zomSpawn.m_x, zomSpawn.m_y, GetTime(), Zombies, CollisionBoxes, player);
			pzombie->SetSize(50, 50);
			pzombie->SetDirection(rand() % 360);
			pzombie->SetSpeed(100);
			pzombie->ChangeState(CZombie::CHASE);
			Zombies.push_back(pzombie);
			spawnTimer = 0;
		}
	}
}

void CMyGame::spawnAmmoBox(CVector pos) { // ammo box spawner function
	if (0 + rand() % (20 +player.GetPistolAmmo()) == 0) { // ammo box spawn chance is inversely proportional to player ammo of that type
		CSprite* pBox = new CSprite(pos.GetX(), pos.GetY(), "pistol_ammo.png", GetTime()); 
		pBox->SetSize(50, 50);
		pBox->SetPos(pos);
		pBox->SetStatus(1);
		ammoboxes.push_back(pBox);
	}
	else if (0 + rand() % (60+player.GetShotgunAmmo()) == 0) {
        CSprite* pBox = new CSprite(pos.GetX(), pos.GetY(), "shotgun_ammo.png", GetTime());
        pBox->SetSize(50, 50);
        pBox->SetPos(pos);
		pBox->SetStatus(2);
        ammoboxes.push_back(pBox);
    }
	else if (0 + rand() % (50+player.GetRifleAmmo()) == 0) {
        CSprite* pBox = new CSprite(pos.GetX(), pos.GetY(), "rifle_ammo.png", GetTime());
        pBox->SetSize(50, 50);
        pBox->SetPos(pos);
		pBox->SetStatus(3);
        ammoboxes.push_back(pBox);
    }
}

void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_F4 && (mod & (KMOD_LALT | KMOD_RALT)))
		StopGame(); 
	if (sym == SDLK_F2) {
		NewGame();
		Zombies.clear();
	}
	if (IsMenuMode() && sym == SDLK_SPACE) StartGame();

	if (sym == SDLK_r && player.GetWeaponAmmo() > 0 && !player.getReloading()) {
		player.ChangeState(Player::RELOAD);
		player.setReloading();
	}
	
}

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	mousePos = CVector(x, y);
	CVector lookAt = mousePos - player.GetPos();	 //displacement vector from player to mouse
	if (player.GetHealth() > 0) player.SetRotation(RAD2DEG(atan2(lookAt.m_x, lookAt.m_y))); // player rotation is set to face the mouse
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{
	if (player.GetWeapon() == Player::PISTOL&& player.getWeaponClip() > 0 && !player.getReloading()) { // player shoots a bullet when left mouse button is clicked and player has ammo and is not reloading
		player.FireWeapon(); // player ammo is reduced
		CVector gunNozzleOffset = CVector(11 * cos(DEG2RAD(player.GetRotation())) + 21 * sin(DEG2RAD(player.GetRotation())), 21 * cos(DEG2RAD(player.GetRotation())) - 11 * sin(DEG2RAD(player.GetRotation())));
		CVector gunNozzlePos = player.GetPosition() + gunNozzleOffset; // bullet spawn position is calculated based on player position and rotation 
		CSprite* pBullet = new CSprite(gunNozzlePos.GetX(), gunNozzlePos.GetY(), "bullet.bmp", CColor::Green(), GetTime());
		BulletList.push_back(pBullet); 
		pBullet->SetSize(20, 20);
		pBullet->SetDirection(player.GetRotation());
		pBullet->SetRotation(pBullet->GetDirection());
		pBullet->SetSpeed(1000);
    }
	if (player.GetWeapon() == Player::SHOTGUN && player.getWeaponClip() > 0 && !player.getReloading()) {
		player.FireWeapon();
		CVector gunNozzleOffset = CVector(11 * cos(DEG2RAD(player.GetRotation())) + 21 * sin(DEG2RAD(player.GetRotation())), 21 * cos(DEG2RAD(player.GetRotation())) - 11 * sin(DEG2RAD(player.GetRotation())));
		CVector gunNozzlePos = player.GetPosition() + gunNozzleOffset;
		for (int i = 0; i < 5; i++) {
            CSprite* pBullet = new CSprite(gunNozzlePos.GetX(), gunNozzlePos.GetY(), "bullet.bmp", CColor::Green(), GetTime());
            BulletList.push_back(pBullet);
            pBullet->SetSize(20, 20);
            pBullet->SetDirection(player.GetRotation() + (rand() % 20 - 10));
            pBullet->SetRotation(pBullet->GetDirection());
            pBullet->SetSpeed(1000);
        }
	}
	if (player.GetWeapon() == Player::RIFLE && player.getWeaponClip() > 0 && !player.getReloading()) {
        player.FireWeapon();
        CVector gunNozzleOffset = CVector(11 * cos(DEG2RAD(player.GetRotation())) + 21 * sin(DEG2RAD(player.GetRotation())), 21 * cos(DEG2RAD(player.GetRotation())) - 11 * sin(DEG2RAD(player.GetRotation())));
        CVector gunNozzlePos = player.GetPosition() + gunNozzleOffset;
        CSprite* pBullet = new CSprite(gunNozzlePos.GetX(), gunNozzlePos.GetY(), "bullet.bmp", CColor::Green(), GetTime());
        BulletList.push_back(pBullet);
        pBullet->SetSize(20, 20);
        pBullet->SetDirection(player.GetRotation());
        pBullet->SetRotation(pBullet->GetDirection());
        pBullet->SetSpeed(1500);
    }
}
