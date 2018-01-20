//
//  WarObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#include "WarObject.h"

USING_NS_CC;

WarObject::WarObject()
{
    mCalmPeriod = 1.0f; // default to 1 second
    mWeapon = nullptr;
    mInitialHealth = 10;   // default to a small value, so it will be killed easily
    mCurrentHealth = mInitialHealth;
    mChangeCallbacks={};
}

WarObject::~WarObject()
{
    if (mWeapon) {
        // no need to remove child as it is automatically done by cocos2d?
        //removeChild(mWeapon, true);
        mWeapon = nullptr;
    }
    
    //assert(mChangeCallbacks.size() == 0);
}

void WarObject::setInitialHealth(int health)
{
    mInitialHealth = health;
    mCurrentHealth = mInitialHealth;
}

int WarObject::getInitialHealth() const
{
    return mInitialHealth;
}

void WarObject::setCurrentHealth(int health)
{
    mCurrentHealth = health;
}

int WarObject::getCurrentHealth() const
{
    return mCurrentHealth;
}

void WarObject::setWeapon(Weapon *weapon)
{
    if (mWeapon) {
        removeChild(mWeapon, true);
        mWeapon = nullptr;
    }

    if (weapon) {
        mWeapon = weapon->clone();
        mWeapon->attachToWarObject(this);
        addChild(mWeapon);
    }
}

void WarObject::openFire()
{
    if (mWeapon) {
        mWeapon->start();
    }
}

void WarObject::ceaseFire()
{
    if (mWeapon) {
        mWeapon->stop();
    }
}

void WarObject::setCalmPeriod(float calmPeriod)
{
    mCalmPeriod = calmPeriod > 0.0f ? calmPeriod : 0.0f;
}

void WarObject::onEnter()
{
    FlyingObject::onEnter();
    
     auto calmPeriodPast = CallFunc::create(CC_CALLBACK_0(WarObject::onCalmPeriodPast, this));
     auto calm = DelayTime::create(mCalmPeriod);
     auto seq = Sequence::create(calm, calmPeriodPast, nullptr);
     runAction(seq);
}

void WarObject::onExit()
{
    callCallbacks(CT_Exit);
    ceaseFire();
    FlyingObject::onExit();
}


void WarObject::collideWith(CollideObject *otherCollideObject)
{
    mCurrentHealth -= otherCollideObject->getDamage();
    callCallbacks(CT_Health);
    if (mCurrentHealth <= 0) {
        onKilled();
    }
}

void WarObject::onKilled()
{
    stop();
    removeFromParent();
}

void WarObject::onCalmPeriodPast()
{
    openFire();
}

void WarObject::registerChangeCallback(GameObject * gameObject, ChangeCallback changeCallback)
{
    if (gameObject) {
        gameObject->retain();
        mChangeCallbacks.insert(std::make_pair(gameObject, changeCallback));
    } else {
        assert(false);
    }
}

void WarObject::unregisterChangeCallback(GameObject * gameObject)
{
    if (gameObject) {
        auto howmany = mChangeCallbacks.erase(gameObject);
        while (howmany > 0) {
            gameObject->autorelease();
            howmany--;
        }
    }
}

void WarObject::callCallbacks(ChangeTypes changeType)
{
    auto itCallback = mChangeCallbacks.cbegin();
    auto endCallback = mChangeCallbacks.cend();
    std::vector<ChangeCallback> copiedCallbacks = {};
    
    // copy the functions because these functions may modify mChangeCallbacks via unregisterChangeCallback()
    while (itCallback != endCallback) {
        copiedCallbacks.push_back(itCallback->second);
        itCallback++;
    }
    
    // call the functions
    auto itCopiedCallback = copiedCallbacks.cbegin();
    auto endCopiedCallback = copiedCallbacks.cend();
    while (itCopiedCallback != endCopiedCallback) {
        (*itCopiedCallback)(this, changeType);
        itCopiedCallback++;
    }
}
