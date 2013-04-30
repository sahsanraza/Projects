#include "OptionScene.h"
#include "StartScreen.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;


bool OptionLayer::init()
{
     if ( !CCLayer::init() )
    {
        return false;
    }

    // add options splash screen
    CCSprite* pSprite = CCSprite::create("options_screen.jpg");

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    pSprite->setScaleY(visibleSize.height/pSprite->getContentSize().height);
    // add the sprite as a child to this layer
    this->addChild(pSprite);

    //create option button
    CCSprite *on= CCSprite::create("soundOn.png");

    //create start button
    CCSprite *off = CCSprite::create("soundOff.png");

    //create the two items
    CCMenuItemSprite *OnItem = CCMenuItemSprite::create(on, on, this, menu_selector(OptionLayer::turnSoundOn));
    if(!OnItem) return false;
    CCMenuItemSprite *OffItem = CCMenuItemSprite::create(off, off, this, menu_selector(OptionLayer::turnSoundOff));
    if(!OffItem) return false;


    //create menu
    CCMenu* pMenu = CCMenu::create(OnItem, NULL);
    if(!pMenu) return false;

    CCMenu* pMenu2 = CCMenu::create(OffItem, NULL);
    if(!pMenu2) return false;

    //setup positioning
    pMenu->setPosition(ccp(visibleSize.width/5.0, visibleSize.height/6.0));
    //OptionItem->setPosition(ccp(visibleSize.width/4.0, visibleSize.height/6.0));
    pMenu2->setPosition(ccp(visibleSize.width/2.0, visibleSize.height/6.0));
    //StartItem->setPosition(ccp(visibleSize.width/3.0, visibleSize.height/4.0));

    //pMenu->alignItemsHorizontallyWithPadding(300.0f);

    //add menu item as layer
    this->addChild(pMenu, 1);
    this->addChild(pMenu2, 1);

    volume1 = CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();
    volume2 = CocosDenshion::SimpleAudioEngine::sharedEngine()->getEffectsVolume();

    // Create a back button
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("back-button.png", "back-button.png", this, menu_selector(OptionLayer::BacktoMain));
        
    // Place the menu item bottom-right conner.
    pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                origin.y + pCloseItem->getContentSize().height/2));

    // Create a menu with the "close" menu item, it's an auto release object.
    CCMenu* pMenu3 = CCMenu::create(pCloseItem, NULL);
    pMenu3->setPosition(CCPointZero);

    // Add the menu to Option layer as a child layer.
    this->addChild(pMenu3, 1);

    return true;

}

void OptionLayer::turnSoundOff()
{
    isSoundOn = false;

    volume1 = CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);

    volume2 = CocosDenshion::SimpleAudioEngine::sharedEngine()->getEffectsVolume();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
}

void OptionLayer::turnSoundOn()
{
    isSoundOn = true;

    CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(volume2);

    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(volume2);


}

void OptionLayer::BacktoMain()
{
    CCDirector::sharedDirector()->replaceScene( StartLayer::scene() );
}


CCScene* OptionLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    OptionLayer *layer = OptionLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}