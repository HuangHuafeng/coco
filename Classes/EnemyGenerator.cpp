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
    //log("seed: %f", seed);
    std::srand(seed);
}

void EnemyGenerator::generateOnce()
{
    //EnemyObject * enemyObject = dynamic_cast<EnemyObject *>(mObject);
    // DEBUG
    FlyingObject * enemyObject = nullptr;
    EnemyObject * tempObject = dynamic_cast<EnemyObject *>(mObject);
    if (!tempObject) {
        enemyObject = dynamic_cast<Bullet *>(mObject);
    } else {
        enemyObject = tempObject;
    }
    // DEBUG END
    if (enemyObject) {
        auto newEnemyObject = enemyObject->clone();
        if (newEnemyObject) {
            auto height = enemyObject->getContentSize().height;
            auto windowSize = Director::getInstance()->getWinSize();
            newEnemyObject->setPosition(Vec2(windowSize.width * rand_0_1(), windowSize.height + height));
            newEnemyObject->setDestination(Vec2(windowSize.width * rand_0_1(), - height));
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
    auto eg = new (std::nothrow) EnemyGenerator(interval);
    eg->autorelease();
    
    return eg;
}

EnemyGenerator * EnemyGenerator::clone() const
{
    auto generator = new (std::nothrow) EnemyGenerator(mInterval);
    generator->setObjectId(mId);
    generator->setObjectName(mName);
    generator->setObject(mObject);
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
