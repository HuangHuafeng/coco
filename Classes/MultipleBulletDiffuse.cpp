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
    mBulletStartInterval = 0.01;
    mBulletDestinationInterval = mBulletStartInterval;
    mMaxNumberOfBullet = 8;
    mNumberOfBullet = 4;
}

void MultipleBulletDiffuse::generateOnce()
{
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        const auto firRange = equippedBullet->getFireRange();
        const auto bulletOffset = equippedBullet->getForceType() == FRIEND ? Vec2(0, firRange) : Vec2(0, - firRange);
        
        auto currentPosition = getParent()->getPosition();
        const auto bulletSize = equippedBullet->getContentSize();
        const auto bulletStartInterval = bulletSize.width * mBulletStartInterval;
        const auto bulletDestinationInterval = bulletSize.width * mBulletDestinationInterval;
        
        auto numberOfBullet = mNumberOfBullet;
        if (numberOfBullet > mMaxNumberOfBullet) {
            numberOfBullet = mMaxNumberOfBullet;
        }
        
        auto destinationPosition = currentPosition + bulletOffset;
        destinationPosition.x -= (bulletSize.width + bulletDestinationInterval) * (numberOfBullet - 1) * 0.5;
        currentPosition.x -= (bulletSize.width + bulletStartInterval) * (numberOfBullet - 1) * 0.5;
        for (auto i = 0;i < numberOfBullet;i++) {
            fireABullet(currentPosition, destinationPosition);
            currentPosition.x += bulletSize.width + bulletStartInterval;
            destinationPosition.x += bulletSize.width + bulletDestinationInterval;
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
