//
//  HealthIndicator.cpp
//  MyGame
//
//  Created by 黄华锋 on 05/01/2018.
//

#include "HealthIndicator.h"

USING_NS_CC;

HealthIndicator::HealthIndicator(Color4F initialHealthColor, Color4F currentHealthColor, PositionPolicy positionPolicy)
{
    mPositionPolicy = positionPolicy;
    mInitialHealthColor = initialHealthColor;
    mCurrentHealthColor = currentHealthColor;
    mLinkedWarObject = nullptr;
    mHealthBar = nullptr;
    mWidth = 100.0;
    mHeight = mWidth / WIDTH_HEIGHT_RATIO;
    
    mHealthBar = DrawNode::create();
    addChild(mHealthBar);
}

HealthIndicator::~HealthIndicator()
{
    unlink();
    if (mHealthBar != nullptr) {
        // nothing to do as mHealthBar is an auto release object
        mHealthBar = nullptr;
    }
}

// this function should be called only once when the healthIndicator is created
void HealthIndicator::linkToObject(WarObject *warObject)
{
    assert(mLinkedWarObject == nullptr);
    mLinkedWarObject = warObject;
    if (mLinkedWarObject) {
        // should NOT retain mLinkedWarObject!!!
        //mLinkedWarObject->retain();
        
        mWidth = mLinkedWarObject->getContentSize().width;
        mHeight = mWidth / WIDTH_HEIGHT_RATIO;
        mHealthBar->setPosition(Vec2(0, - mHeight));

        mLinkedWarObject->addChild(this);
        mLinkedWarObject->registerChangeCallback(this, CC_CALLBACK_2(HealthIndicator::onWarObjectChanged, this));
        
        updateHealthBarPicture();
    }
}

void HealthIndicator::unlink()
{
    if (mLinkedWarObject) {
        mLinkedWarObject->unregisterChangeCallback(this);

        // no need to remove myself from mLinkedWarObject as mLinkedWarObject its my parent
        // unlink() was ONLY triggerred when mLinkedWarObject is relased (thus releasing myself
        // as I am a child of mLinkedWarObject)
        //mLinkedWarObject->removeChild(this, true);
        
        // no need to release mLinkedWarObject as we haven't retain() it.
        //mLinkedWarObject->autorelease();
        
        mLinkedWarObject = nullptr;
    }
}

void HealthIndicator::onWarObjectChanged(WarObject *warObject, WarObject::ChangeTypes changeType)
{
    assert(mLinkedWarObject && mLinkedWarObject == warObject);
    // CT_Exit maybe moved to a parent class (not created yet) in future
    if (changeType == WarObject::CT_Exit) {
        unlink();
        //removeFromParent();
    } else if (changeType == WarObject::CT_Health) {
        updateHealthBarPicture();
    }else {
        // not interested in any other changes
    }
}

// the bar is default to horzontal
void HealthIndicator::updateHealthBarPicture()
{
    assert(mHealthBar != nullptr);
    assert(mLinkedWarObject != nullptr);
    auto ratio = (1.0f * mLinkedWarObject->getCurrentHealth()) / mLinkedWarObject->getInitialHealth();
    mHealthBar->clear();
    mHealthBar->drawSolidRect(Vec2(0, 0), Vec2(mWidth, mHeight), mInitialHealthColor);
    mHealthBar->drawSolidRect(Vec2(0, 0), Vec2(mWidth * ratio, mHeight), mCurrentHealthColor);
}

HealthIndicator * HealthIndicator::createHealthIndicatorForObject(WarObject *warObject, Color4F initialHealthColor, Color4F currentHealthColor, PositionPolicy positionPolicy)
{
    HealthIndicator* hi = new (std::nothrow) HealthIndicator(initialHealthColor, currentHealthColor, positionPolicy);
    if (hi) {
        hi->linkToObject(warObject);
        hi->autorelease();
    } else {
        delete hi;
        hi = nullptr;
    }
    
    return hi;
}

void HealthIndicator::dontMoveWithLinkedObject(cocos2d::Rect rectArea)
{
    assert(mLinkedWarObject != nullptr);
    removeFromParent();
    setPosition(rectArea.origin);
    mWidth = rectArea.size.width;  // the bar is default to horzontal
    mHeight = rectArea.size.height;
    mHealthBar->setPosition(Vec2(0, 0));
    updateHealthBarPicture();
}
