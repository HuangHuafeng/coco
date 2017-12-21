//
//  FlyingObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 19/12/2017.
//

#include "FlyingObject.h"

USING_NS_CC;

FlyingObject::FlyingObject()
{
    mSpeed = 0.0f;
    mDestination = Vec2(0, 0);
}

void FlyingObject::setDestination(const cocos2d::Vec2 &destination)
{
    mDestination = destination;
    updateRoute();
}

void FlyingObject::setSpeed(float speed)
{
    mSpeed = speed;
    updateRoute();
}

void FlyingObject::updateRoute()
{
    stop();
    
    if (mSpeed > 0) {
        auto currentPosition = getPosition();
        if (currentPosition != mDestination) {
            // calculate the time needed
            auto distance = currentPosition.getDistance(mDestination);
            auto moveTo = MoveTo::create(distance/mSpeed, mDestination);
            moveTo->setTag(ACTION_TAG);
            runAction(moveTo);
        }
    }
}

void FlyingObject::stop()
{
    stopAllActionsByTag(ACTION_TAG);
}
