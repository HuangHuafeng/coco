//
//  EnemyGenerator.h
//  MyGame
//
//  Created by 黄华锋 on 30/12/2017.
//

#ifndef EnemyGenerator_h
#define EnemyGenerator_h

#include "ObjectGenerator.h"

class EnemyGenerator : public ObjectGenerator {
public:
    typedef enum {
        DP_RandomTo = 1,
        DP_VerticalTo,
        DP_HorizontalTo,
        DP_ToPlayerPlane,
    } DestinationPolicy;

protected:
    DestinationPolicy mDestinationPolicy;
    cocos2d::Rect mFromRect;
    cocos2d::Rect mToRect;

    EnemyGenerator(float interval = 1.0f);
    
    virtual void generateOnce() override;
    
    cocos2d::Vec2 generatePosition();
    cocos2d::Vec2 generateDestination(const cocos2d::Vec2 & from);
    
public:
    static EnemyGenerator * create(float interval = 1.0f);
    virtual EnemyGenerator * clone() const override;
    
    void setFromRect(cocos2d::Rect fromRect);
    void setToRect(cocos2d::Rect toRect);
    void setDestinationPolicy(DestinationPolicy destinationPolicy);
    
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif /* EnemyGenerator_h */
