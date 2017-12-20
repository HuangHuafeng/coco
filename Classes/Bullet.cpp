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

Bullet* Bullet::create(const std::string &filename)
{
    Bullet* b = new (std::nothrow) Bullet();
    b->initWithFile(filename);
    b->initialize();
    b->autorelease();
    
    return b;
}

int Bullet::getDamage()
{
    return mDamage;
}

void Bullet::setDamage(int damage)
{
    mDamage = damage;
}

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

bool Bullet::collideWith(CollideObject* otherCollideObject)
{
    removeFromParent();
}
