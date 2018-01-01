//
//  ObjectGenerator.cpp
//  MyGame
//
//  Created by 黄华锋 on 30/12/2017.
//

#include "ObjectGenerator.h"

USING_NS_CC;

ObjectGenerator::ObjectGenerator(float interval)
{
    mInterval = interval;
    mObject = nullptr;
}

ObjectGenerator::~ObjectGenerator()
{
    if (mObject) {
        mObject->autorelease();
        mObject = nullptr;
    }
}

void ObjectGenerator::setObject(GameObject *object)
{
    if (mObject) {
        mObject->autorelease();
        mObject = nullptr;
    }
    
    if (object) {
        mObject = object->clone();
        mObject->retain();
    }
}

// start() calls generateOnce() every "mInterval" seconds, repeatedly
// subclass can override this behaviour if needed
void ObjectGenerator::start()
{
    auto generateOnce = CallFunc::create(CC_CALLBACK_0(ObjectGenerator::generateOnce, this));
    auto delay = DelayTime::create(mInterval);
    auto seq = Sequence::create(generateOnce, delay, nullptr);
    runAction(RepeatForever::create(seq));
}

void ObjectGenerator::stop()
{
    stopAllActions();
}

void ObjectGenerator::onEnter()
{
    GameObject::onEnter();
    start();
}

void ObjectGenerator::onExit()
{
    stop();
    GameObject::onExit();
}
