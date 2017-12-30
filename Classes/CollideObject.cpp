//
//  CollideObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "CollideObject.h"

USING_NS_CC;

CollideObject::CollideObject()
{
    mDamage = 0;
}

bool CollideObject::initialize()
{
    auto physicsBody = PhysicsBody::createBox(getContentSize());
    //physicsBody->setDynamic(false);
    physicsBody->setGravityEnable(false);
    addComponent(physicsBody);
    setForceType(ENEMY); // by default, this is an enemy
    
    return true;
}

void CollideObject::setForceType(ForceType type)
{
    mForceType = type;

    auto physicsBody = getPhysicsBody();
    if (mForceType == FRIEND) {
        physicsBody->setCategoryBitmask(CategoryMaskFriend);
        //physicsBody->setCollisionBitmask(CollisionMaskFriend);
        physicsBody->setContactTestBitmask(ContactTestBitmaskFriend);
    } else {
        physicsBody->setCategoryBitmask(CategoryMaskEnemy);
        //physicsBody->setCollisionBitmask(CollisionMaskEnemy);
        physicsBody->setContactTestBitmask(ContactTestBitmaskEnemy);
    }
    // do not use collide as we are NOT interested! We use "contact test".
    physicsBody->setCollisionBitmask(0);
}

ForceType CollideObject::getForceType() const
{
    return mForceType;
}

void CollideObject::onExit()
{
    GameObject::onExit();
}

void CollideObject::onEnter()
{
    GameObject::onEnter();
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(CollideObject::onContactBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool CollideObject::onContactBegan(PhysicsContact &contact)
{
    auto objA = contact.getShapeA()->getBody()->getNode();
    auto objB = contact.getShapeB()->getBody()->getNode();
    if (objA == this) {
        collideWith(dynamic_cast<CollideObject *>(objB));
    } else if (objB == this) {
        collideWith(dynamic_cast<CollideObject *>(objA));
    } else {
        // not my business
    }
    
    return true;
}

int CollideObject::getDamage()
{
    return mDamage;
}

void CollideObject::setDamage(int damage)
{
    mDamage = damage;
}
