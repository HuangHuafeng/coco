//
//  Weapon.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "Weapon.h"
#include "WarObject.h"
#include "ObjectManager.h"
#include "GameScene.h"

USING_NS_CC;

Weapon::Weapon(float interval)
: ObjectGenerator(interval)
{
    mBulletOffset = Vec2(0, 0);
    mWarObject = nullptr;
}

Weapon::~Weapon()
{
    if (mWarObject) {
        mWarObject->autorelease();
        mWarObject = nullptr;
    }
}

// subclass can override this function to get different effect
// for example, fire 4 bullets a time
void Weapon::generateOnce()
{
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        auto currentPosition = getParent()->getPosition();
        auto bullet = equippedBullet->clone();
        auto destination = currentPosition + mBulletOffset;
        bullet->modifyPosition(currentPosition);
        bullet->setDestination(destination);
        auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
        if (gameScene) {
            auto localZorder = getParent()->getLocalZOrder();
            localZorder--;  // the Bullet is under the WarObject
            gameScene->AddObjectToScene(bullet, localZorder);
        }
    }
}

void Weapon::attachToWarObject(WarObject *warObject)
{
    if (mWarObject) {
        mWarObject->autorelease();
        mWarObject = nullptr;
    }
    
    if (warObject) {
        mWarObject = warObject;
        mWarObject->retain();
        updateBullet();
    }
}

void Weapon::updateBullet()
{
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet && mWarObject) {
        auto forceType = mWarObject->getForceType();
        equippedBullet->setForceType(forceType);
        
        auto firRange = equippedBullet->getFireRange();
        mBulletOffset = forceType == FRIEND ? Vec2(0, firRange) : Vec2(0, - firRange);
    }
}

void Weapon::setBullet(Bullet *bullet)
{
    if (bullet) {
        setObject(bullet);
        updateBullet();
    }
}

Weapon * Weapon::create(float interval)
{
    auto weapon = new (std::nothrow) Weapon(interval);
    weapon->autorelease();

    return weapon;
}

Weapon * Weapon::clone() const
{
    auto weapon = new (std::nothrow) Weapon(mInterval);
    Bullet * equippedBullet = dynamic_cast<Bullet *>(mObject);
    if (equippedBullet) {
        weapon->setBullet(equippedBullet);
    }
    weapon->autorelease();
    
    return weapon;
}
