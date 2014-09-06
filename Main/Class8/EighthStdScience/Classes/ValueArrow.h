#pragma once

#include "cocos2d.h"

class ValueArrow : public cocos2d::Layer
{
    float _val;
    cocos2d::LabelTTF * _labelForce;
    cocos2d::LabelTTF * _labelName;
    cocos2d::Node * _shaft;
    cocos2d::Node * _head;
    cocos2d::Color4B _color;

public :
    static float BASE_VALUE;
    static float WIDTH;
    static float HEIGHT;
    static void setBaseValue(float value) { BASE_VALUE=value; }
    static ValueArrow * create(const cocos2d::Color4B &color, const std::string &name);
    ValueArrow() : _val(0.0), _labelForce(nullptr), _labelName(nullptr), _shaft(nullptr), _head(nullptr) {}
    bool init(const cocos2d::Color4B &color, const std::string & name);
    void setValue(float f)
    {
        if( _val != f )
        {
            _val = f;
            adjustSize();
        }
    }

    float getValue() { return _val; }
    void adjustSize();
};

