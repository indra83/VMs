#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class SpriteLayer;
class BackGroundLayer;

class MenuLayer : public cocos2d::Layer
{
private :    
    SpriteLayer * _spriteLayer;
    cocos2d::Menu * _topMenu;
public :  
    MenuLayer() 
        : _spriteLayer(nullptr), 
          _topMenu(nullptr)
    {}
    virtual bool init();  

    void addForceMenu(float min, float max, float start, cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);
    void addFrictionMenu(cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);

    CREATE_FUNC(MenuLayer);

    void setSpriteLayer(SpriteLayer * layer);

    void addToTopMenu(cocos2d::MenuItem * item);

    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
