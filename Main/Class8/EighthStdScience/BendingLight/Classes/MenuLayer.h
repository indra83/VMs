#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"

class MenuLayer : public cocos2d::Layer
{
private :    
    cocos2d::Menu * _topMenu;

public :
    MenuLayer() :
          _topMenu(nullptr) {}
    virtual bool init();  

    void setTopMenu(cocos2d::Menu * menu);

    CREATE_FUNC(MenuLayer);

    ~MenuLayer();
};

#endif // __MENU_LAYER_H__
