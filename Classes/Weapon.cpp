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

void Weapon::FireOn()
{
    auto fireOnce = CallFunc::create(CC_CALLBACK_0(Weapon::fireBullet, this));
    auto delay = DelayTime::create(mBulletInterval);
    auto seq = Sequence::create(fireOnce, delay, nullptr);
    runAction(RepeatForever::create(seq));
}

void Weapon::FireOff()
{
    stopAllActions();
}

void Weapon::fireBullet()
{
    auto currentPosition = getParent()->getPosition();
    auto bulletDestination = currentPosition + Vec2(0, 480);
    log("Weapon::fireBullet()");
    auto bullet = Bullet::create("bullet1.png");
    bullet->setForceType(mForceType);
    bullet->setPosition(currentPosition);
    bullet->setDestination(bulletDestination);
    bullet->setDamage(100);
    bullet->setSpeed(200);
    Director::getInstance()->getRunningScene()->addChild(bullet);
}

void Weapon::setBulletInterval(float bulletInterval)
{
    mBulletInterval = bulletInterval;
}
