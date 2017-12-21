//
//  Bullet.h
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#ifndef Bullet_h
#define Bullet_h

#include "FlyingObject.h"

class Bullet : public FlyingObject {

protected:
    int mDamage;
    
    virtual void updateRoute() override;
    virtual void collideWith(CollideObject* otherCollideObject) override;

public:
    Bullet();
    
    void setDamage(int damage);
    int getDamage();
    void setForceType(ForceType type);  // override to set bullet contact test bit mask
    
    virtual float getFireRange();
    
    static Bullet* create(const std::string &filename, const cocos2d::Vec2& position, float speed, int damage, ForceType type = ENEMY);
};

#endif /* Bullet_h */
