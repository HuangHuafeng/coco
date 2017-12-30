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
    
public:
    FriendPlane();
    
    virtual FriendPlane * clone() const override;
    
    static FriendPlane * create(const std::string &filename);
};
#endif /* FriendPlane_h */
