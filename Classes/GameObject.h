//
//  GameObject.h
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

// base class for all the game

#ifndef GameObject_h
#define GameObject_h

class GameObject : public cocos2d::Sprite, public cocos2d::Clonable {
    
protected:
    int mId;
    std::string mName;
    
public:
    GameObject();
    
    const std::string & getName();
    void setObjectName(const std::string & name);
    int getObjectId() const;
    void setObjectId(int id);
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual GameObject * clone() const override;
};

#endif /* GameObject_h */
