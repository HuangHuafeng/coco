//
//  EnemyGenerator.cpp
//  MyGame
//
//  Created by 黄华锋 on 30/12/2017.
//

#include "EnemyGenerator.h"
#include "ObjectManager.h"
#include "EnemyObject.h"

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
    EnemyObject * enemyObject = dynamic_cast<EnemyObject *>(mObject);
    if (enemyObject) {
        auto newEnemyObject = enemyObject->clone();
        if (newEnemyObject) {
            auto height = enemyObject->getContentSize().height;
            auto windowSize = Director::getInstance()->getWinSize();
            newEnemyObject->modifyPosition(Vec2(windowSize.width * rand_0_1(), windowSize.height + height));
            newEnemyObject->setDestination(Vec2(windowSize.width * rand_0_1(), - height));
            newEnemyObject->setObjectId(ObjectManager::getInstance()->giveMeId());
            ObjectManager::getInstance()->AddObjectToScene(newEnemyObject);
        }
    }
}

EnemyGenerator * EnemyGenerator::create(float interval)
{
    auto eg = new (std::nothrow) EnemyGenerator(interval);
    eg->autorelease();
    
    return eg;
}
