#ifndef __SPRITE_LAYER_H__
#define __SPRITE_LAYER_H__

#include "cocos2d.h"

class SpriteAlt;
class ValueArrow;
class SpriteLayer : public cocos2d::Layer 
{
    ValueArrow * _forceFriction;
    ValueArrow * _forceExternal;
    ValueArrow * _sumOfForces; 
    SpriteAlt * _crate;
    cocos2d::Sprite *_person;
    cocos2d::PhysicsWorld *_world; 
    cocos2d::PhysicsShape *_shape; 
    cocos2d::MenuItemLabel * _massLabel;
    cocos2d::MenuItemLabel * _speedLabel;
    float _forceExternalValue;
public :  
    SpriteLayer() : 
       _crate(nullptr), 
       _person(nullptr),
       _shape(nullptr),
       _world(nullptr),
       _forceFriction(nullptr),
       _forceExternal(nullptr),
       _sumOfForces(nullptr),
       _speedLabel(nullptr),
       _massLabel(nullptr),
       _forceExternalValue(0.0) {}

    ~SpriteLayer();
    virtual bool init();  
    void changeForceValue(float value);
    void changeFrictionValue(float value);
    void setPhysicsWorld(cocos2d::PhysicsWorld * world) { _world = world; }
    void setBackGroundLayer(Layer *layer);
    //void showMasses(bool enable);
    void showSumOfForces(bool enable);
    //void showForceValues(bool enable);
    //void showSpeeds(bool enable);
    //void showForces(bool enable);
    void addPersonOfForce(float);
    void update(float);
    cocos2d::LabelTTF * getSpeedLabel();
    cocos2d::LabelTTF * getMassLabel();
    CREATE_FUNC(SpriteLayer);
};

#endif // __SPRITE_LAYER_H__
