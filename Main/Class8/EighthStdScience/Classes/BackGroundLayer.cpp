#include "BackGroundLayer.h"

#define BUF_HT 10
USING_NS_CC;

// TODO: 
// 2) add randomness
//  a) hills are overlapping
//  b) clouds and trees need not be continuous

static const Color4B BLUEISH(173, 255, 250, 255);
static const Color4B GRAYISH (70, 70, 70, 255);

// due to lack of access to the Point2Object class in ParallaxNode, replicate it here
class Point2Object : public Ref
{
public:
    inline void setRatio(Vec2 ratio) {_ratio = ratio;}
    inline void setOffset(Vec2 offset) {_offset = offset;}
    inline void setChild(Node *var) {_child = var;}
    inline Vec2 getOffset() const {return _offset;}
    inline Node* getChild() const {return _child;}
private:
    Vec2 _ratio;
    Vec2 _offset;
    Node* _child;
};

// N.B. assumes anchor points are in the left corner
class HorInfiniteParallaxNode : public ParallaxNode
{
public:
    static HorInfiniteParallaxNode* create()
    {
        // Create an instance of InfiniteParallaxNode
        HorInfiniteParallaxNode* node = new HorInfiniteParallaxNode();
        if (node) 
        {
            // Add it to autorelease pool
            node->autorelease();
        }
        else 
        {
            // Otherwise delete
            delete node;
            node = nullptr;
        }
        return node;
    }

    float getScaledSizeX(Node * n)
    {
        return n->getContentSize().width * n->getScaleX();
    }

    void addChild(std::function<Node * ()> cb, int zIndex, const Vec2& ratio, const Vec2& offset)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        float totalSize = 0;
        Vec2 offsetIter = offset;
        while (totalSize < visibleSize.width*2)
        {
            auto child = cb();
            ParallaxNode::addChild(child, zIndex, ratio, offsetIter);
            totalSize += getScaledSizeX( child );
            offsetIter += Vec2( getScaledSizeX(child), 0 );
        }
    }

    void updatePosition()
    {
        int safeOffset = 0;
        // Get visible size
        Size visibleSize = Director::getInstance()->getVisibleSize();
        // 1. For each child of an parallax node
        for (int i = 0; i < _children.size(); i++)
        {
            auto node = _children.at(i);

            float whichSide = 0;
            auto pointInWorld = this->absolutePosition() + node->getPosition();

            // 2. We check whether it is out of the left side of the visible area
            // N.B. assumes anchor points are in the left corner
            if (pointInWorld.x + getScaledSizeX(node) < safeOffset)
                whichSide = 1;
            else if (pointInWorld.x > visibleSize.width - safeOffset)
                whichSide = -1;

            if (whichSide != 0)
            {
                // 3. Find Point2Object that corresponds to current node
                for (int i = 0; i < _parallaxArray->num; i++)
                {
                    auto po = (Point2Object*)_parallaxArray->arr[i];
                    // If yes increase its current offset on the value of visible width
                    if (po->getChild() == node)
                        po->setOffset(po->getOffset() +
                                Vec2(visibleSize.width + getScaledSizeX(node), 0)*whichSide);
                }
            }
        }
    }

    void visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags) override
    {
        updatePosition();
        ParallaxNode::visit(renderer, parentTransform, parentFlags);
    }

};

// on "init" you need to initialize your instance
bool BackGroundLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    int zIndex = 0;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _lower_boundary = visibleSize.height/3;

    auto parallaxNode = HorInfiniteParallaxNode::create();

    //////////////////////////////
    // add blue back layer
    parallaxNode->addChild(
            [&] () -> Node * { 
                return LayerColor::create(BLUEISH,
                    visibleSize.width,
                    visibleSize.height);
            },
            zIndex,
            Vec2(1.0, 1.0),
            Vec2(0 , 0)
    );

    //////////////////////////////
    //add gray ground layer
    parallaxNode->addChild(
            [&]() -> Node * {
                auto layer = LayerColor::create(GRAYISH, visibleSize.width, _lower_boundary);
                layer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                return layer;
            },
            ++zIndex,
            Vec2(1.0, 1.0),
            Vec2(0, 0)
    );

    //////////////////////////////
    // add clouds
    parallaxNode->addChild(
            [&] () -> Node * {
                auto clouds = Sprite::create("cloud1.png");
                clouds->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                clouds->setScale(0.5);
                return clouds; 
            },
            ++zIndex,
            Vec2(0.025, 1.0),
            Vec2(0, 9*visibleSize.height/10)
    );

    //////////////////////////////
    // add mountains
    parallaxNode->addChild(
            [] () -> Node * {
                auto mountains = Sprite::create("mountains.png");
                mountains->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                mountains->setScale(1.3);
                return mountains;
            },
            ++zIndex,
            Vec2(0.05, 1.0),
            Vec2(0, _lower_boundary)
    );

    //////////////////////////////
    //add trees
    parallaxNode->addChild(
            [] () -> Node * {
                auto tree = Sprite::create("tree.png");
                tree->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
                tree->setScale(1.7);
                return tree;
            },
            ++zIndex,
            Vec2(0.75, 1.0),
            Vec2(0, _lower_boundary)
    );

    //////////////////////////////
    //add grass
    parallaxNode->addChild(
            [] () -> Node * {
                auto grass = Sprite::create("grass.png");
                grass->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                return grass;
            },
            ++zIndex,
            Vec2(1.0, 1.0),
            Vec2(0, _lower_boundary + 2*BUF_HT)
    );

    //////////////////////////////
    //add bricks
    parallaxNode->addChild(
            [] () -> Node * {
                auto bricks = Sprite::create("brick-tile.png");
                bricks->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
                bricks->setScale(0.3);
                return bricks;
            },
            ++zIndex,
            Vec2(1.0, 1.0),
            Vec2(0, _lower_boundary + BUF_HT)
    );

    addChild(parallaxNode, 0);
    return true;
}

void BackGroundLayer::setSurface(const std::string &name, float startPos, float endPos)
{
    auto offset = startPos;
    static const int SURF_TAG = 1;

    // drop existing surface
    // TODO: this is currently rudimentary
    for (auto child : getChildren())
    {
        if (child->getTag() == SURF_TAG)
        {
            float start = child->getPosition().x;
            float end = start + (child->getContentSize().width * child->getScaleX());
            if (start >= startPos && end <= endPos) 
                removeChild(child);
        }
    }

    do {
        if (offset >= endPos)
            break;
        auto surface = Sprite::create(name);
        surface->setTag(SURF_TAG);
        if ( (endPos - offset) < surface->getContentSize().width )
            surface->setScaleX( (endPos - offset) / surface->getContentSize().width );
        surface->setPosition(Vec2(offset, _lower_boundary + 2*BUF_HT));
        offset += surface->getContentSize().width * surface->getScaleX();
        surface->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        addChild(surface, 1);
    } while(true);
}
