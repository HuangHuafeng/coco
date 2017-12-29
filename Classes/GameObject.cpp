//
//  GameObject.cpp
//  MyGame
//
//  Created by 黄华锋 on 20/12/2017.
//

#include "GameObject.h"

GameObject::GameObject()
{
    mId = 0;
    mName = "";
}


const std::string & GameObject::getName()
{
    return mName;
}

void GameObject::setName(const std::string & name)
{
    mName = name;
}

int GameObject::getId() const
{
    return mId;
}

void GameObject::setId(int id)
{
    mId = id;
}
