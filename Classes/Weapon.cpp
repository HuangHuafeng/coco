//
//  Weapon.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "Weapon.h"
#include "WarObject.h"

USING_NS_CC;

Weapon::Weapon(float triggerInterval)
{
    mTriggerInterval = triggerInterval;
    mBulletOffset = Vec2(0, 0);
    mBullet = nullptr;
    mWarObject = nullptr;
}

Weapon::~Weapon()
{
    if (mBullet) {
        mBullet->autorelease();
        mBullet = nullptr;
    }

    if (mWarObject) {
        mWarObject->autorelease();
        mWarObject = nullptr;
    }
}

// openFire fire a bullet very mBulletInterval seconds
// subclass should ONLY override this function if it does
// not like this "fire a bullet every mBulletInterval seconds"
// otherwise, override pullTrigger() should work
void Weapon::openFire()
{
    auto fireOnce = CallFunc::create(CC_CALLBACK_0(Weapon::pullTrigger, this));
    auto delay = DelayTime::create(mTriggerInterval);
    auto seq = Sequence::create(fireOnce, delay, nullptr);
    runAction(RepeatForever::create(seq));
}

void Weapon::ceaseFire()
{
    stopAllActions();
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
    if (mBullet && mWarObject) {
        auto forceType = mWarObject->getForceType();
        mBullet->setForceType(forceType);
        
        auto firRange = mBullet->getFireRange();
        mBulletOffset = forceType == FRIEND ? Vec2(0, firRange) : Vec2(0, - firRange);
    }
}

// subclass can override this function to get different effect
// for example, fire 4 bullets a time
void Weapon::pullTrigger()
{
    if (mBullet) {
        auto currentPosition = getParent()->getPosition();
        auto bullet = mBullet->clone();
        auto destination = currentPosition + mBulletOffset;
        bullet->modifyPosition(currentPosition);
        bullet->setDestination(destination);
        
        Director::getInstance()->getRunningScene()->addChild(bullet);
    }
}

void Weapon::setBulletInterval(float triggerInterval)
{
    mTriggerInterval = triggerInterval;
}

void Weapon::setBullet(Bullet *bullet)
{
    if (mBullet) {
        mBullet->autorelease();
        mBullet = nullptr;
    }
    
    if (bullet) {
        mBullet = bullet->clone();
        // mBullet will never be added to the scene, so we need to retain it
        mBullet->retain();
        updateBullet();
    }
}

Weapon * Weapon::create(float triggerInterval)
{
    auto weapon = new (std::nothrow) Weapon(triggerInterval);
    weapon->autorelease();

    return weapon;
}
