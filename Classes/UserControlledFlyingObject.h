//
//  UserControlledFlyingObject.h
//  MyGame
//
//  Created by 黄华锋 on 19/12/2017.
//

#ifndef UserControlledFlyingObject_h
#define UserControlledFlyingObject_h

#include "WarObject.h"

class UserControlledFlyingObject : public WarObject {
    
private:
    double mPreviousTime;
    double mTimeLastFrame;

protected:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    const cocos2d::Vec2 keepInsideScreen(const cocos2d::Vec2& position);
    
public:
    UserControlledFlyingObject();

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float delta) override;
    
};

#endif /* UserControlledFlyingObject_h */
