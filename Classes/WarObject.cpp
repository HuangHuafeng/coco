//
//  WarObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "WarObject.h"

WarObject::WarObject()
{
    mWeapon = nullptr;
    mHealth = 10;   // default to a small value, so it will be killed easily
}

WarObject::~WarObject()
{
    if (mWeapon) {
        removeChild(mWeapon, true);
        mWeapon = nullptr;
    }
}

void WarObject::setHealth(int health)
{
    mHealth = health;
}

int WarObject::getHealth() const
{
    return mHealth;
}

void WarObject::setWeapon(Weapon *weapon)
{
    if (mWeapon) {
        removeChild(mWeapon, true);
        mWeapon = nullptr;
    }

    if (weapon) {
        mWeapon = weapon->clone();
        mWeapon->attachToWarObject(this);
        addChild(mWeapon);
    }
}

void WarObject::openFire()
{
    if (mWeapon) {
        mWeapon->start();
    }
}

void WarObject::ceaseFire()
{
    if (mWeapon) {
        mWeapon->stop();
    }
}

void WarObject::onEnter()
{
    FlyingObject::onEnter();
    openFire();
}

void WarObject::onExit()
{
    FlyingObject::onExit();
    ceaseFire();
}

void WarObject::collideWith(CollideObject *otherCollideObject)
{
    mHealth -= otherCollideObject->getDamage();

    if (mHealth <= 0) {
        OnKilled();
    }
}

void WarObject::OnKilled()
{
    stop();
    removeFromParent();
}
