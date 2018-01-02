//
//  FriendPlane.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "FriendPlane.h"
#include "Weapon.h"
#include "WeaponTripleBullet.h"

USING_NS_CC;

FriendPlane::FriendPlane()
{
    
}


FriendPlane* FriendPlane::create(const std::string &filename)
{
    FriendPlane* fp = new (std::nothrow) FriendPlane();
    fp->initWithFile(filename);
    fp->initialize();
    fp->setForceType(FRIEND);
    fp->autorelease();

    return fp;
}

FriendPlane * FriendPlane::clone() const
{
    FriendPlane* fp = new (std::nothrow) FriendPlane();
    fp->initWithTexture(_texture);
    fp->initialize();
    fp->setForceType(mForceType);
    fp->modifyPosition(getPosition());
    fp->setDamage(mDamage);
    fp->setHealth(mHealth);
    fp->setSpeed(mSpeed);
    fp->setWeapon(mWeapon);
    fp->autorelease();
    
    return fp;
}

void FriendPlane::collideWith(CollideObject *otherCollideObject)
{
    UserControlledFlyingObject::collideWith(otherCollideObject);
}

void FriendPlane::onEnter()
{
    UserControlledFlyingObject::onEnter();
    openFire();
}

void FriendPlane::onExit()
{
    ceaseFire();
    UserControlledFlyingObject::onExit();
}
