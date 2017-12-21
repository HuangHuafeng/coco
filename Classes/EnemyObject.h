//
//  EnemyObject.h
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#ifndef EnemyObject_h
#define EnemyObject_h

#include "WarObject.h"

class EnemyObject : public WarObject {
    
protected:
    bool mSelfDestroy;
    
    virtual void updateRoute() override;
    virtual void collideWith(CollideObject* otherCollideObject) override;
    
    void setSelfDestroy(bool selfDestroy);
    
public:
    EnemyObject();
};

#endif /* EnemyObject_h */
