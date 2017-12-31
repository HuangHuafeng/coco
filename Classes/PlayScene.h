//
//  PlayScene.h
//  MyGame
//
//  Created by 黄华锋 on 31/12/2017.
//

#ifndef PlayScene_h
#define PlayScene_h

#include "GameScene.h"

class PlayScene : public GameScene {
    
public:
    static PlayScene * createScene(const std::string &sceneDataInJson);
    CREATE_FUNC(PlayScene);

    void menuStartGameCallback(Ref *sender);
    
    virtual bool init() override;
    virtual void onFriendPlaneExit() override;
};

#endif /* PlayScene_h */
