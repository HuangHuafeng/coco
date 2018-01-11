//
//  EnemyGenerator.cpp
//  MyGame
//
//  Created by 黄华锋 on 30/12/2017.
//

#include "EnemyGenerator.h"
#include "ObjectManager.h"
#include "EnemyObject.h"
#include "GameScene.h"

USING_NS_CC;

EnemyGenerator::EnemyGenerator(float interval)
: ObjectGenerator(interval)
{
    auto seed = utils::gettime() * 1000000;
    std::srand(seed);
}

void EnemyGenerator::generateOnce()
{
    FlyingObject * enemyObject = dynamic_cast<FlyingObject *>(mObject);
    if (enemyObject) {
        auto newEnemyObject = enemyObject->clone();
        if (newEnemyObject) {
            auto position = generatePosition();
            auto destination = generateDestination(position);
            newEnemyObject->setPosition(position);
            newEnemyObject->setDestination(destination);
            auto gameScene = dynamic_cast<GameScene *>(Director::getInstance()->getRunningScene());
            if (gameScene) {
                //newEnemyObject->setObjectId(gameScene->giveMeId());
                gameScene->AddObjectToScene(newEnemyObject);
            }
        }
    }
}

EnemyGenerator * EnemyGenerator::create(float interval)
{
    auto windowSize = Director::getInstance()->getWinSize();
    auto fromRect = Rect(0, windowSize.height, windowSize.width, windowSize.height * 0.1);
    auto toRect = Rect(0, - windowSize.height * 0.1, windowSize.width, 0);
    auto generator = new (std::nothrow) EnemyGenerator(interval);
    generator->setDestinationPolicy(DP_RandomTo);
    generator->setFromRect(fromRect);
    generator->setToRect(toRect);
    generator->autorelease();
    
    return generator;
}

EnemyGenerator * EnemyGenerator::clone() const
{
    auto generator = new (std::nothrow) EnemyGenerator(mInterval);
    generator->setObjectId(mId);
    generator->setObjectName(mName);
    generator->setObject(mObject);
    generator->setDestinationPolicy(mDestinationPolicy);
    generator->setFromRect(mFromRect);
    generator->setToRect(mToRect);
    generator->autorelease();
    
    return generator;
}

void EnemyGenerator::onEnter()
{
    ObjectGenerator::onEnter();
    start();
}

void EnemyGenerator::onExit()
{
    stop();
    ObjectGenerator::onExit();
}

void EnemyGenerator::setDestinationPolicy(DestinationPolicy destinationPolicy)
{
    mDestinationPolicy = destinationPolicy;
}

void EnemyGenerator::setFromRect(cocos2d::Rect fromRect)
{
    mFromRect = fromRect;
}

void EnemyGenerator::setToRect(cocos2d::Rect toRect)
{
    mToRect = toRect;
}

Vec2 EnemyGenerator::generatePosition()
{
    return Vec2(mFromRect.origin.x + mFromRect.size.width * rand_0_1(), mFromRect.origin.y + mFromRect.size.height * rand_0_1());
}

Vec2 EnemyGenerator::generateDestination(const Vec2 & from)
{
    Vec2 destination = Vec2(mToRect.origin.x + mToRect.size.width * rand_0_1(), mToRect.origin.y + mToRect.size.height * rand_0_1());
    switch (mDestinationPolicy) {
        case DP_VerticalTo:
            destination.x = from.x;
            break;
            
        case DP_HorizontalTo:
            destination.y = from.y;
            
        case DP_RandomTo:
            // nothing to do
            break;
            
        default:
            assert(false);
            break;
    }
    
    return destination;
}
