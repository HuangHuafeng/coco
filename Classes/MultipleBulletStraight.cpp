//
//  MultipleBulletStraight.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "MultipleBulletStraight.h"
#include "GameScene.h"

USING_NS_CC;

MultipleBulletStraight::MultipleBulletStraight(float interval) : Weapon(interval)
{
}

void MultipleBulletStraight::generateOnce()
{
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
    if (equippedBullet && gameScene) {
        const auto firRange = equippedBullet->getFireRange();
        const auto bulletOffset = equippedBullet->getForceType() == FRIEND ? Vec2(0, firRange) : Vec2(0, - firRange);
        
        auto currentPosition = getParent()->getPosition();
        const auto bulletSize = equippedBullet->getContentSize();
        const auto bulletInterval = bulletSize.width * 0.05;
        const auto localZorder = getParent()->getLocalZOrder() - 1;   // the Bullet is under the WarObject
        auto numberOfBullet = mNumberOfBullet;
        if (numberOfBullet > 5) {   // maximumly 5 bullets
            numberOfBullet = 5;
        }
        currentPosition.x -= (bulletSize.width + bulletInterval) * (numberOfBullet - 1) * 0.5;
        for (auto i = 0;i < numberOfBullet;i++) {
            auto bullet = equippedBullet->clone();
            bullet->setPosition(currentPosition);
            bullet->setDestination(currentPosition + bulletOffset);
            gameScene->AddObjectToScene(bullet, localZorder);
            currentPosition.x += bulletSize.width + bulletInterval;
        }
    }
}


MultipleBulletStraight * MultipleBulletStraight::create(float interval)
{
    auto weapon = new (std::nothrow) MultipleBulletStraight(interval);
    weapon->autorelease();
    
    return weapon;
}


MultipleBulletStraight * MultipleBulletStraight::clone() const
{
    auto mbs = new (std::nothrow) MultipleBulletStraight(mInterval);
    mbs->setObjectId(mId);
    mbs->setObjectName(mName);
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        mbs->setBullet(equippedBullet);
    }
    mbs->autorelease();
    
    return mbs;
}
