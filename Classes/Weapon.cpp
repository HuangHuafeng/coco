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
    auto currentPosition = getParent()->getPosition();
    auto bullet = Bullet::create("bullet1.png", currentPosition, 250, 100, mForceType);
    Director::getInstance()->getRunningScene()->addChild(bullet);
}

void Weapon::setBulletInterval(float bulletInterval)
{
    mBulletInterval = bulletInterval;
}
