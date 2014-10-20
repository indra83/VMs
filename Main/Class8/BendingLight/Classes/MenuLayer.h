#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

// class ControlSliderRollBack;

class MenuLayer : public cocos2d::Layer
{
private :    
    cocos2d::Menu * _topMenu;
    // cocos2d::Layer * _forceLayer;
    // ControlSliderRollBack * _forceSlider;

public :
    // enum SurfaceType { ICE, GRASS, GRAVEL, LAST };
    // struct SurfInfo
    // {
    //     float coeff;
    //     std::string label;
    //     std::string sprite;
    //     cocos2d::Color4F color;
    // };

    // static SurfInfo SURF_INFO[LAST];  
    MenuLayer() :
          _topMenu(nullptr) {}
    virtual bool init();  

    // void addForceMenu(float min, float max, float start, cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);
    // void addSurfaceMenu(std::function<void (cocos2d::Ref *)> cb);
    // void setForceSliderValue(float force);
    // void showForceSlider(bool show);
    // cocos2d::Menu* selectSurfaceFriction(const std::string &surface_no , const cocos2d::Vec2 &label_pos , const cocos2d::Vec2 &menu_pos, std::function<void(int)> cb);
    void setTopMenu(cocos2d::Menu * menu);

    CREATE_FUNC(MenuLayer);

    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
