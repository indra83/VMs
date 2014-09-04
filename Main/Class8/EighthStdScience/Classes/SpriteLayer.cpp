#include "SpriteLayer.h"
#include "extensions/cocos-ext.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>

USING_NS_CC;
USING_NS_CC_EXT;

static const int STATIONARY_ZINDEX = 0;
static const int SPRITE_ZINDEX = 1;
static const int PERSON_ZINDEX = 2;
static const int LABEL_ZINDEX = 3;

static const float EPSILON=0.2;
static const int NUM_IMAGES=15;

static const float MAX_ANGLE=270.0;
static const float OFFSET_ANGLE=-45.0;
static const float MAX_SPEED=50.0;

// per person;
const float SpriteLayer::MAX_FORCE=100;

// TODO: base it on the crate size.. assuming the crate is 1m wide
const int SpriteLayer::PTM_RATIO = 150;

class ValueArrow : public Layer
{
    float _val;
    LabelTTF * _label;
    Scale9Sprite * _sprite;
    std::string _name;

public :
    static ValueArrow * create(const std::string &fileName, const std::string &name)
    {
        ValueArrow *layer = new ValueArrow();
        if (layer && layer->init(fileName, name))
        {
            layer->autorelease();
            return layer;
        }
        CC_SAFE_DELETE(layer);
        return nullptr;
    }

    ValueArrow() : _val(0.0), _label(nullptr), _name() {}

    bool init(const std::string & fileName, const std::string & name)
    {
        if ( !Layer::init() )
            return false;
        _label = LabelTTF::create("", "fonts/Maven Pro Black.otf", 30);
        _label->setHorizontalAlignment(TextHAlignment::LEFT);
        _label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        addChild(_label);

        _name = name;
        _sprite = Scale9Sprite::create(fileName);
        addChild(_sprite);

        adjustSize();
        return true;
    }

    void setValue(float f)
    {
        if( _val != f )
        {
            _val = f;
            adjustSize();
        }
    }

    float getValue() { return _val; }

    void adjustSize()
    {
        auto val = fabs(_val);
        std::stringstream sstr;
        sstr << (int)val << " N";
        _label->setString(sstr.str());

        setAnchorPoint(_val < 0.0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT);

        _label->setPosition(Vec2(getContentSize().width * getScaleX()/2 , 0.0));
        //_sprite->setFlippedX(_val < 0);

        // Stretches content proportional to newLevel
        float scale = val/SpriteLayer::MAX_FORCE;
        setScaleX(scale/1.7);
    }
};

// on "init" you need to initialize your instance
bool SpriteLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //////////////////////////////
    // 2. add crate
    _crate = Layer::create();
    _crate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _crate->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/3 + origin.y + 15));

    auto crate = Sprite::create("crate.png");
    crate->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    crate->setPosition(Vec2::ZERO);
    _crate->setContentSize(crate->getContentSize());
    _crate->addChild(crate, SPRITE_ZINDEX);

    //////////////////////////////
    // 3. add mass label
    _massLabel = LabelTTF::create(getMassString().c_str(), "fonts/Marker Felt.ttf", 30);
    _massLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    auto menuLabel1 = MenuItemLabel::create(_massLabel);
    menuLabel1->setColor(Color3B::BLACK);
    menuLabel1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    menuLabel1->setPosition(Vec2(-crate->getContentSize().width/2, 0.0));
    _crate->addChild(menuLabel1, LABEL_ZINDEX);

    this->addChild(_crate, SPRITE_ZINDEX);

    //////////////////////////////
    // 3. add the force arrows
    Vec2 curr(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
    auto addArrow = [&](const std::string & fileName, const std::string & name) -> ValueArrow *
    {
        auto arrow = ValueArrow::create(fileName, name);
        arrow->setScaleY(0.25);
        arrow->setPosition(curr);
        curr = curr + Vec2(0, arrow->getContentSize().height / 4);
        this->addChild(arrow, LABEL_ZINDEX);
        return arrow;
    };

    _sumOfForces = addArrow("arrow-sof.png", "Net");
    _forceFriction = addArrow("arrow-fr.png", "Friction");
    _forceExternal = addArrow("arrow-force.png", "Applied");


    //////////////////////////////
    // 4. add the speed related display
    auto speedLayer = Layer::create();

    // label
    _speedLabel = LabelTTF::create(getSpeedString().c_str(), "fonts/Marker Felt.ttf", 30);
    _speedLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    _speedLabel->setColor(Color3B::BLACK);
    speedLayer->addChild(_speedLabel);

    // speed-o-meter
    // dial
    auto dial = Sprite::create("dial.png");
    //dial->setPosition(Vec2(visibleSize.width/6 ,visibleSize.height/6));
    dial->setPosition(Vec2(0.0, 10.0));
    dial->setScale(0.8);
    speedLayer->addChild(dial);
    // needle
    _needle = Sprite::create("needle.png");
    //_needle->setAnchorPoint(Vec2(0.85, 0.5));
    dial->setPosition(Vec2(0.0, 10.0));
    _needle->setPosition(Vec2(visibleSize.width/6, visibleSize.height/6));
    _needle->setScale(0.8);
    _needle->setRotation(OFFSET_ANGLE);
    speedLayer->addChild(_needle, 1);

    speedLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    speedLayer->setPosition(Vec2(visibleSize.width, visibleSize.height/3 + 10));
    addChild(speedLayer);

    this->addPersonOfForce(0.0);
    this->scheduleUpdate();

    return true;
}

int getIndexFromForce(float force)
{
    static const int DELTA = SpriteLayer::MAX_FORCE/NUM_IMAGES;
    auto val = floor(force/DELTA);
    if (val >= NUM_IMAGES) val = NUM_IMAGES - 1; // for the exactly MAX_FORCE case
    return val;
}

void SpriteLayer::addPersonOfForce(float force)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto beforeCrate = Vec2(visibleSize.width/2 + origin.x - _crate->getContentSize().width/2,
                    visibleSize.height/3 + 15 +origin.y);
    auto afterCrate = Vec2(visibleSize.width/2 + origin.x + _crate->getContentSize().width/2,
                    visibleSize.height/3 + 15 +origin.y);

    auto prevPosition = beforeCrate;
    auto prevAnchorPoint = Vec2::ANCHOR_BOTTOM_RIGHT;
    if (_personLayer)
    {
        prevPosition = _personLayer->getPosition();
        prevAnchorPoint = _personLayer->getAnchorPoint();
        removeFromMovables(_personLayer);
        removeChild(_personLayer);
    }

    std::stringstream sstr;
    if (force == 0)
        sstr << "pusher_straight_on.png";
    else
    {
        sstr << "pusher_" << getIndexFromForce(fabs(_showAnotherPerson ? force/2 : force)) << ".png";
    }

    _personLayer = Layer::create();
    _personLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    auto person = Sprite::create(sstr.str());
    _personLayer->addChild(person);
    _personLayer->setContentSize(person->getContentSize());

    Sprite * anotherPerson = nullptr;
    if (_showAnotherPerson)
    {
        anotherPerson = Sprite::create(sstr.str());
        anotherPerson->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _personLayer->setContentSize(person->getContentSize() + anotherPerson->getContentSize());
        _personLayer->addChild(anotherPerson);
    }

    if (force == 0)
    {
        _personLayer->setPosition(prevPosition);
        _personLayer->setAnchorPoint(prevAnchorPoint);
        person->setPosition(Vec2(0.0, 0.0));
        person->setAnchorPoint(prevAnchorPoint);
        if (anotherPerson)
        {
            anotherPerson->setAnchorPoint(prevAnchorPoint);
            auto direction = prevAnchorPoint == Vec2::ANCHOR_BOTTOM_RIGHT ? -1 : 1;
            anotherPerson->setPosition(Vec2(direction * ( person->getContentSize().width + 10 ), 0.0));
        }
    }
    else
    {
        _personLayer->setAnchorPoint( force > 0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT );
        _personLayer->setPosition( force > 0 ? beforeCrate : afterCrate);

        person->setAnchorPoint( force > 0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT );
        person->setPosition(Vec2::ZERO);

        if (anotherPerson)
        {
            anotherPerson->setAnchorPoint( force > 0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT );
            auto direction = force > 0 ? -1 : 1;
            anotherPerson->setPosition(Vec2( direction * 10, 0.0));
        }
    }

    if (force < 0)
    {
        person->setFlippedX(true);
        if (_showAnotherPerson)
            anotherPerson->setFlippedX(true);
    }

    addChild(_personLayer, PERSON_ZINDEX);
    if (force == 0)
        addToMovables(_personLayer);
}

float SpriteLayer::getFrictionalForce()
{
    float fric = 0.0;
    float gravity = 10.0;
    float max = _frictionCoefficient * _mass * gravity;
    if (fabs(_velocity) > EPSILON)
        fric = _velocity > 0.0 ? -max : max;
    else if (fabs(_forceExternalValue) != 0.0)
    {
        if (fabs(_forceExternalValue) > max) 
            fric = _forceExternalValue > 0 ? -max : max;
        else
            fric = -_forceExternalValue;
    }
    return fric;
}

void SpriteLayer::readjustForces()
{
    _forceFrictionValue = getFrictionalForce();
    _forceFriction->setValue(_forceFrictionValue);
    _sumOfForcesValue = _forceExternalValue + _forceFrictionValue;
    _sumOfForces->setValue(_sumOfForcesValue);
}

void SpriteLayer::changeForceValue(float value)
{
    if(value != _forceExternalValue)
        addPersonOfForce(value);

    _forceExternalValue = value;
    _forceExternal->setValue(_forceExternalValue);

    readjustForces();
}

void SpriteLayer::changeFrictionValue(float value)
{
    _frictionCoefficient = value;
}

std::string SpriteLayer::getMassString()
{
    std::stringstream sstr;
    sstr << _mass << " Kg";
    return sstr.str();
}

std::string SpriteLayer::getSpeedString()
{
    std::stringstream sstr;
    sstr << "Speed - " << (int)_velocity << " m/sec";
    return sstr.str();
}

void SpriteLayer::setMass(float mass)
{
    _mass = mass;
    _massLabel->setString(getMassString());
}

void SpriteLayer::update(float dt)
{
    if (_periodicCB && !_periodicCB())
        return;

    readjustForces();

    float acc = _prevSumOfForcesValue / _mass;
    float dv = acc * dt;
    _velocity += dv;
    float dx = 0.0;
    if(fabs(_velocity) > EPSILON)
        dx = _velocity * dt * PTM_RATIO;
    _speedLabel->setString(getSpeedString());

    if(fabs(dx) > 0.0)
    {
        for( auto node : _movables )
            node->runAction(Place::create(node->getPosition() + Vec2(-dx, 0.0)));
    }

    _prevSumOfForcesValue = _sumOfForcesValue;

    // speed-o-meter implementation
    auto angle = ( ( fabs(_velocity) / MAX_SPEED ) * MAX_ANGLE ) + OFFSET_ANGLE;
    _needle->setRotation(angle);

    Node::update(dt);
}

void SpriteLayer::addStationaryChild(Node * node)
{
    addChild( node, STATIONARY_ZINDEX);
    addToMovables(node);
}

void SpriteLayer::addToMovables( Node * node )
{
    _movables.push_back(node);
}

void SpriteLayer::removeFromMovables( Node * node )
{
    auto f = std::find(_movables.begin(), _movables.end(), node); 
    if (f != _movables.end())
        _movables.erase(f); 
}

void SpriteLayer::addAnotherPerson()
{
    _showAnotherPerson = true;
    addPersonOfForce(getExternalForceValue());
}

SpriteLayer::~SpriteLayer()
{
    unscheduleUpdate();
}
