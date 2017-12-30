//
//  EnemyPlane.h
//  MyGame
//
//  Created by 黄华锋 on 21/12/2017.
//

#ifndef EnemyPlane_h
#define EnemyPlane_h

#include "EnemyObject.h"

class EnemyPlane : public EnemyObject {
    
public:
    EnemyPlane();
    
    virtual EnemyPlane * clone() const override;
    
    static EnemyPlane * create(const std::string &filename);
};

#endif /* EnemyPlane_h */
