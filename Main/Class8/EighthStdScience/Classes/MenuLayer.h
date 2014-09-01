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
    float _topMenuOffsetX;
    cocos2d::Layer * _forceSlider;

public :
    enum SurfaceType { ICE, GRASS, GRAVEL };
    MenuLayer() 
        : _spriteLayer(nullptr), 
          _topMenu(nullptr),
          _topMenuOffsetX(0.0),
          _forceSlider(nullptr)
    {}
    virtual bool init();  

    void addForceMenu(float min, float max, float start, cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);
    void addSurfaceMenu(std::function<void (cocos2d::Ref *)> cb);

    CREATE_FUNC(MenuLayer);

    void addToTopMenu(cocos2d::MenuItem * item, const std::function< void (cocos2d::Ref *)> &cb);

    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
