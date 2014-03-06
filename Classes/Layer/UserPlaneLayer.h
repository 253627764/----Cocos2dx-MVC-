#ifndef __UserPlaneLayer__h__
#define __UserPlaneLayer__h__

#include "System/GameDefine.h"

class UserPlane;

class UserPlaneLayer: public cocos2d::CCLayer
{
public:
	virtual bool init();   
	//初始化用户飞机
	void initUserPlane();
	//播放出场动画
	void playUserPlaneAnimate();
	//激活用户飞机控制
	void enableControlUserPlane();
public:
	//继承
	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	CREATE_FUNC(UserPlaneLayer)

private:
	//用户飞机死亡回调函数
	void userPlaneDeadCallBack(CCObject *obj);
	
private:
	CCPoint	m_userPlaneTouchOffset;    //用户飞机与触摸点的偏移距离
};


#endif