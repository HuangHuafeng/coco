//
//  GameObject.h
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

// base class for all the game

#ifndef GameObject_h
#define GameObject_h

class GameObject : public cocos2d::Sprite {
    
protected:
    int mId;
    std::string mName;
    
public:
    GameObject();
    
    const std::string & getName();
    void setName(const std::string & name);
    int getId() const;
    void setId(int id);
};

#endif /* GameObject_h */
