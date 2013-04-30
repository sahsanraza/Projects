#ifndef __OPTION_SCENE_H__
#define __OPTION_SCENE_H__

#include "cocos2d.h"

class OptionLayer : public cocos2d::CCLayer
{
public:

    virtual bool init();

    static cocos2d::CCScene* scene();

    CREATE_FUNC(OptionLayer);

    //function to go back to start screen
    void BacktoMain();

    void turnSoundOn();
    void turnSoundOff();

    bool isSoundOn;

    float volume1, volume2;

};

#endif // __OPTION_SCENE_H__