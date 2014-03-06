#ifndef __GameWorld_H__
#define __GameWorld_H__

#include "System/GameDefine.h"

class GameWorldLayer: public cocos2d::CCLayer
{
public:
	static GameWorldLayer* shareGameWorldLayer();
	static cocos2d::CCScene* scene();
	//继承
    virtual bool init();
private:
	static GameWorldLayer* _gameWorldLayer;
};

#endif // __HELLOWORLD_SCENE_H__
