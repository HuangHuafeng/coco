//
//  WarObject.h
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#ifndef WarObject_h
#define WarObject_h

#include "FlyingObject.h"
#include "Weapon.h"

class WarObject : public FlyingObject {
    
protected:
    float mCalmPeriod;    // after the carm period, the war object open fire
    Weapon *mWeapon;
    int mHealth;
    
    virtual void collideWith(CollideObject* otherCollideObject) override;
    virtual void OnKilled();
    
public:
    WarObject();
    ~WarObject();

    void openFire();
    void ceaseFire();
    void setCalmPeriod(float calmPeriod);
    void setWeapon(Weapon *weapon);
    void setHealth(int health);
    int getHealth() const;
    
    virtual void onEnter() override;
    virtual void onExit() override;
    //virtual void update (float delta) override;
};

#endif /* WarObject_h */
