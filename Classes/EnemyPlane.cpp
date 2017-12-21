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
    ep->autorelease();
    
    // equip the plane with the basic weapon
    auto weapon = new Weapon(ENEMY, 0.8f);
    ep->setWeapon(weapon);
    
    return ep;
}
