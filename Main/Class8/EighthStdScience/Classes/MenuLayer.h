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
    cocos2d::Layer * _popupLayer;
    cocos2d::LabelTTF * _popupLabelTitle;
    cocos2d::LabelTTF * _popupLabelCaption;

public :  
    MenuLayer() 
        : _spriteLayer(nullptr), 
          _topMenu(nullptr),
          _topMenuOffsetX(0.0),
          _forceSlider(nullptr),
          _popupLayer(nullptr),
          _popupLabelTitle(nullptr),
          _popupLabelCaption(nullptr)
    {}
    virtual bool init();  

    void addForceMenu(float min, float max, float start, cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);
    void addFrictionMenu(cocos2d::Ref * target, cocos2d::extension::Control::Handler handler);

    CREATE_FUNC(MenuLayer);

    void addPopupMenu(const std::string & title, const std::string & caption, std::function< bool () > cb);
    void addToTopMenu(cocos2d::MenuItem * item, const std::function< void (cocos2d::Ref *)> &cb);

    void disablePopUpMenu();

    bool isShowingPopupMenu() { return _popupLayer != nullptr; }

    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
