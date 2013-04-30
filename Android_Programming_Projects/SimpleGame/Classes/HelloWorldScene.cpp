#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#include "StartScreen.h"
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace cocos2d;

//increasing this variable makes changes in X more dramatic in the ball as it bounces off the paddle
const float DX_CONSTANT = 2.0;

//Label strings
char str_s[] = "";
char message[] = "";

HelloWorld::~HelloWorld()
{
	//Free memory
	if (_bricks)
	{
		_bricks->release();
		_bricks = NULL;
	}

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}


HelloWorld::HelloWorld()
{	
	
    //Get window size	
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    won = false;

    //Set the background color
    CCLayerColor::create( ccc4(252,180,213,255) );

    //Create paddle sprite and place it centered on screen
    paddle = CCSprite::create("Paddle.png");
    paddle->setPosition(ccp(winSize.width/2, 50));
    this->addChild(paddle);

    //create ball sprite and place it above paddle in scene
    ball = CCSprite::create("kirby.png");
    ball->setPosition(ccp(winSize.width/2, (winSize.height/2)-ball->getContentSize().height));
    this->addChild(ball);

    //Set movement deltas of ball
    delta_x = 0;
    delta_y = -3;

    score = 0;

    //Allow touch input
    this->setTouchEnabled(true);

    //Initialize brick array
    _bricks = new CCArray;

    //Add bricks to break
    for(int i = 0; i < 5; i++) {
 
        static int padding=5;
     
        // Create blocks and add them to the layer
        CCSprite *block = CCSprite::create("brick.jpg");
        
        //Shrink the size of the image
        block->setScaleY(25.0/block->getContentSize().height);
        
        int xOffset = ((winSize.width/5)*i)+padding+(block->getContentSize().width/2);
        
        block->setPosition(ccp(xOffset, winSize.height - (winSize.height/7)));
        block->setTag(2);
        //Add brick to scene layer
        this->addChild(block);
        //Add brick to array
        _bricks->addObject(block);

        // Create blocks and add them to the layer
        block = CCSprite::create("brick.jpg");
        //Shrink the size of the image
        block->setScaleY(25.0/block->getContentSize().height);

        xOffset = ((winSize.width/5)*i)+padding+(block->getContentSize().width/2);
       
        block->setPosition(ccp(xOffset, winSize.height - ((winSize.height/7)*2)));
        block->setTag(2);
        
        //Add brick to scene layer
        this->addChild(block);
        //Add brick to array
        _bricks->addObject(block);
    }


		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		//CC_BREAK_IF(! pCloseItem);
        
		// Place the menu item bottom-right conner.
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		//CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);


}


CCScene* HelloWorld::scene()
{

	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		HelloWorld *layer = HelloWorld::create();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}


//Function to move paddle according to user touch
void HelloWorld::ccTouchesBegan(CCSet* touches, CCEvent* event)
{

    // Choose one of the touches to work with
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
    //Get coordinates of user touch
	CCPoint location = touch->getLocation();

	//Get current position of paddle before movement occurs
	float tempX = paddle->getPosition().x;
	float tempY = paddle->getPosition().y;
    
	//CCLog("BEN after  x:%f, y:%f", location.x, location.y);

    //Move paddle to user touch, only change X position
    paddle->setPosition( ccp(location.x, 50));

    paddle_dx = (location.x - tempX) * DX_CONSTANT;
}



void HelloWorld::resetGameState()
{
	
	won = false;

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	//Remake bricks
    CCSprite* temp_brick;

	//First remove all the old bricks
	for (int j = 0; j < _bricks->count(); j++)
	{
		temp_brick = dynamic_cast<CCSprite*>(_bricks->objectAtIndex(j));

		_bricks->removeObject(temp_brick);
		this->removeChild(temp_brick, true);
	}

    //Add bricks to break
    for(int i = 0; i < 5; i++) {
 
        static int padding=5;
     
        // Create blocks and add them to the layer
        CCSprite *block = CCSprite::create("brick.jpg");
        
        //Shrink the size of the image
        block->setScaleY(25.0/block->getContentSize().height);
        
        int xOffset = ((visibleSize.width/5)*i)+padding+(block->getContentSize().width/2);
        
        block->setPosition(ccp(xOffset, visibleSize.height - (visibleSize.height/7)));
        block->setTag(2);
        //Add brick to scene layer
        this->addChild(block);
        //Add brick to array
        _bricks->addObject(block);

        // Create blocks and add them to the layer
        block = CCSprite::create("brick.jpg");
        //Shrink the size of the image
        block->setScaleY(25.0/block->getContentSize().height);

        xOffset = ((visibleSize.width/5)*i)+padding+(block->getContentSize().width/2);
       
        block->setPosition(ccp(xOffset, visibleSize.height - ((visibleSize.height/7)*2)));
        block->setTag(2);
        
        //Add brick to scene layer
        this->addChild(block);
    }

    //Reset postions of paddle and ball
    paddle->setPosition(ccp(visibleSize.width/2, 50));
    ball->setPosition(ccp(visibleSize.width/2, (visibleSize.height/2)-ball->getContentSize().height));


    //reset game variables
    delta_x = 0;
    delta_y = -3;

    score = 0;

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
        
		// Place the menu item bottom-right conner.
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);
	
	
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CCDirector::sharedDirector()->replaceScene( HelloWorld::scene() );
	//CCDirector::sharedDirector()->popScene();

}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	bool bRet = false;

	do 
	{
		
		// super init first
		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(252,180,213,255) ) );

		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);
        
		// Place the menu item bottom-right conner.
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
		pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                    origin.y + pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

        
        //Set up score
        sprintf(str_s, "Score: %d", score);
        Scorelabel = CCLabelTTF::create(str_s, "Arial", 15);
    	Scorelabel->setPosition( ccp(30, 30) );
    	Scorelabel->setColor(ccc3(255,255,255));
    	this->addChild(Scorelabel, 1);

		this->setTouchEnabled(true);

		//Setup end game message
		win_or_lose = CCLabelTTF::create(message, "Arial", 30);
		win_or_lose->setColor(ccc3(207,181,59));
		win_or_lose->setPosition(ccp(visibleSize.width/2.0, visibleSize.height/2.0));

		this->addChild(win_or_lose, 1);

		//place reset button on screen
		CCSprite *reset= CCSprite::create("reset.png");
		//reset->setPosition(ccp(visibleSize.width/2.0 + 50, visibleSize.height/2.0 + 50));
		CCMenuItemSprite *rItem = CCMenuItemSprite::create(reset, reset, this, menu_selector(HelloWorld::resetGameState));

		resetMenu = CCMenu::create(rItem, NULL);

		resetMenu->setPosition(ccp(visibleSize.width/2.0, visibleSize.height/2.0 + 60));

		this->addChild(resetMenu, 1);

		resetMenu->setEnabled(false);
		resetMenu->setOpacity(0);

	
		this->schedule( schedule_selector(HelloWorld::updateGame) );

		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;
	} while (0);

	return bRet;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

//Move ball around and check for collisions with bricks and paddles
void HelloWorld::updateGame(float delta_time)
{

	CCPoint Pos = ball->getPosition();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	

	//Check collision with paddle
	CCRect paddle_rect = CCRectMake(paddle->getPosition().x - (paddle->getContentSize().width/2),
									paddle->getPosition().y - (paddle->getContentSize().height/2),
									paddle->getContentSize().width,
									paddle->getContentSize().height);

	CCRect ball_rect = CCRectMake(ball->getPosition().x - (ball->getContentSize().width/2),
									ball->getPosition().y - (ball->getContentSize().height/2),
									ball->getContentSize().width,
									ball->getContentSize().height);

	//Check if the player lost
	if (ball->getPosition().y <= 0.0 && won == false)
	{
		strcpy(message, "You Lost :(");
		win_or_lose->setString(message);
		resetMenu->setEnabled(true);
		resetMenu->setOpacity(255);
	}
	else if (paddle_rect.intersectsRect(ball_rect))
	{
		//Flip Y direction of movement
		delta_y *= -1;

		delta_x = -paddle_dx;

		//Apply delta_x and delta_y to ball
		Pos.y -= (delta_y);

		Pos.x -= (delta_x * delta_time);
		
		//Play sound effect
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("spring.wav");
		
		ball->setPosition( Pos );

		return;
	}
	
	//Check collisions with the bricks
	CCRect brick_bounding_box;
	CCSprite* temp_brick;

	for (int x = 0; x < _bricks->count(); x++)
	{
		temp_brick = dynamic_cast<CCSprite*>(_bricks->objectAtIndex(x));

		brick_bounding_box = CCRectMake(temp_brick->getPosition().x - (temp_brick->getContentSize().width/2),
									temp_brick->getPosition().y - (temp_brick->getContentSize().height/2),
									temp_brick->getContentSize().width,
									temp_brick->getContentSize().height);

		//Flip Y direction if you hit a brick
		if (ball_rect.intersectsRect(brick_bounding_box))
		{
			delta_y *= -1;
			Pos.y -= (delta_y);

			ball->setPosition( Pos );

			//Remove brick that was hit from scene
			_bricks->removeObject(temp_brick);

			this->removeChild(temp_brick, true);

			//Play sound effect
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("photon.wav");

			score++;

			sprintf(str_s, "Score: %d", score);
			Scorelabel->setString(str_s);

			//Check if the player lost
			if (_bricks->count() == 0)
			{
				strcpy(message, "You Win :)");
				win_or_lose->setString(message);
				resetMenu->setEnabled(true);
				resetMenu->setOpacity(255);

			}

			return;
		}

		

	}


	//Apply delta_x and delta_y to ball
	Pos.y -= (delta_y);
	Pos.x -= (delta_x * delta_time);

	//Flip Y direction when we hit top of screen
	if ((Pos.y + (ball->getContentSize().height/2)) >= winSize.height)
	{
		delta_y *= -1;
		Pos.y -= (delta_y);
	}

	//Flip X direction if there is a collision with one of the sides of the screen
	if( ((Pos.x + (ball->getContentSize().width/2)) >= winSize.width) || ((Pos.x - (ball->getContentSize().width/2)) <= 0.0))
	{
		delta_x *= -1;
		Pos.x -= (delta_x * delta_time);
	}

	
	ball->setPosition( Pos );
	

}



void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
