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
    Weapon *mWeapon;
    
public:
    WarObject();
    ~WarObject();

    void openFire();
    void ceaseFire();
    void setWeapon(Weapon *weapon);
    
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif /* WarObject_h */
