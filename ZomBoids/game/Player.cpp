#include "stdafx.h"
#include "Player.h"
#include "MyGame.h"

extern CMyGame game;

void Player::OnStart() {
	LoadAnimation("pistol_idle.png", "pistol_idle", CSprite::Sheet(10, 1).Row(1).From(0).To(9),CColor::White());
	LoadAnimation("pistol_move.png", "pistol_move", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
	LoadAnimation("pistol_reload.png", "pistol_reload", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
	LoadAnimation("pistol_shoot.png", "pistol_shoot", CSprite::Sheet(3, 1).Row(1).From(0).To(2), CColor::White());

    LoadAnimation("rifle_idle.png", "rifle_idle", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
    LoadAnimation("rifle_move.png", "rifle_move", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
    LoadAnimation("rifle_reload.png", "rifle_reload", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
    LoadAnimation("rifle_shoot.png", "rifle_shoot", CSprite::Sheet(3, 1).Row(1).From(0).To(2), CColor::White());

    LoadAnimation("shotgun_idle.png", "shotgun_idle", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
    LoadAnimation("shotgun_move.png", "shotgun_move", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
    LoadAnimation("shotgun_reload.png", "shotgun_reload", CSprite::Sheet(10, 1).Row(1).From(0).To(9), CColor::White());
    LoadAnimation("shotgun_shoot.png", "shotgun_shoot", CSprite::Sheet(3, 1).Row(1).From(0).To(2), CColor::White());

    LoadImage("playerDied.png", "playerDead");

    weapon = PISTOL;
	health = 100;
    pistol_ammo = 50;
    rifle_ammo = 30;
    shotgun_ammo = 10;
    pistol_clip = 10;
    rifle_clip = 30;
    shotgun_clip = 5;
    stamina = 100.0f;
}
//https://uk.indeed.com/q-game-programming-internship-jobs.html?vjk=60b60b86dc61a2ac

void Player::FireWeapon() 
{
    switch (weapon) {
    case PISTOL:
        if (pistol_clip == 0&&!isReloading && pistol_ammo > 0) {
            ChangeState(RELOAD);
            isReloading = true;
        }
        if (pistol_clip > 0&& !isReloading) {
            pistol_clip--;
        }
        break;
    case RIFLE:
        if (rifle_clip == 0&&!isReloading && rifle_ammo > 0) {
            ChangeState(RELOAD);
            isReloading = true;
        }
        if (rifle_clip > 0&&!isReloading) {
            rifle_clip--;
        }
        break;
    case SHOTGUN:
        if (shotgun_clip== 0&&!isReloading && shotgun_ammo > 0) {
            ChangeState(RELOAD);
            isReloading = true;
        }
        if (shotgun_clip > 0&&!isReloading) {
            shotgun_clip--;
        }
        break;
    }
}

void Player::Reload() {
    switch (weapon) {
    case PISTOL:
        if (pistol_ammo > 0 && pistol_ammo >=10) {
            pistol_ammo += pistol_clip;
            pistol_clip = 10;
            pistol_ammo -= 10;
        }
        else if (pistol_ammo > 0 && pistol_ammo < 10) {
            pistol_clip = pistol_ammo;
            pistol_ammo = 0;
        }
        break;
    case RIFLE:
        if (rifle_ammo > 0 && rifle_ammo>=30) {
            rifle_ammo += rifle_clip;
            rifle_clip = 30;
            rifle_ammo -= 30;
        }
        else if (rifle_ammo > 0 && rifle_ammo < 30) {
            rifle_clip = rifle_ammo;
            rifle_ammo = 0;
        }
        break;
    case SHOTGUN:
        if (shotgun_ammo > 0 && shotgun_ammo>=5) {
            shotgun_ammo += shotgun_clip;
            shotgun_clip = 5;
            shotgun_ammo -= 5;
        }
        else if (shotgun_ammo > 0 && shotgun_ammo < 5) {
            shotgun_clip = shotgun_ammo;
            shotgun_ammo = 0;
        }
        break;
    }
    isReloading = false;
} 



void Player::ChangeState(STATE s) {
    if (state == s)
        return;

    state = s;
    if (weapon == RIFLE || weapon == SHOTGUN) SetSize(50, 60);
    else SetSize(50, 50);

    switch (state) {
    case IDLE:
        if (weapon == PISTOL) {
            SetAnimationKeepSize("pistol_idle", 12);
        }
        else if (weapon == RIFLE) {
            SetAnimationKeepSize("rifle_idle", 12);
        }
        else if (weapon == SHOTGUN) {
            SetAnimationKeepSize("shotgun_idle", 12);
        }
        break;
    case MOVE:
        if (weapon == PISTOL) {
            SetAnimationKeepSize("pistol_move", 12);
        }
        else if (weapon == RIFLE) {
            SetAnimationKeepSize("rifle_move", 12);
        }
        else if (weapon == SHOTGUN) {
            SetAnimationKeepSize("shotgun_move", 12);
        }
        break;
    case RELOAD:
        if (weapon == PISTOL) {
            SetAnimationKeepSize("pistol_reload", 12);
        }
        else if (weapon == RIFLE) {
            SetAnimationKeepSize("rifle_reload", 12);
        }
        else if (weapon == SHOTGUN) {
            SetAnimationKeepSize("shotgun_reload", 12);
        }
        break;
    case SHOOT:
        if (weapon == PISTOL) {
            SetAnimationKeepSize("pistol_shoot", 12);
        }
        else if (weapon == RIFLE) {
            SetAnimationKeepSize("rifle_shoot", 12);
        }
        else if (weapon == SHOTGUN) {
            SetAnimationKeepSize("shotgun_shoot", 12);
        }
        break;
    case NONE:
        ChangeState(IDLE);
        break;
    }

}

void Player::OnUpdate(Uint32 time, Uint32 deltaTime)
{

    if (game.IsKeyDown(SDLK_1)) {ChangeWeapon(PISTOL);  ChangeState(NONE); }
    if (game.IsKeyDown(SDLK_2)) {ChangeWeapon(RIFLE);   ChangeState(NONE); }
    if (game.IsKeyDown(SDLK_3)) {ChangeWeapon(SHOTGUN); ChangeState(NONE); }
#
    if (stamina < 0) {
        canSprint = false;
    }
    if (!canSprint && stamina >= 100) {
        canSprint = true;
    }

    if (game.IsKeyDown(SDLK_a))
    {
        SetVelocity(left);
        game.PlayerCollisionBox.SetPosition(GetX() - 15, GetY());
    }
    if (game.IsKeyDown(SDLK_d))
    {
        SetVelocity(right);
        game.PlayerCollisionBox.SetPosition(GetX() + 15, GetY());
    }
    if (game.IsKeyDown(SDLK_w))
    {
        SetVelocity(front);
        game.PlayerCollisionBox.SetPosition(GetX(), GetY() + 15);
        if (game.IsKeyDown(SDLK_a)) {
            SetVelocity((front + left) / 2);
            game.PlayerCollisionBox.SetPosition(GetX() - 15, GetY() + 15);
        }
        else if (game.IsKeyDown(SDLK_d)) {
            SetVelocity((front + right) / 2);
            game.PlayerCollisionBox.SetPosition(GetX() + 15, GetY() + 15);
        }
    }
    if (game.IsKeyDown(SDLK_s))
    {
        SetVelocity(back);
        game.PlayerCollisionBox.SetPosition(GetX(), GetY() - 15);
        if (game.IsKeyDown(SDLK_a)) {
            SetVelocity((back + left) / 2);
            game.PlayerCollisionBox.SetPosition(GetX() - 15, GetY() - 15);
        }
        else if (game.IsKeyDown(SDLK_d)) {
            SetVelocity((back + right) / 2);
            game.PlayerCollisionBox.SetPosition(GetX() + 15, GetY() - 15);
        }

    }
    if (game.IsKeyDown(SDLK_w) || game.IsKeyDown(SDLK_a) || game.IsKeyDown(SDLK_s) || game.IsKeyDown(SDLK_d)) {
        SetSpeed(playerSpeed);
        if (game.IsKeyDown(SDLK_LSHIFT) && canSprint) {
            SetSpeed(playerSpeed +50);
            stamina -= 0.6f;
        }
        else if (stamina < 100 && !game.IsKeyDown(SDLK_LSHIFT)){
            stamina += 0.1f;
        }
        isMoving = true;
    }
    else {
        if (stamina < 100) {
            stamina += 0.2f;
        }
        SetSpeed(0);
        isMoving = false;
    }

    for (CSprite* pbox : game.CollisionBoxes)
    {
        if (game.PlayerCollisionBox.HitTest(pbox)) SetSpeed(0);
    }

    switch (state) {
    case MOVE:
        if (game.IsLButtonDown() && !isReloading && getWeaponClip() > 0) ChangeState(SHOOT);
        if (!isMoving) ChangeState(IDLE);
        break;
    case RELOAD:
        if (GetCurrentAnimationFrame() == 9) {
            Reload();
            ChangeState(IDLE);
        }
        break;
    case SHOOT:
        if (GetCurrentAnimationFrame() == 2) ChangeState(IDLE); 
        
        //else ChangeState(IDLE);
        break;
    case IDLE:
        if (game.IsLButtonDown() && !isReloading && getWeaponClip() > 0) ChangeState(SHOOT);
        if (isMoving) ChangeState(MOVE);
        break;
    }
    CSprite::OnUpdate(time, deltaTime);
}

int Player::GetWeaponAmmo() {
    switch (weapon) {
    case PISTOL:
        return pistol_ammo;
        break;
    case RIFLE:
        return rifle_ammo;
        break;
    case SHOTGUN:
        return shotgun_ammo;
        break;
    }
}

int Player::getWeaponClip() {
    switch (weapon) {
    case PISTOL:
        return pistol_clip;
        break;
    case RIFLE:
        return rifle_clip;
        break;
    case SHOTGUN:
        return shotgun_clip;
        break;
    }
}

void Player::reset() {
    health = 100;
    pistol_ammo = 50;
    rifle_ammo = 30;
    shotgun_ammo = 10;
    pistol_clip = 10;
    rifle_clip = 30;
    shotgun_clip = 5;
    weapon = PISTOL;
    ChangeState(NONE);
}
