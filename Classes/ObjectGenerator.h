//
//  ObjectGenerator.h
//  MyGame
//
//  Created by 黄华锋 on 30/12/2017.
//

#ifndef ObjectGenerator_h
#define ObjectGenerator_h

#include "GameObject.h"

class ObjectGenerator : public GameObject {
protected:
    ObjectGenerator(float interval);
    
    GameObject *mObject;
    float mInterval;

    // generateOnce() is called every mInterval seconds repeatedly.
    // subclass should implement the function to generate actual object(s)
    virtual void generateOnce() = 0;
    
public:
    ~ObjectGenerator();
    
    void setObject(GameObject *object);
    void setInterval(float interval);
    virtual void start();
    virtual void stop();
    
    //virtual void onEnter() override;
    //virtual void onExit() override;
};

#endif /* ObjectGenerator_h */
