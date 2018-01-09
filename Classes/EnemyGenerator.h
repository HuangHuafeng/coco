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
    } DestinationPolicy;

protected:
    float mStart;
    cocos2d::Rect mDestinationRect;

    EnemyGenerator(float interval = 1.0f);
    
    virtual void generateOnce() override;
    
public:
    static EnemyGenerator * create(float interval = 1.0f);
    virtual EnemyGenerator * clone() const override;
    
    
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif /* EnemyGenerator_h */
