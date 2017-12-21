//
//  CollideObject.h
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#ifndef CollideObject_h
#define CollideObject_h

#include "GameObject.h"

typedef enum {
    FRIEND = 1,
    ENEMY = 2,
} ForceType;

class CollideObject : public GameObject {
    
protected:
    ForceType mForceType;

    virtual bool initialize();
    
    typedef enum {
        CategoryMaskFriend = 0x01,  // 0001
        CategoryMaskEnemy = 0x02,   // 0010
        CategoryMaskFriendBullet = 0x04,  // 0100
        CategoryMaskEnemyBullet = 0x08,   // 1000
    } CategoryMask;
    
    typedef enum {
        CollisionMaskFriend = 0x02,    // 0010
        CollisionMaskEnemy = 0x01,     // 0001
    } CollisionMask;
    
    typedef enum {
        ContactTestBitmaskFriend = 0x0A,    // 1010
        ContactTestBitmaskEnemy = 0x05,     // 0101
        ContactTestBitmaskFriendBullet = 0x02,    // 0010
        ContactTestBitmaskEnemyBullet = 0x01,     // 0001
        // supper bullet?
    } ContactTestBitmask;
    
    
    bool onContactBegan(cocos2d::PhysicsContact &contact);
    virtual void collideWith(CollideObject* otherCollideObject);
    
public:
    CollideObject();
    
    void setForceType(ForceType type = ENEMY);
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif /* CollideObject_h */
