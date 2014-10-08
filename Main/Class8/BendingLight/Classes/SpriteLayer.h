#ifndef __SPRITE_LAYER_H__
#define __SPRITE_LAYER_H__

#include "cocos2d.h"
#include "BackGroundLayer.h"

class SpriteLayer : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(SpriteLayer);
};

#endif //__SPRITE_LAYER_H__
