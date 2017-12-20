//
//  UserControlledFlyingObject.h
//  MyGame
//
//  Created by 黄华锋 on 19/12/2017.
//

#ifndef UserControlledFlyingObject_h
#define UserControlledFlyingObject_h

#include "FlyingObject.h"

class UserControlledFlyingObject : public FlyingObject {

protected:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
public:
    UserControlledFlyingObject();

    virtual void onEnter() override;
    virtual void onExit() override;
    
    static UserControlledFlyingObject* create(const std::string &filename);
};

#endif /* UserControlledFlyingObject_h */
