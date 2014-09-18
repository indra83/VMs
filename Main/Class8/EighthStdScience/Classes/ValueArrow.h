#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class ValueArrow : public cocos2d::extension::Scale9Sprite
{
    float _val;
    cocos2d::LabelTTF * _labelForce;
//    cocos2d::LabelTTF * _labelName;
//    cocos2d::Node * _shaft;
//    cocos2d::Node * _head;
//    cocos2d::Color4B _color;

public :
//    static float BASE_VALUE;
//    static float WIDTH;
//    static float HEIGHT;
//    static void setBaseValue(float value) { BASE_VALUE=value; }

    static ValueArrow * create(const std::string &fileName , const cocos2d::Rect &capInset);
    ValueArrow() :
    	_val(0.0),
    	_labelForce(nullptr){}

    bool initWithFile(const std::string &fileName , const cocos2d::Rect &capInset);
    //TODO: use when the basic function works

    void setValue(float f)
    {
        if( _val != f )
        {
            _val = f;
//            adjustSize();
        }
    }

    float getValue() { return _val; }

    void adjustSize();
};

