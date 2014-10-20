#ifndef __BACKGROUND_LAYER_H__
#define __BACKGROUND_LAYER_H__

#include "cocos2d.h"

class BackGroundLayer : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(BackGroundLayer);
    // void setSurface(const std::string &name, float startPos, float endPos);
private:
    // float _lower_boundary;
};

#endif // __BACKGROUND_LAYER_H__
