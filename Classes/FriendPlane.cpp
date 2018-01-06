//
//  FriendPlane.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "FriendPlane.h"
#include "Weapon.h"
#include "WeaponTripleBullet.h"
#include "GameScene.h"

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
    fp->setObjectId(mId);
    fp->setObjectName(mName);
    fp->initWithTexture(_texture);
    fp->initialize();
    fp->setForceType(mForceType);
    fp->modifyPosition(getPosition());
    fp->setCalmPeriod(mCalmPeriod);
    fp->setDamage(mDamage);
    fp->setInitialHealth(mInitialHealth);
    fp->setCurrentHealth(mCurrentHealth);
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
    
    auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (gameScene) {
        gameScene->onPlayerPlaneEnter(this);
    }
}

void FriendPlane::onExit()
{
    auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (gameScene) {
        gameScene->onPlayerPlaneExit(this);
    }
    
    UserControlledFlyingObject::onExit();
}

