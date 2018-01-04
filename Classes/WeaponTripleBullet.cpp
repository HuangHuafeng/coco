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
        auto bulletSize = equippedBullet->getContentSize();
        const auto startInterval = Vec2(bulletSize.width / 2, 0);
        const auto destinationInterval = Vec2(bulletSize.width * 3, 0);
        auto bullet1 = equippedBullet->clone();
        auto bullet2 = equippedBullet->clone();
        auto bullet3 = equippedBullet->clone();
        bullet1->modifyPosition(currentPosition - startInterval);
        bullet1->setDestination(currentPosition - destinationInterval + mBulletOffset);
        bullet2->modifyPosition(currentPosition);
        bullet2->setDestination(currentPosition + mBulletOffset);
        bullet3->modifyPosition(currentPosition + startInterval);
        bullet3->setDestination(currentPosition + destinationInterval + mBulletOffset);
        auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
        
        if (gameScene) {
            // DEBUG
            /*
            bullet1->setObjectId(gameScene->giveMeId());
            bullet1->setObjectName("bullet1");
            bullet2->setObjectId(gameScene->giveMeId());
            bullet2->setObjectName("bullet2");
            bullet3->setObjectId(gameScene->giveMeId());
            bullet3->setObjectName("bullet3");
             */
            // DEBUG END
            
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
    // cloned object has id 0 and name "", it should not be touched unless necessary
    //weaponTripleBullet->setObjectName("clonedWeaponTripleBullet");
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        weaponTripleBullet->setBullet(equippedBullet);
    }
    weaponTripleBullet->autorelease();
    
    return weaponTripleBullet;
}
