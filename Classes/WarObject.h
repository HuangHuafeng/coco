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
public:
    typedef enum {
        CT_Health = 1,
        CT_Exit,
    } ChangeTypes;
    typedef std::function<void(WarObject *, ChangeTypes)> ChangeCallback;
    
protected:
    float mCalmPeriod;    // after the carm period, the war object open fire
    Weapon *mWeapon;
    int mInitialHealth;            // initial/max health
    int mCurrentHealth;     // current health
    std::map<GameObject *, ChangeCallback> mChangeCallbacks;
    
    virtual void collideWith(CollideObject* otherCollideObject) override;
    virtual void OnKilled();
    
    void callCallbacks(ChangeTypes changeType);
    
public:
    WarObject();
    ~WarObject();

    void openFire();
    void ceaseFire();
    void setCalmPeriod(float calmPeriod);
    void setWeapon(Weapon *weapon);
    void setInitialHealth(int health);
    int getInitialHealth() const;
    void setCurrentHealth(int health);
    int getCurrentHealth() const;
    void registerChangeCallback(GameObject * gameObject, ChangeCallback changeCallback);
    void unregisterChangeCallback(GameObject * gameObject);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    //virtual void update(float delta) override;
    
};

#endif /* WarObject_h */
