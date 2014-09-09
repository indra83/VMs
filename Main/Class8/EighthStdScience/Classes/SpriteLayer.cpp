#include "SpriteLayer.h"
#include "extensions/cocos-ext.h"
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include "NativeHelper.h"
#include "ValueArrow.h"

USING_NS_CC;
USING_NS_CC_EXT;

const int SpriteLayer::STATIONARY_ZINDEX = 0;
const int SpriteLayer::TROLLEY_RIGHT_ZINDEX = 1;
const int SpriteLayer::SPRITE_ZINDEX = 2;
const int SpriteLayer::PERSON_ZINDEX = 3;
const int SpriteLayer::LABEL_ZINDEX = 4;
const int SpriteLayer::TROLLEY_LEFT_ZINDEX = 5;

static const float EPSILON=0.2;
static const int NUM_IMAGES=15;

static const float MAX_ANGLE=270.0;
static const float OFFSET_ANGLE=-45.0;
static const float MAX_SPEED=50.0;

const float SpriteLayer::MINI_MAP_SCALE=0.10;

// per person;
const float SpriteLayer::MAX_FORCE=100;

// TODO: base it on the crate size.. assuming the crate is 1m wide
const int SpriteLayer::PTM_RATIO = 50;
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
    // 2. add mini map
    _minimap = Layer::create();

    _minimap->setPosition(Vec2(0.0, visibleSize.height/10));
    _minimap->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _minimap->setPosition(Vec2(0.0, visibleSize.height/10));

    auto drawNode = DrawNode::create();
    drawNode->drawSegment(Vec2::ZERO, Vec2(visibleSize.width, 0.0), 1.0, Color4F::BLACK);
    drawNode->setPosition(Vec2::ZERO);
    _minimap->addChild(drawNode);

    addChild(_minimap, LABEL_ZINDEX);

    //////////////////////////////
    // 3. add crate
    
    auto gen = []() -> Node *
    {
        auto node = Sprite::create("crate.png");
        node->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
        return node;
    };
    _crate = addMovingChild(gen, 0.0, SPRITE_ZINDEX, Vec2(0.0, visibleSize.height/3 + 15), true);


    //////////////////////////////
    // 4. add mass label
    // TODO: fix the label pos
    _massLabel = LabelTTF::create(getMassString().c_str(), "fonts/Marker Felt.ttf", 30);
    _massLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    _massLabel->setColor(Color3B::BLACK);
    _massLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _massLabel->setPosition(Vec2(-_crate->getContentSize().width/2, 0.0));
    _crate->addChild(_massLabel, LABEL_ZINDEX);

    this->addChild(_crate, SPRITE_ZINDEX);

    //////////////////////////////
    // 5. add the force arrows
    Vec2 curr(visibleSize.width/2, visibleSize.height/2);
    ValueArrow::setBaseValue(SpriteLayer::MAX_FORCE);
    auto addArrow = [&](const Color4B & color, const std::string & name) -> ValueArrow *
    {
        auto arrow = ValueArrow::create(color, name);
        arrow->setPosition(curr);
        curr = curr + Vec2(0, arrow->getContentSize().height);
        this->addChild(arrow, LABEL_ZINDEX);
        return arrow;
    };

    _forceExternal = addArrow(Color4B::GREEN, "Applied");
    _forceFriction = addArrow(Color4B::RED, "Friction");
    _sumOfForces = addArrow(Color4B::BLUE, "Net");


    //////////////////////////////
    // 6. add the speed related display
    auto speedLayer = Layer::create();

    // label
    _speedLabel = LabelTTF::create(getSpeedString().c_str(), "fonts/Marker Felt.ttf", 30);
    _speedLabel->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _speedLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    _speedLabel->setColor(Color3B::BLACK);
    speedLayer->addChild(_speedLabel);

    // speed-o-meter
    // dial
    auto dial = Sprite::create("dial.png");
    dial->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    dial->setPosition(Vec2(-10.0, 40.0));
    dial->setScale(0.8);
    speedLayer->addChild(dial);
    // needle
    _needle = Sprite::create("needle.png");
    _needle->setAnchorPoint(Vec2(0.85, 0.5));
    _needle->setPosition(Vec2(-dial->getContentSize().width/2 + 10.0, dial->getContentSize().height/2 + 17.0));
    _needle->setScale(0.8);
    _needle->setRotation(OFFSET_ANGLE);
    speedLayer->addChild(_needle, 1);

    speedLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    speedLayer->setPosition(Vec2(visibleSize.width, visibleSize.height/3 + 10));
    addChild(speedLayer, LABEL_ZINDEX);

    //////////////////////////////
    // 7. setup
    this->addPersonOfForce();
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

void SpriteLayer::addPersonOfForce()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float force = _forceExternalValue;

    auto beforeCrate = Vec2(visibleSize.width/2 + origin.x - _crate->getContentSize().width/2,
                    visibleSize.height/3 + 15 +origin.y);
    auto afterCrate = Vec2(visibleSize.width/2 + origin.x + _crate->getContentSize().width/2,
                    visibleSize.height/3 + 15 +origin.y);

    auto prevPosition = beforeCrate;
    auto prevAnchorPoint = Vec2::ANCHOR_BOTTOM_RIGHT;
    auto prevFlippedX = false;
    if (_personLayer)
    {
        prevPosition = _personLayer->getPosition();
        prevAnchorPoint = _personLayer->getAnchorPoint();
        prevFlippedX = _person->isFlippedX();
        removeFromMovables(_personLayer);
        removeChild(_personLayer);
    }

    std::stringstream sstr;
    if (force == 0)
    {
        sstr << (_personFell ? "pusher_fall_down.png" : "pusher_straight_on.png");
    }
    else
    {
        sstr << "pusher_" << getIndexFromForce(fabs(_showAnotherPerson ? force/2 : force)) << ".png";
    }

    _personLayer = Layer::create();
    _personLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

    _person = Sprite::create(sstr.str());
    _personLayer->addChild(_person);
    _personLayer->setContentSize(_person->getContentSize());

    _anotherPerson = nullptr;
    if (_showAnotherPerson)
    {
        _anotherPerson = Sprite::create(sstr.str());
        _anotherPerson->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        _personLayer->setContentSize(_person->getContentSize() + _anotherPerson->getContentSize());
        _personLayer->addChild(_anotherPerson);
    }

    if (force == 0)
    {
        _personLayer->setPosition(prevPosition);
        _personLayer->setAnchorPoint(prevAnchorPoint);
        _person->setPosition(Vec2(0.0, 0.0));
        _person->setAnchorPoint(prevAnchorPoint);
        _person->setFlippedX(!prevFlippedX);
        if (_anotherPerson)
        {
            _anotherPerson->setAnchorPoint(prevAnchorPoint);
            auto direction = prevAnchorPoint == Vec2::ANCHOR_BOTTOM_RIGHT ? -1 : 1;
            _anotherPerson->setPosition(Vec2(direction * ( _person->getContentSize().width + 10 ), 0.0));
            _anotherPerson->setFlippedX(!prevFlippedX);
        }
    }
    else
    {
        _personLayer->setAnchorPoint( force > 0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT );
        _personLayer->setPosition( force > 0 ? beforeCrate : afterCrate);

        _person->setAnchorPoint( force > 0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT );
        _person->setPosition(Vec2::ZERO);

        if (_anotherPerson)
        {
            _anotherPerson->setAnchorPoint( force > 0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT );
            auto direction = force > 0 ? -1 : 1;
            _anotherPerson->setPosition(Vec2( direction * 10, 0.0));
        }
    }

    if (force < 0)
    {
        _person->setFlippedX(true);
        if (_showAnotherPerson)
            _anotherPerson->setFlippedX(true);
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
    {
        _forceExternalValue = value;
        if (_forceExternalValue > 0.0)
           _personFell = false; 
        addPersonOfForce();
    }

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

    if (fabs(_velocity) > MAX_SPEED)
    {
        _personFell = true;
        
        // this will in turn invoke changeForceValue() fn
        _menuLayer->setForceSliderValue(0.0);
        NativeHelper::vibrate(500);
        addPersonOfForce();
    }
    readjustForces();

    float acc = _prevSumOfForcesValue / _mass;
    float dv = acc * dt;
    _velocity += dv;
    float dx = 0.0;
    if(fabs(_velocity) > EPSILON)
        dx = _velocity * dt * PTM_RATIO;
    _speedLabel->setString(getSpeedString());

    for( auto mov: _movables )
    {
        auto node = mov.getNode();
        node->runAction(Place::create(node->getPosition() + Vec2( (-dx + mov.getVelocity() * dt * PTM_RATIO) * mov.getScale(), 0.0)));
    }

    _prevSumOfForcesValue = _sumOfForcesValue;

    // speed-o-meter implementation
    auto angle = ( ( fabs(_velocity) / MAX_SPEED ) * MAX_ANGLE ) + OFFSET_ANGLE;
    _needle->setRotation(angle);

    Node::update(dt);
}

Node * SpriteLayer::addMovingChild(std::function< Node * () > generator, float velocity, int zindex, Vec2 pos, bool baseMover)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto create = [=] (bool mini) ->  Node *
    {
        auto scale = mini ? MINI_MAP_SCALE : 1.0;
        Node * node = generator();
        node->setScale( node->getScale() * scale);

        node->setPosition(Vec2((pos.x*scale) + visibleSize.width/2, mini ? 0.0 : pos.y)); 
        if (!baseMover)
            addToMovables(node, velocity, scale);
        auto parent = mini ? _minimap : this;
        parent->addChild(node, zindex);
        return node;
    };

    create(true);
    return create(false); 
}

Node * SpriteLayer::addStationaryChild(std::function< Node * () > generator, Vec2 pos)
{
    return addMovingChild(generator, 0.0, STATIONARY_ZINDEX, pos, false);
}

void SpriteLayer::addToMovables( Node * node, float vel, float scale )
{
    _movables.push_back(Movable(vel, scale, node));
}

void SpriteLayer::removeFromMovables( Node * node )
{
    auto f = std::find(_movables.begin(), _movables.end(), Movable(0.0, 0.0, node)); 
    if (f != _movables.end())
        _movables.erase(f); 
}

void SpriteLayer::addAnotherPerson()
{
    _showAnotherPerson = true;
    addPersonOfForce();
}

SpriteLayer::~SpriteLayer()
{
    unscheduleUpdate();
}
