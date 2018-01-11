//
//  HealthIndicator.h
//  MyGame
//
//  Created by 黄华锋 on 05/01/2018.
//

#ifndef HealthIndicator_h
#define HealthIndicator_h

#include "WarObject.h"

/*
 HealthIndicator is a health bar that indicate the health status of a warObject
    1. links to a warObject
    2. is added as a child to the linked warObject, so that this healthInidcator is released automatically when the warObject is released
    3. should not retain the warObject to avoid deadlock results in healthIndicator and warObject are not released
 */

class HealthIndicator : public GameObject {
public:
    typedef enum {
        DONT_UPDATE = 0,
        STICK_LEFT,
        STICK_RIGHT,
        STICK_TOP,
        STICK_BOTTOM,
    } PositionPolicy;
    
    typedef enum {
        WIDTH_HEIGHT_RATIO = 20,
    } HeightWidthRatio;

protected:
    HealthIndicator(cocos2d::Color4F initialHealthColor, cocos2d::Color4F currentHealthColor, PositionPolicy positionPolicy);
    
    cocos2d::DrawNode *mHealthBar;
    WarObject *mLinkedWarObject;
    PositionPolicy mPositionPolicy;
    float mHeight, mWidth;
    cocos2d::Color4F mInitialHealthColor, mCurrentHealthColor;
    
    void updateHealthBarPicture();
    void onWarObjectChanged(WarObject *warObject, WarObject::ChangeTypes changeType);
    void linkToObject(WarObject *warObject);
    void unlink();
    
public:
    ~HealthIndicator();
    
    static HealthIndicator * createHealthIndicatorForObject(WarObject *warObject, cocos2d::Color4F initialHealthColor = cocos2d::Color4F(1.0, 0.0, 0.0, 0.5), cocos2d::Color4F currentHealthColor = cocos2d::Color4F(0.0, 1.0, 0.0, 0.75), PositionPolicy positionPolicy = STICK_BOTTOM);
    
    void dontMoveWithLinkedObject(cocos2d::Rect rectArea);
};
#endif /* HealthIndicator_h */
