#include "BackGroundLayer.h"

USING_NS_CC;

// TODO: 
// 2) add randomness
//  a) hills are overlapping
//  b) clouds and trees need not be continuous


// due to lack of access to the PointObject class in ParallaxNode, replicate it here
class PointObject : public Ref
{
public:
    inline void setRation(Point ratio) {_ratio = ratio;}
    inline void setOffset(Point offset) {_offset = offset;}
    inline void setChild(Node *var) {_child = var;}
    inline Point getOffset() const {return _offset;}
    inline Node* getChild() const {return _child;}
private:
    Point _ratio;
    Point _offset;
    Node* _child;
};

// N.B. assumes anchor points are in the left corner
class InfiniteParallaxNode : public ParallaxNode
{
public:
    static InfiniteParallaxNode* create()
    {
        // Create an instance of InfiniteParallaxNode
        InfiniteParallaxNode* node = new InfiniteParallaxNode();
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

    void addChild(std::function<Node * ()> cb, int zIndex, const Point& ratio, const Point& offset)
    {
        Size visibleSize = Director::getInstance()->getVisibleSize();
        float totalSize = 0;
        Point offsetIter = offset;
        while (totalSize < visibleSize.width*2)
        {
            auto child = cb();
            ParallaxNode::addChild(child, zIndex, ratio, offsetIter);
            totalSize += child->getContentSize().width * child->getScaleX();
            offsetIter += Point(child->getContentSize().width * child->getScaleX(), 0);
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
            auto pointInWorld = convertToWorldSpace(node->getPosition());

            // 2. We check whether it is out of the left side of the visible area
            if (pointInWorld.x + node->getContentSize().width*node->getScaleX() < safeOffset)
                whichSide = 1;
            else if (pointInWorld.x > visibleSize.width - safeOffset)
                whichSide = -1;

            if (whichSide != 0) 
            {
                // 3. Find PointObject that corresponds to current node
                for (int i = 0; i < _parallaxArray->num; i++)
                {
                    auto po = (PointObject*)_parallaxArray->arr[i];
                    // If yes increase its current offset on the value of visible width
                    if (po->getChild() == node)
                        po->setOffset(po->getOffset() +
                                Point(visibleSize.width + node->getContentSize().width*node->getScaleX(), 0)*whichSide);
                }
            }
        }
    }

    void visit(Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated)
    {
        updatePosition();
        ParallaxNode::visit(renderer, parentTransform, parentTransformUpdated);
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
    Point origin = Director::getInstance()->getVisibleOrigin();

    auto parallaxNode = InfiniteParallaxNode::create();

    //////////////////////////////
    // add blue back layer
    parallaxNode->addChild(
            [&] () -> Node * { 
                return LayerColor::create(Color4B(173, 255, 250, 255),
                		visibleSize.width,
                		visibleSize.height);
            },
            zIndex,
            Point(1.0, 1.0),
            Point(0 , 0)
    );   


    //////////////////////////////
    // add clouds
    parallaxNode->addChild(
            [&] () -> Node * {
                auto clouds = Sprite::create("cloud1.png");
                clouds->setAnchorPoint(Point(0.0, 1.0));
                clouds->setScale(0.5);
                return clouds; 
            },
            ++zIndex,
            Point(0.025, 1.0),
            Point(0, 9*visibleSize.height/10)
    );   

    //////////////////////////////
    // add mountains
    parallaxNode->addChild(
            [] () -> Node * {
                auto mountains = Sprite::create("mountains.png");
                mountains->setAnchorPoint(Point(0.0, 0.0));
                mountains->setScale(1.3);
                return mountains;
            },
            ++zIndex,
            Point(0.05, 1.0),
            Point(0, visibleSize.height/3)
    );   

    //////////////////////////////
    //add trees
    parallaxNode->addChild(
            [] () -> Node * {
                auto tree = Sprite::create("tree.png");
                tree->setAnchorPoint(Point(0.0, 0.0));
                tree->setScale(1.7);
                return tree;
            },
            ++zIndex,
            Point(0.75, 1.0),
            Point(0, visibleSize.height/3)
    );   

    //////////////////////////////
    //add grass
    parallaxNode->addChild(
            [] () -> Node * {
                auto grass = Sprite::create("grass.png");
                grass->setAnchorPoint(Point(0.0, 1.0));
                return grass;
            },
            ++zIndex,
            Point(1.0, 1.0),
            Point(0, visibleSize.height/3)
    );   

    //////////////////////////////
    //add gray ground layer
    parallaxNode->addChild(
            [&]() -> Node * { 
                return LayerColor::create(Color4B(70, 70, 70, 255), visibleSize.width, visibleSize.height/3);
            },
            zIndex,
            Point(1.0, 1.0),
            Point(0, 0)
    );   

    //////////////////////////////
    //add bricks
    parallaxNode->addChild(
            [] () -> Node * {
                auto bricks = Sprite::create("brick-tile.png");
                bricks->setAnchorPoint(Point(0.0, 1.0));
                bricks->setScale(0.3);
                return bricks;
            },
            ++zIndex,
            Point(1.0, 1.0),
            Point(0, visibleSize.height/3 + 15)
    );   

    //////////////////////////////
    //add physics ground to the background layer


    addChild(parallaxNode, 0);
    return true;
}
