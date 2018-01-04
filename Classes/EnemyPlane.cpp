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
    // cloned object has id 0 and name "", it should not be touched unless necessary
    //ep->setObjectName("clonedEnemyPlane");
    ep->initWithTexture(_texture);
    ep->initialize();
    ep->setForceType(mForceType);
    ep->modifyPosition(getPosition());
    ep->setCalmPeriod(mCalmPeriod);
    ep->setDamage(mDamage);
    ep->setHealth(mHealth);
    ep->setSpeed(mSpeed);
    ep->setWeapon(mWeapon);
    ep->autorelease();
    
    return ep;
}
