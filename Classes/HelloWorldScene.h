#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{
private:
    Sprite *sprite;
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    bool onContactBegan(PhysicsContact &contact);
    
    void menuStartGameCallback(Ref *sender);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
