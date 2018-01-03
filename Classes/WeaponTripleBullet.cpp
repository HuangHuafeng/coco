//
//  WeaponTripleBullet.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "WeaponTripleBullet.h"
#include "GameScene.h"

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
        auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
        if (gameScene) {
            auto localZorder = getParent()->getLocalZOrder();
            localZorder--;  // the Bullet is under the WarObject
            gameScene->AddObjectToScene(bullet1, localZorder);
            gameScene->AddObjectToScene(bullet2, localZorder);
            gameScene->AddObjectToScene(bullet3, localZorder);
        }
    }
}


WeaponTripleBullet * WeaponTripleBullet::create(float interval)
{
    auto weapon = new (std::nothrow) WeaponTripleBullet(interval);
    weapon->autorelease();
    
    return weapon;
}


WeaponTripleBullet * WeaponTripleBullet::clone() const
{
    auto weaponTripleBullet = new (std::nothrow) WeaponTripleBullet(mInterval);
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        weaponTripleBullet->setBullet(equippedBullet);
    }
    weaponTripleBullet->autorelease();
    
    return weaponTripleBullet;
}
