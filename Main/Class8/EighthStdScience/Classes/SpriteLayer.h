#ifndef __SPRITE_LAYER_H__
#define __SPRITE_LAYER_H__

#include "cocos2d.h"
#include "MenuLayer.h"

class ValueArrow;

class Movable
{
    float _vel;
    float _scale;
    cocos2d::Node * _node;
public:
    Movable(float vel, float scale, cocos2d::Node * node) 
    {
        _vel = vel;
        _scale = scale;
        _node = node;
    }

    Movable(const Movable &m)
    {
        _vel = m._vel;
        _scale = m._scale;
        _node = m._node;
    }

    bool operator== (const Movable& mov)
    {
        return mov._node == _node;
    }

    float getScale() { return _scale; }
    float getVelocity() { return _vel; }
    cocos2d::Node * getNode() { return _node; }

};

class SpriteLayer : public cocos2d::Layer 
{
public :
    // per person
    static const float MAX_FORCE;
    static const int PTM_RATIO;
    static const float MINI_MAP_SCALE;

    static const int STATIONARY_ZINDEX;
    static const int TROLLEY_RIGHT_ZINDEX;
    static const int SPRITE_ZINDEX;
    static const int TROLLEY_LEFT_ZINDEX;
    static const int PERSON_ZINDEX;
    static const int LABEL_ZINDEX;


private :
    ValueArrow * _forceFriction;
    ValueArrow * _forceExternal;
    ValueArrow * _sumOfForces; 
    cocos2d::Node * _crate;
    cocos2d::Layer * _personLayer;
    cocos2d::Sprite * _person;
    cocos2d::Sprite * _anotherPerson;
    std::function< bool (float) > _periodicCB;
    cocos2d::LabelTTF * _speedLabel;
    cocos2d::LabelTTF * _massLabel;
    cocos2d::Sprite * _needle;
    float _forceExternalValue;
    float _forceFrictionValue;
    float _sumOfForcesValue;
    float _prevSumOfForcesValue;
    float _velocity;
    float _mass;
    float _frictionCoefficient;
    float _angle;
    bool _showAnotherPerson;
    std::vector< Movable > _movables;
    bool _personFell;
    MenuLayer * _menuLayer;
    Layer * _minimap;
    float _minimapOffset;

public :  
    SpriteLayer() : 
       _forceFriction(nullptr),
       _forceExternal(nullptr),
       _sumOfForces(nullptr),
       _crate(nullptr), 
       _personLayer(nullptr),
       _person(nullptr),
       _anotherPerson(nullptr),
       _speedLabel(nullptr),
       _massLabel(nullptr),
       _forceExternalValue(0.0),
       _forceFrictionValue(0.0),
       _sumOfForcesValue(0.0),
       _prevSumOfForcesValue(0.0),
       _velocity(0.0),
       _mass(0.0),
       _frictionCoefficient(0.0),
       _showAnotherPerson(false),
       _periodicCB(),
       _movables(),
       _personFell(false),
       _menuLayer(nullptr),
       _minimap(nullptr),
       _minimapOffset(0.0) {}

    ~SpriteLayer();

    CREATE_FUNC(SpriteLayer);

    bool init() override;
    void changeForceValue(float value);
    void changeFrictionValue(float value);
    void setBackGroundLayer(Layer *layer);
    void addPersonOfForce();
    void update(float) override;
    void setMass(float mass);
    void setFrictionCoefficient(float coeff) { _frictionCoefficient = coeff; }
    void setFriction(float coeff, cocos2d::Color3B color, float startPos, float endPos);
    void setPeriodicCB( std::function< bool (float) > cb ) { _periodicCB = cb; }
    float getExternalForceValue() { return _forceExternalValue; }
    void addAnotherPerson();
    Node * addStationaryChild(std::function< cocos2d::Node * () > generator, cocos2d::Vec2 pos);
    Node * addMovingChild(std::function< cocos2d::Node * () > generator, float vel, int zIndex, cocos2d::Vec2 pos, bool baseMover);
    void addToMovables( Node * node, float vel=0.0, float scale=1.0 );
    void removeFromMovables( cocos2d::Node * node );
    void setMenuLayer( MenuLayer * layer) { _menuLayer = layer; }
    void setMiniMapOffset(float off);

private :    
    std::string getMassString();
    std::string getSpeedString();
    float getFrictionalForce();
    void readjustForces();
};

#endif // __SPRITE_LAYER_H__
