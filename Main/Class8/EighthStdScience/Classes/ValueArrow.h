#pragma once

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

class ValueArrow : public cocos2d::extension::Scale9Sprite
{
    float _val;
    cocos2d::LabelTTF * _labelForce;

public :
    float _contWD;
    float _contHT;

    static ValueArrow * create(const std::string &fileName , const cocos2d::Rect &capInset);
    ValueArrow() :
    	_val(0.0),
    	_labelForce(nullptr),
    	_contWD(0.0),
    	_contHT(0.0) {}

    bool initWithFile(const std::string &fileName , const cocos2d::Rect &capInset);
    void adjustSize();
    void defaultArrowSize();
    // TODO: understand its implementation and logic
    void setValue(float f)
    {
        if( _val != f )
        {
            _val = f;
            adjustSize();
        }
    }
//    float getValue() { return _val; }
};

