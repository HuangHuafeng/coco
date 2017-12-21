//
//  WeaponTripleBullet.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "WeaponTripleBullet.h"

USING_NS_CC;


WeaponTripleBullet::WeaponTripleBullet(ForceType forceType, float bulletInterval) : Weapon(forceType, bulletInterval)
{
    
}

void WeaponTripleBullet::pullTrigger()
{
    const auto currentPosition = getParent()->getPosition();
    const auto interval = Vec2(50, 0);
    auto bullet1 = Bullet::create("bullet1.png", currentPosition - interval, 250, 100, mForceType);
    Director::getInstance()->getRunningScene()->addChild(bullet1);
    auto bullet2 = Bullet::create("bullet1.png", currentPosition, 250, 100, mForceType);
    Director::getInstance()->getRunningScene()->addChild(bullet2);
    auto bullet3 = Bullet::create("bullet1.png", currentPosition + interval, 250, 100, mForceType);
    Director::getInstance()->getRunningScene()->addChild(bullet3);
}
