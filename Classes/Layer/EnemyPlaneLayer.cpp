#include "EnemyPlaneLayer.h"
#include "Plane/Plane.h"
#include "Bullte/Bullte.h"
#include "Layer/PartsLayer.h"

EnemyPlaneLayer::~EnemyPlaneLayer()
{
	CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, USERPLANE_READY_MSG);
}
bool EnemyPlaneLayer::init()
{    
	if ( !CCLayer::init() )
		return false;
	m_enemyPlanesArray.initWithArray(CCArray::createWithContentsOfFile(ENEMYPLANE_PLIST));

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(EnemyPlaneLayer::userPlaneReadyCallBack),USERPLANE_READY_MSG,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(EnemyPlaneLayer::enemyPlaneDeadCallBack),ENEMYPLANE_DEAD_MESSAGE,NULL);
	return true;
}

void EnemyPlaneLayer::enemyPlaneDeadCallBack(CCObject *obj)
{
	EnemyPlane* enemyPlane = dynamic_cast<EnemyPlane*>(obj);
	if(enemyPlane){
		enemyPlane->removeFromParent();
		enemyPlane->retrieve();
	}
}

void EnemyPlaneLayer::userPlaneReadyCallBack(CCObject *obj)
{
	delayTimerCallLoadEnemys(0.0f);
}

void EnemyPlaneLayer::delayTimerCallLoadEnemys(float dt)
{
	CCDelayTime * delayAction = CCDelayTime::create(dt);  
	CCCallFunc * callFunc = CCCallFunc::create(this,callfunc_selector(EnemyPlaneLayer::loadEnemys));  
	this->runAction(CCSequence::create(delayAction,callFunc,NULL)); 
}
EnemyPlane* EnemyPlaneLayer::_enemyPlane = NULL;

void EnemyPlaneLayer::loadEnemys()
{
	//当一批飞机全部添加后，此批敌机没有被消灭干净前，不再增加下批敌机
	if((EnemyPlane::_enemyPlanes.count() > 0)){
		delayTimerCallLoadEnemys(1.0f);
		return;
	}
	static int batchIndex      = 0;                      //第几批敌机
	float aircraftTimeSum = 0;                           //每批敌机出场时间和
	//当前批敌机数组
	CCArray* batchEnemys = dynamic_cast<CCArray*>(m_enemyPlanesArray.objectAtIndex(batchIndex));
	CCObject* obj = NULL;
	CCARRAY_FOREACH(batchEnemys,obj){
		CCDictionary* enemyDict  = dynamic_cast<CCDictionary*>(obj);
		//当前敌机属性字典，获取敌机出场时间
		GameObjectPool* factory = EnemyPlaneFactory::shareEnemyPlaneFactory();
		EnemyPlane* enemyPlane = NULL;
		if(factory)
		{
			enemyPlane = dynamic_cast<EnemyPlane*>(factory->getObject(enemyDict));
			//调式用的全局敌机变量
			_enemyPlane = enemyPlane;
			//设置被碰撞粒子效果的文件名
			enemyPlane->setHitParticleName(NORMAL_HIT_PARTICLE_PLIST);
			//粒子效果的父层需要是非批渲染层
			enemyPlane->setBatchParticleNodesParent(this);

		//计算出场时间之和
		aircraftTimeSum += enemyPlane->getAircraftTime();
		//计算出场位置
		srand((unsigned) time(NULL));
		float t = enemyPlane->getScale();
		int t1 = rand();
		float widthRange = CCDirector::sharedDirector()->getWinSize().width - 
			enemyPlane->boundingBox().size.width * enemyPlane->getScale();
		float startPositionX = enemyPlane->boundingBox().size.width / 2 * enemyPlane->getScale() + rand() % int(widthRange);				
		float startPositionY = CCDirector::sharedDirector()->getWinSize().height  + 
			enemyPlane->boundingBox().size.height * enemyPlane->getScale() + 10;

		startPositionX = CCDirector::sharedDirector()->getWinSize().width / 2;
		enemyPlane->setPosition(ccp(startPositionX,startPositionY));

		enemyPlane->prepareFly();
		CCSpriteBatchNode* batchNode = AppConfig::shareAppConfig()->getGlobalBatchNode();
		enemyPlane->setBullteFlySpace(batchNode);
		batchNode->addChild(enemyPlane,ENEMY_PLANE_ZORDER);
		}
	}
	//更新静态的敌机批次索引和敌机出场顺序索引
	batchIndex = (batchIndex + 1) % m_enemyPlanesArray.count();
	delayTimerCallLoadEnemys(aircraftTimeSum);
}


