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
    virtual bool collideWith(CollideObject* otherCollideObject) override;

public:
    Bullet();
    
    void setDamage(int damage);
    int getDamage();
    
    static Bullet* create(const std::string &filename);
};

#endif /* Bullet_h */
