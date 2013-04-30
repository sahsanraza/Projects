#include "StartScreen.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "OptionScene.h"

using namespace cocos2d;


bool StartLayer::init()
{
     if ( !CCLayer::init() )
    {
        return false;
    }

    // add "Start" splash screen"
    CCSprite* pSprite = CCSprite::create("startscreen.png");

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    pSprite->setScaleY(visibleSize.height/pSprite->getContentSize().height);
    // add the sprite as a child to this layer
    this->addChild(pSprite);

    //create option button
    CCSprite *option = CCSprite::create("options.png");
    option->setScaleX(visibleSize.width/(option->getContentSize().width*4));
    option->setScaleY(visibleSize.height/(option->getContentSize().height*4));

    //create start button
    CCSprite *start = CCSprite::create("start.png");
    start->setScaleX(visibleSize.width/(4*start->getContentSize().width));
    start->setScaleY(visibleSize.height/(4*start->getContentSize().height));

    //create the two items
    CCMenuItemSprite *OptionItem = CCMenuItemSprite::create(option, option, this, menu_selector(StartLayer::optionGo));
    if(!OptionItem) return false;
    CCMenuItemSprite *StartItem = CCMenuItemSprite::create(start, start, this, menu_selector(StartLayer::startDone));
    if(!StartItem) return false;

    
    //create menu
    CCMenu* pMenu = CCMenu::create(OptionItem, NULL);
    if(!pMenu) return false;

    CCMenu* pMenu2 = CCMenu::create(StartItem, NULL);
    if(!pMenu2) return false;

    //setup positioning
    pMenu->setPosition(ccp(visibleSize.width/5.0, visibleSize.height/6.0));
    //OptionItem->setPosition(ccp(visibleSize.width/4.0, visibleSize.height/6.0));
    pMenu2->setPosition(ccp(visibleSize.width/2.0, visibleSize.height/6.0));
    StartItem->setPosition(ccp(visibleSize.width/3.0, visibleSize.height/4.0));

    //pMenu->alignItemsHorizontallyWithPadding(300.0f);

    //add menu item as layer
    this->addChild(pMenu, 1);
    this->addChild(pMenu2, 1);

    //Play sound effect
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("bugle.wav");

    

    //Set the background color
    //CCLayerColor::create( ccc4(252,180,213,255) );

    return true;

}

void StartLayer::startDone()
{
    CCDirector::sharedDirector()->replaceScene( HelloWorld::scene() );
}

void StartLayer::optionGo()
{
    CCDirector::sharedDirector()->replaceScene( OptionLayer::scene() );
}


CCScene* StartLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    StartLayer *layer = StartLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

