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
}

WarObject::~WarObject()
{
    if (mWeapon) {
        removeChild(mWeapon, true);
        mWeapon = nullptr;
    }
}

void WarObject::setWeapon(Weapon *weapon)
{
    if (mWeapon) {
        removeChild(mWeapon, true);
        mWeapon = nullptr;
    }

    if (weapon) {
        mWeapon = weapon;
        mWeapon->attachToWarObject(this);
        addChild(mWeapon);
    }
}

void WarObject::openFire()
{
    if (mWeapon) {
        mWeapon->openFire();
    }
}

void WarObject::ceaseFire()
{
    if (mWeapon) {
        mWeapon->ceaseFire();
    }
}
