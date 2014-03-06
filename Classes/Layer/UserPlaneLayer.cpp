#include "UserPlaneLayer.h"
#include "Plane/Plane.h"
#include "Layer/PartsLayer.h"

bool UserPlaneLayer::init()
{    
	if (!CCLayer::init())
		return false;
	initUserPlane();
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(UserPlaneLayer::userPlaneDeadCallBack),USERPLANE_DEAD_MESSAGE,NULL);
	return true;
}

void UserPlaneLayer::userPlaneDeadCallBack(CCObject *obj)
{
	UserPlane* userPlane = dynamic_cast<UserPlane*>(obj);
	if(userPlane){
		userPlane->removeFromParent();
		userPlane->retrieve();
	}
	EnemyPlane::stopEnemyPlanesEmit();
}

void UserPlaneLayer::initUserPlane()
{
	UserPlane* userPlane = UserPlane::shareUserPlane();
	//初始化
	CCDictionary* dict = CCDictionary::createWithContentsOfFile(USERPLANE_CONFIG_PLIST);
	userPlane->init(dict);
	//放置到屏幕中央
	float x = CCDirector::sharedDirector()->getVisibleSize().width / 2;
	float y = CCDirector::sharedDirector()->getVisibleSize().height / 2;
	userPlane->setPosition(ccp(x,y));

	//设置被碰撞粒子效果的文件名
	userPlane->setHitParticleName(NORMAL_HIT_PARTICLE_PLIST);
	//粒子效果的父层需要是非批渲染层
	userPlane->setBatchParticleNodesParent(this);

	//优化：将用户飞机层加入批渲染节点
	CCSpriteBatchNode* batchNode = AppConfig::shareAppConfig()->getGlobalBatchNode();
	userPlane->setBullteFlySpace(batchNode);
	batchNode->addChild(userPlane,USER_PLANE_ZORDER);
}

void UserPlaneLayer::playUserPlaneAnimate()
{
	UserPlane* userPlane = UserPlane::shareUserPlane();
	float dispScale = userPlane->getDispScale();
	userPlane->setScale(userPlane->getDispScale() * USERPLANE_MAX_SCALE);
	CCScaleTo* scaleAction = CCScaleTo::create(3.0f,dispScale,dispScale);
	CCCallFunc* callAction = CCCallFunc::create(this,callfunc_selector(UserPlaneLayer::enableControlUserPlane));
	CCSequence* seqAction = CCSequence::create(scaleAction,callAction,NULL); 
	userPlane->runAction(seqAction);
}

void UserPlaneLayer::onEnter()
{
	CCLayer::onEnter();
	playUserPlaneAnimate();
}

void UserPlaneLayer::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void UserPlaneLayer::enableControlUserPlane()
{
	UserPlane* userPlane = UserPlane::shareUserPlane();
	
	/*
	1优化：将批粒子渲染节点的父设置为当前层，否则与用户飞机使用的批渲染节点产生冲突
	2播放完成入场动画后加入粒子效果
	*/
	userPlane->setBatchParticleNodesParent(this);
	userPlane->setStaticParticleVisible(true);
	userPlane->startEmitBulltes();
	CCLog("x:%f,y:%f",userPlane->boundingBox().size.width,userPlane->boundingBox().size.height);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(USERPLANE_READY_MSG, NULL);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,true);
}
bool UserPlaneLayer::ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent)
{
	UserPlane* userPlane = UserPlane::shareUserPlane();
	m_userPlaneTouchOffset = ccpSub(userPlane->getPosition(),convertTouchToNodeSpace(pTouch));
	return true;
}
void UserPlaneLayer::ccTouchMoved(CCTouch *pTouch,CCEvent *pEvent)
{
	UserPlane* userPlane = UserPlane::shareUserPlane();
	CCPoint newPoint = ccpAdd(convertTouchToNodeSpace(pTouch), m_userPlaneTouchOffset);
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	newPoint.x = min(max(0,newPoint.x), size.width);
	newPoint.y = min(max(0,newPoint.y), size.height);
	userPlane->setPosition(newPoint);
}

