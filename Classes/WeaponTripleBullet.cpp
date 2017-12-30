//
//  WeaponTripleBullet.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "WeaponTripleBullet.h"

USING_NS_CC;


WeaponTripleBullet::WeaponTripleBullet(float interval) : Weapon(interval)
{
    
}

void WeaponTripleBullet::generateOnce()
{
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        auto currentPosition = getParent()->getPosition();
        const auto interval = Vec2(80, 0);
        auto bullet1 = equippedBullet->clone();
        auto bullet2 = equippedBullet->clone();
        auto bullet3 = equippedBullet->clone();
        bullet1->modifyPosition(currentPosition);
        bullet1->setDestination(currentPosition - interval + mBulletOffset);
        bullet2->modifyPosition(currentPosition);
        bullet2->setDestination(currentPosition + mBulletOffset);
        bullet3->modifyPosition(currentPosition);
        bullet3->setDestination(currentPosition + interval + mBulletOffset);
        Director::getInstance()->getRunningScene()->addChild(bullet1);
        Director::getInstance()->getRunningScene()->addChild(bullet2);
        Director::getInstance()->getRunningScene()->addChild(bullet3);
    }
}


WeaponTripleBullet * WeaponTripleBullet::create(float bulletInterval)
{
    auto weapon = new (std::nothrow) WeaponTripleBullet(bulletInterval);
    weapon->autorelease();
    
    return weapon;
}

