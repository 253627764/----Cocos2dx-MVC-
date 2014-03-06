#ifndef __EnemyPlaneLayer__h__
#define __EnemyPlaneLayer__h__

#include "System/GameDefine.h"
#include "Base/GameObject.h"

class EnemyPlane;
class EnemyPlaneLayer: public cocos2d::CCLayer
{
public:
	virtual bool init();
public:
	~EnemyPlaneLayer();
	static EnemyPlane* _enemyPlane;                            //调试变量
	CREATE_FUNC(EnemyPlaneLayer)
	//回收敌机
	void retrieveEnemyPlane(CCNode* pSender,void* data);
private:
	CCArray m_enemyPlanesArray;                                //敌机数组
private:
	//开始加载敌机
	void loadEnemys();
	//延时调用loadEnemys
	void delayTimerCallLoadEnemys(float dt);

	//用户飞机准备好回调函数
	void userPlaneReadyCallBack(CCObject *obj);
	//敌机血量变化回调函数
	void enemyPlaneDeadCallBack(CCObject *obj);

};


#endif