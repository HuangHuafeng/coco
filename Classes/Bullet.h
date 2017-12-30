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
    virtual void updateRoute() override;
    virtual void collideWith(CollideObject* otherCollideObject) override;

public:
    Bullet();
    
    void setForceType(ForceType type);  // override to set bullet contact test bit mask
    
    virtual float getFireRange();
    virtual Bullet * clone() const override;
    
    static Bullet* create(const std::string &filename, float speed, int damage);
};

#endif /* Bullet_h */
