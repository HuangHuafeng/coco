//
//  FlyingObject.h
//  MyGame
//
//  Created by 黄华锋 on 19/12/2017.
//

#ifndef FlyingObject_h
#define FlyingObject_h

#include "CollideObject.h"

class FlyingObject : public CollideObject {
protected:
    bool mRouteNeedUpdate;
    float mSpeed;
    cocos2d::Vec2 mDestination;
    
protected:
    typedef enum {
        ACTION_TAG = 1,
    } ActionTag;

    virtual void updateRoute();
    
public:
    FlyingObject();

    virtual void update (float delta) override;
    
    void setSpeed(float speed);
    void setDestination(const cocos2d::Vec2 &destination);
    void modifyPosition(const cocos2d::Vec2& pos);
    void stop();
    void invalidRoute();
};

#endif /* FlyingObject_h */
