#ifndef __START_SCENE_H__
#define __START_SCENE_H__

#include "cocos2d.h"

class StartLayer : public cocos2d::CCLayer
{
public:

    virtual bool init();

    static cocos2d::CCScene* scene();

    CREATE_FUNC(StartLayer);



    void startDone();
    void optionGo();

};


#endif // __START_SCENE_H__
