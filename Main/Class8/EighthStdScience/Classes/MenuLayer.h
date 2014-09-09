#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class ControlSliderRollBack;

class MenuLayer : public cocos2d::Layer
{
private :    
    cocos2d::Menu * _topMenu;
    float _topMenuOffsetX;
    cocos2d::Layer * _forceLayer;
    ControlSliderRollBack * _forceSlider;

public :
    enum SurfaceType { ICE, GRASS, GRAVEL };
    MenuLayer() :
          _topMenu(nullptr),
          _topMenuOffsetX(0.0),
          _forceLayer(nullptr),
          _forceSlider(nullptr)
    {}
    virtual bool init();  

    void addForceMenu(float min, float max, float start, cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);
    void addSurfaceMenu(std::function<void (cocos2d::Ref *)> cb);
    void radioSelectSurface(cocos2d::Object *pSender);
    void setForceSliderValue(float force);

    CREATE_FUNC(MenuLayer);

    void addToTopMenu(cocos2d::MenuItem * item, const std::function< void (cocos2d::Ref *)> &cb);

    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
