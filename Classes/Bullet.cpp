//
//  Bullet.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "Bullet.h"

USING_NS_CC;

Bullet::Bullet()
{
    
}


float Bullet::getFireRange()
{
    auto windowSize = Director::getInstance()->getWinSize();
    return windowSize.height;
}


Bullet* Bullet::create(const std::string &filename, const cocos2d::Vec2& position, float speed, int damage, ForceType type)
{
    Bullet* b = new (std::nothrow) Bullet();
    b->initWithFile(filename);
    b->initialize();
    b->setPosition(position);
    b->setDamage(damage);
    b->setForceType(type);
    b->setSpeed(speed);
    b->autorelease();

    return b;
}


void Bullet::setForceType(ForceType type)
{
    FlyingObject::setForceType(type);
    
    auto physicsBody = getPhysicsBody();
    auto categoryBitmask = type == FRIEND ? CategoryMaskFriendBullet : CategoryMaskEnemyBullet;
    physicsBody->setCategoryBitmask(categoryBitmask);
    auto contactTestBitmask = type == FRIEND ? ContactTestBitmaskFriendBullet : ContactTestBitmaskEnemyBullet;
    physicsBody->setContactTestBitmask(contactTestBitmask);
    
    // set the destination accordingly
    auto fireRange = getFireRange();
    auto position = getPosition();
    auto destination = type == FRIEND ? position + Vec2(0, fireRange) : position - Vec2(0, fireRange);
    setDestination(destination);
}

int Bullet::getDamage()
{
    return mDamage;
}

void Bullet::setDamage(int damage)
{
    mDamage = damage;
}

// override updateRoute() to remove myself after reaching the destination
void Bullet::updateRoute()
{
    stop();
    
    if (mSpeed > 0) {
        auto currentPosition = getPosition();
        if (currentPosition != mDestination) {
            // calculate the time needed
            auto distance = currentPosition.getDistance(mDestination);
            auto moveTo = MoveTo::create(distance/mSpeed, mDestination);
            moveTo->setTag(ACTION_TAG);
            auto removeSelf = RemoveSelf::create();
            removeSelf->setTag(ACTION_TAG);
            auto seq = Sequence::create(moveTo, removeSelf, nullptr);
            runAction(seq);
        }
    }
}

void Bullet::collideWith(CollideObject* otherCollideObject)
{
    removeFromParent();
}