//
//  Weapon.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "Weapon.h"

USING_NS_CC;

Weapon::Weapon(ForceType forceType, float bulletInterval)
{
    mForceType = forceType;
    mBulletInterval = bulletInterval;
    mBullet = nullptr;
}

Weapon::~Weapon()
{
    if (mBullet) {
        mBullet->autorelease();
        mBullet = nullptr;
    }
}

// openFire fire a bullet very mBulletInterval seconds
// subclass should ONLY override this function if it does
// not like this "fire a bullet every mBulletInterval seconds"
// otherwise, override pullTrigger() should work
void Weapon::openFire()
{
    auto fireOnce = CallFunc::create(CC_CALLBACK_0(Weapon::pullTrigger, this));
    auto delay = DelayTime::create(mBulletInterval);
    auto seq = Sequence::create(fireOnce, delay, nullptr);
    runAction(RepeatForever::create(seq));
}

void Weapon::ceaseFire()
{
    stopAllActions();
}

// subclass can override this function to get different effect
// for example, fire 4 bullets a time
void Weapon::pullTrigger()
{
    if (mBullet) {
        auto currentPosition = getParent()->getPosition();
        auto bullet = mBullet->clone();
        auto offset = Vec2(0, bullet->getFireRange());
        auto destination = mForceType == FRIEND ? currentPosition + offset : currentPosition - offset;
        bullet->modifyPosition(currentPosition);
        bullet->setDestination(destination);
        
        Director::getInstance()->getRunningScene()->addChild(bullet);
    }
}

void Weapon::setBulletInterval(float bulletInterval)
{
    mBulletInterval = bulletInterval;
}

void Weapon::setBullet(Bullet *bullet)
{
    if (mBullet) {
        mBullet->autorelease();
        mBullet = nullptr;
    }
    
    if (bullet) {
        mBullet = bullet->clone();
        mBullet->setForceType(mForceType);
        // mBullet will never be added to the scene, so we need to retain it
        mBullet->retain();
    }
}

Weapon * Weapon::create(ForceType forceType, float bulletInterval)
{
    auto weapon = new (std::nothrow) Weapon(forceType, bulletInterval);
    weapon->autorelease();

    return weapon;
}
