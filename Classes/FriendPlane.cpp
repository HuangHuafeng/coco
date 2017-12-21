//
//  FriendPlane.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "FriendPlane.h"
#include "Weapon.h"

USING_NS_CC;

FriendPlane::FriendPlane()
{
    
}


FriendPlane* FriendPlane::create(const std::string &filename)
{
    FriendPlane* fp = new (std::nothrow) FriendPlane();
    fp->initWithFile(filename);
    fp->initialize();
    fp->autorelease();
    
    // equip the plane with the basic weapon
    auto weapon = new Weapon(FRIEND, 0.4f);
    fp->setWeapon(weapon);
    
    return fp;
}
