#pragma once
#include "stdafx.h"

class Bullet : public CSprite
{
private:
    CVector bulletOrigin;
public:
    Bullet(Sint16 x, Sint16 y, Uint32 time);
    //~bullet(void);
    CVector getBulletOrigin() { return bulletOrigin; }
    void setBulletOrigin(CVector neworigin) { bulletOrigin = neworigin; }
};

Bullet::Bullet(Sint16 x, Sint16 y, Uint32 time) : CSprite(x,y,"bullet.bmp",CColor::Green(), time)
{
    bulletOrigin = CVector(0, 0);
}
