#ifndef __CHALLENGE1_SCENE_H__
#define __CHALLENGE1_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "Challenge.h"

class Challenge1: public Challenge<Challenge1>
{
public:
    virtual bool init();  
};

#endif // __CHALLENGE1_SCENE_H__
