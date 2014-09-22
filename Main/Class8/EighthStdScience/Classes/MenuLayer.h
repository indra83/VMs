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
    enum SurfaceType { ICE, GRASS, GRAVEL, LAST };
    struct SurfInfo
    {
        float coeff;
        std::string label;
        std::string sprite;
        cocos2d::Color3B color;
    };

    static SurfInfo SURF_INFO[LAST];  
    MenuLayer() :
          _topMenu(nullptr),
          _topMenuOffsetX(0.0),
          _forceLayer(nullptr),
          _forceSlider(nullptr)
    {}
    virtual bool init();  

    void addForceMenu(float min, float max, float start, cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);
    void addSurfaceMenu(std::function<void (cocos2d::Ref *)> cb);
    void setForceSliderValue(float force);
    cocos2d::Menu* selectSurfaceFriction(const std::string &surface_no , const cocos2d::Vec2 &label_pos , const cocos2d::Vec2 &menu_pos, std::function<void(int)> cb);

    CREATE_FUNC(MenuLayer);

    void addToTopMenu(cocos2d::MenuItem * item, const std::function< void (cocos2d::Ref *)> &cb);

    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
