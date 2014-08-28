#ifndef __SPRITE_LAYER_H__
#define __SPRITE_LAYER_H__

#include "cocos2d.h"

class ValueArrow;
class SpriteLayer : public cocos2d::Layer 
{
    ValueArrow * _forceFriction;
    ValueArrow * _forceExternal;
    ValueArrow * _sumOfForces; 
    cocos2d::Sprite * _crate;
    cocos2d::Sprite *_person;
    cocos2d::MenuItemLabel * _massLabel;
    cocos2d::MenuItemLabel * _speedLabel;
    float _forceExternalValue;
    float _forceFrictionValue;
    float _sumOfForcesValue;
    float _velocity;
    float _mass;
    float _frictionCoefficient;
public :  
    SpriteLayer() : 
       _crate(nullptr), 
       _person(nullptr),
       _speedLabel(nullptr),
       _massLabel(nullptr),
       _velocity(0.0),
       _mass(0.0),
       _forceFriction(nullptr),
       _forceExternal(nullptr),
       _sumOfForces(nullptr),
       _forceExternalValue(0.0),
       _forceFrictionValue(0.0),
       _sumOfForcesValue(0.0) {}

    ~SpriteLayer();

    CREATE_FUNC(SpriteLayer);

    virtual bool init();  
    void changeForceValue(float value);
    void changeFrictionValue(float value);
    void setBackGroundLayer(Layer *layer);
    void addPersonOfForce(float);
    void update(float);

private :    
    cocos2d::LabelTTF * getSpeedLabel();
    cocos2d::LabelTTF * getMassLabel();
    float getFrictionalForce();
    
};

#endif // __SPRITE_LAYER_H__
