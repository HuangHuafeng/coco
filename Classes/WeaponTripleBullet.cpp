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
    if (mBullet) {
        auto currentPosition = getParent()->getPosition();
        const auto interval = Vec2(50, 0);
        auto fireRange = mBullet->getFireRange();
        auto offset = mForceType == FRIEND ? Vec2(0, fireRange) : Vec2(0, - fireRange);
        auto bullet1 = mBullet->clone();
        auto bullet2 = mBullet->clone();
        auto bullet3 = mBullet->clone();
        bullet1->modifyPosition(currentPosition - interval);
        bullet1->setDestination(currentPosition - interval + offset);
        bullet2->modifyPosition(currentPosition);
        bullet2->setDestination(currentPosition + offset);
        bullet3->modifyPosition(currentPosition + interval);
        bullet3->setDestination(currentPosition + interval + offset);
        Director::getInstance()->getRunningScene()->addChild(bullet1);
        Director::getInstance()->getRunningScene()->addChild(bullet2);
        Director::getInstance()->getRunningScene()->addChild(bullet3);
    }
}


WeaponTripleBullet * WeaponTripleBullet::create(ForceType forceType, float bulletInterval)
{
    auto weapon = new (std::nothrow) WeaponTripleBullet(forceType, bulletInterval);
    weapon->autorelease();
    
    return weapon;
}

