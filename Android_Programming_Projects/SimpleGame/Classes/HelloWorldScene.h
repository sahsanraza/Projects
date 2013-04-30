#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayerColor
{
	public:
		HelloWorld();
		~HelloWorld();


		virtual bool init();  

		static cocos2d::CCScene* scene();

		virtual void menuCloseCallback(cocos2d::CCObject* pSender);

		CREATE_FUNC(HelloWorld);

		void spriteMoveFinished(cocos2d::CCNode* sender);

		void updateGame(float dt);

		void registerWithTouchDispatcher();

		//Function for moving paddle
		void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

		void resetGameState();
	    
	    
	protected:
		//Array of brick objects
		cocos2d::CCArray *_bricks;
		
		int _projectilesDestroyed;

		//Paddle sprite
		cocos2d::CCSprite *paddle; 

		//Ball sprite
		cocos2d::CCSprite *ball;

		//Changes from last update in paddle must be reflected when ball hits paddle
		float delta_x;
		float delta_y;

		//win or lose message
		cocos2d::CCLabelTTF *win_or_lose;

		cocos2d::CCMenu* pMenu;

		//Score
		cocos2d::CCLabelTTF* Scorelabel;

		int score;

		bool won;

		cocos2d::CCMenu* resetMenu;

		//Changes in movement on X axis of paddle
		float paddle_dx;


};

#endif  // __HELLOWORLD_SCENE_H__
