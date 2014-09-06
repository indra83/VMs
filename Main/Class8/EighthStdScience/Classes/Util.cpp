#include "Util.h"

USING_NS_CC;
// copied from cocos2d::utils captureScreen() code
void onCaptureScreen(std::function< void (Sprite *) > cb)
{
    auto glView = Director::getInstance()->getOpenGLView();
    auto frameSize = glView->getFrameSize();
    
    int width = static_cast<int>(frameSize.width);
    int height = static_cast<int>(frameSize.height);
    
    Sprite * sprite = nullptr; 
    
    do
    {
        std::shared_ptr<GLubyte> buffer(new GLubyte[width * height * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
        if (!buffer)
            break;
        
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
        
        std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[width * height * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
        if (!flippedBuffer)
            break;

        for (int row = 0; row < height; ++row)
            memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);

        std::shared_ptr<Image> image(new Image);
        image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8);
        auto texture = new Texture2D();
        texture->autorelease();
        texture->initWithImage(&(*image));
        sprite = Sprite::createWithTexture(texture); 
    } while(0);

    cb(sprite);
}

void captureScreenAsSprite(std::function< void (Sprite *) > cb)
{
    static CustomCommand captureScreenCommand;
    captureScreenCommand.init(std::numeric_limits<float>::max());
    captureScreenCommand.func = std::bind(onCaptureScreen, cb);
    Director::getInstance()->getRenderer()->addCommand(&captureScreenCommand);
}


