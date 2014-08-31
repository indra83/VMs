#ifndef __SPRITE_LAYER_H__
#define __SPRITE_LAYER_H__

#include "cocos2d.h"

class ValueArrow;
class SpriteLayer : public cocos2d::Layer 
{
public :
    // per person
    static const float MAX_FORCE;
private :
    ValueArrow * _forceFriction;
    ValueArrow * _forceExternal;
    ValueArrow * _sumOfForces; 
    cocos2d::Layer * _crate;
    cocos2d::Layer * _personLayer;
    bool _personPushing;
    std::function< void (float) > _moveCB;
    cocos2d::MenuItemLabel * _speedLabel;
    cocos2d::MenuItemLabel * _massLabel;
    cocos2d::Sprite * _dial;
    cocos2d::Sprite * _needle;
    float _forceExternalValue;
    float _forceFrictionValue;
    float _sumOfForcesValue;
    float _prevSumOfForcesValue;
    float _velocity;
    float _mass;
    float _frictionCoefficient;
<<<<<<< HEAD
    float _angle;
=======
    bool _showAnotherPerson;
>>>>>>> origin/master
public :  
    SpriteLayer() : 
       _forceFriction(nullptr),
       _forceExternal(nullptr),
       _sumOfForces(nullptr),
       _crate(nullptr), 
       _personLayer(nullptr),
       _personPushing(false),
       _speedLabel(nullptr),
       _massLabel(nullptr),
       _forceExternalValue(0.0),
       _forceFrictionValue(0.0),
       _sumOfForcesValue(0.0),
       _prevSumOfForcesValue(0.0),
       _velocity(0.0),
       _mass(0.0),
       _frictionCoefficient(0.0),
       _showAnotherPerson(false) {}

    ~SpriteLayer();

    CREATE_FUNC(SpriteLayer);

    virtual bool init();  
    void changeForceValue(float value);
    void changeFrictionValue(float value);
    void setBackGroundLayer(Layer *layer);
    void addPersonOfForce(float);
    void update(float);
    void setMass(float mass);
    void setFrictionCoefficient(float coeff) { _frictionCoefficient = coeff; }
    void setMoveCB( std::function< void (float) > cb ) { _moveCB = cb; }
    float getExternalForceValue() { return _forceExternalValue; }
    void addAnotherPerson();

private :    
    cocos2d::LabelTTF * getSpeedLabel();
    cocos2d::LabelTTF * getMassLabel();
    float getFrictionalForce();
    void readjustForces();
    
};

#endif // __SPRITE_LAYER_H__
