//
//  FriendPlane.h
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#ifndef FriendPlane_h
#define FriendPlane_h

#include "UserControlledFlyingObject.h"

class FriendPlane : public UserControlledFlyingObject {
    
protected:
    virtual void collideWith(CollideObject* otherCollideObject) override;
    
public:
    FriendPlane();
    
    virtual FriendPlane * clone() const override;
    //virtual void onEnter() override;
    //virtual void onExit() override;
    
    static FriendPlane * create(const std::string &filename);
};
#endif /* FriendPlane_h */
