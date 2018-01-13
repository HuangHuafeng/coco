//
//  MultipleBulletDiffuse.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "MultipleBulletDiffuse.h"
#include "GameScene.h"

USING_NS_CC;

MultipleBulletDiffuse::MultipleBulletDiffuse(float interval) : Weapon(interval)
{
}

void MultipleBulletDiffuse::generateOnce()
{
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (equippedBullet && gameScene) {
        const auto firRange = equippedBullet->getFireRange();
        const auto bulletOffset = equippedBullet->getForceType() == FRIEND ? Vec2(0, firRange) : Vec2(0, - firRange);
        
        auto currentPosition = getParent()->getPosition();
        const auto bulletSize = equippedBullet->getContentSize();
        const auto bulletInterval = bulletSize.width * 4;
        const auto localZorder = getParent()->getLocalZOrder() - 1;   // the Bullet is under the WarObject
        auto numberOfBullet = mNumberOfBullet;
        if (numberOfBullet > 8) {   // maximumly 8 bullets
            numberOfBullet = 8;
        }
        auto destinationPosition = currentPosition + bulletOffset;
        destinationPosition.x -= (bulletSize.width + bulletInterval) * (numberOfBullet - 1) * 0.5;
        //currentPosition.x -= (bulletSize.width + bulletInterval) * (numberOfBullet - 1) * 0.5;
        for (auto i = 0;i < numberOfBullet;i++) {
            auto bullet = equippedBullet->clone();
            bullet->setPosition(currentPosition);
            bullet->setDestination(destinationPosition);
            gameScene->AddObjectToScene(bullet, localZorder);
            destinationPosition.x += bulletSize.width + bulletInterval;
        }
    }
}


MultipleBulletDiffuse * MultipleBulletDiffuse::create(float interval)
{
    auto weapon = new (std::nothrow) MultipleBulletDiffuse(interval);
    weapon->autorelease();
    
    return weapon;
}


MultipleBulletDiffuse * MultipleBulletDiffuse::clone() const
{
    auto mbd = new (std::nothrow) MultipleBulletDiffuse(mInterval);
    mbd->setObjectId(mId);
    mbd->setObjectName(mName);
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        mbd->setBullet(equippedBullet);
    }
    mbd->autorelease();
    
    return mbd;
}
