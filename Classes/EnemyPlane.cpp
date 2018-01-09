//
//  EnemyPlane.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "EnemyPlane.h"

USING_NS_CC;

EnemyPlane::EnemyPlane()
{
    ;
}

EnemyPlane* EnemyPlane::create(const std::string &filename)
{
    EnemyPlane* ep = new (std::nothrow) EnemyPlane();
    ep->initWithFile(filename);
    ep->initialize();
    ep->setForceType(ENEMY);
    ep->autorelease();
    
    return ep;
}

EnemyPlane * EnemyPlane::clone() const
{
    EnemyPlane* ep = new (std::nothrow) EnemyPlane();
    ep->setObjectId(mId);
    ep->setObjectName(mName);
    ep->initWithTexture(_texture);
    ep->initialize();
    ep->setForceType(mForceType);
    ep->setPosition(getPosition());
    ep->setCalmPeriod(mCalmPeriod);
    ep->setDamage(mDamage);
    ep->setInitialHealth(mInitialHealth);
    ep->setCurrentHealth(mCurrentHealth);
    ep->setSpeed(mSpeed);
    ep->setWeapon(mWeapon);
    ep->autorelease();
    
    return ep;
}
