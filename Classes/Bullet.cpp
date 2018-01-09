//
//  Bullet.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "Bullet.h"

USING_NS_CC;

Bullet::Bullet() : FlyingObject()
{
    
}


float Bullet::getFireRange()
{
    auto windowSize = Director::getInstance()->getWinSize();
    return windowSize.height * 1.5f;
}


Bullet* Bullet::create(const std::string &filename, float speed, int damage)
{
    Bullet* b = new (std::nothrow) Bullet();
    b->initWithFile(filename);
    b->initialize();
    b->setDamage(damage);
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
    
    /*
    // set the destination accordingly
    auto fireRange = getFireRange();
    auto position = getPosition();
    auto destination = type == FRIEND ? position + Vec2(0, fireRange) : position - Vec2(0, fireRange);
    setDestination(destination);
    */
}

Bullet* Bullet::clone() const
{
    Bullet* b = new (std::nothrow) Bullet();
    b->setObjectId(mId);
    b->setObjectName(mName);
    b->initWithTexture(_texture);
    b->initialize();
    b->setDamage(mDamage);
    b->setForceType(mForceType);
    b->setPosition(getPosition());
    b->setSpeed(mSpeed);
    b->autorelease();
    
    return b;
}

// override updateRoute() to remove myself after reaching the destination
void Bullet::updateRoute()
{
    stop();
    
    if (mSpeed > 0) {
        auto currentPosition = getPosition();
        if (currentPosition != mDestination) {
            // set rotation
            auto offset = mDestination - currentPosition;
            auto rot = offset.getAngle(Vec2(0, 1));
            setRotation(rot * 180 / 3.1415926);

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
