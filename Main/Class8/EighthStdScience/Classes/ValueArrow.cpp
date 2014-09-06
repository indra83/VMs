#include "ValueArrow.h"

USING_NS_CC;

float ValueArrow::BASE_VALUE=0.0;
float ValueArrow::HEIGHT=50.0;
float ValueArrow::WIDTH=100.0;

ValueArrow * ValueArrow::create(const Color4B &color, const std::string &name)
{
    ValueArrow *layer = new ValueArrow();
    if (layer && layer->init(color, name))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool ValueArrow::init(const Color4B &color, const std::string & name)
{
    if ( !Layer::init() )
        return false;
    _labelName = LabelTTF::create(name, "fonts/Maven Pro Black.otf", 30);
    _labelName->setHorizontalAlignment(TextHAlignment::LEFT);
    _labelName->setColor(Color3B::BLACK);
    _labelName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    addChild(_labelName, 1);

    _labelForce = LabelTTF::create("", "fonts/Maven Pro Black.otf", 30);
    _labelForce->setHorizontalAlignment(TextHAlignment::LEFT);
    _labelForce->setColor(Color3B::BLACK);
    _labelForce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    addChild(_labelForce, 1);

    _color=color;
    _shaft = LayerColor::create(_color, WIDTH, HEIGHT);
    addChild(_shaft);

    adjustSize();
    return true;
}

void ValueArrow::adjustSize()
{
    auto val = fabs(_val);
    float scale = val/ValueArrow::BASE_VALUE;
    float scaleHead = scale > 1.0 ? 1.0 : scale;

    std::stringstream sstr;
    sstr << (int)val << " N";
    _labelForce->setString(sstr.str());

    // scalings and positionings
    auto anchor = _val < 0.0 ? Vec2::ANCHOR_BOTTOM_RIGHT : Vec2::ANCHOR_BOTTOM_LEFT;
    setAnchorPoint(anchor);
    auto direction = _val < 0.0 ? -1 : 1;

    _shaft->setScaleX(scale);
    _shaft->setAnchorPoint(anchor);
    if (direction == -1)
        _shaft->setPosition(Vec2(-_shaft->getContentSize().width, 0.0));
    else
        _shaft->setPosition(Vec2(0.0, 0.0));

    if (_head)
        removeChild(_head);
    auto head = DrawNode::create();
    _head = head;
    _head->setAnchorPoint(anchor);
    head->drawTriangle(Vec2(0.0, HEIGHT), 
                        Vec2(0.0, -HEIGHT), 
                        Vec2(direction * HEIGHT, 0.0),
                        Color4F(_color));
    head->setContentSize(Size(HEIGHT, 2*HEIGHT));
    addChild(_head);

    if (direction == -1)
    {
        _head->setPosition(Vec2(-_shaft->getContentSize().width*scale + head->getContentSize().width*scaleHead, HEIGHT/2)); 
    }
    else
    {
        _head->setPosition(_shaft->getPosition() + Vec2(_shaft->getContentSize().width*scale, HEIGHT/2)); 
    }
    _head->setScaleX(scaleHead);


    if (direction == -1)
    {
        _labelName->setPosition(_head->getPosition() + Vec2( -2 * head->getContentSize().width * scaleHead, 0.0 ));
    }
    else
    {
        _labelName->setPosition(_head->getPosition() + Vec2(head->getContentSize().width * scaleHead, 0.0 ));
    }
    _labelName->setAnchorPoint(anchor);

    if( _labelForce->getContentSize().width > _shaft->getContentSize().width * scale )
    {
        _labelForce->setPosition(_labelName->getPosition() + Vec2(direction * (_labelName->getContentSize().width + 10.0), 0.0 ) );
        _labelForce->setAnchorPoint(anchor);
    }
    else
    {
        _labelForce->setPosition(Vec2(direction * _shaft->getContentSize().width * scale/2 , 0.0));
        _labelForce->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    }

    setContentSize(_head->getContentSize());
}



