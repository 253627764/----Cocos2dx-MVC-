#include "Plane.h"
#include "Battery/Battery.h"

/*
	Plane
*/
bool Plane::init(CCDictionary* dict)
{
	if(HpObject::init(dict)){
		//碰撞尺寸
		m_collisionRect = boundingBox();
		//m_collisionRect.size.height = m_collisionRect.size.height / 2;
		const char* pzCollsionSize = dict->valueForKey("collisionSize")->getCString();	
		if(strlen(pzCollsionSize))
			m_collisionRect.size = CCSizeFromString(dict->valueForKey("collisionSize")->getCString());
		//炮塔组
		initBatterys(dict);
		return true;
	}
	return false;
}

void Plane::initBatterys(CCDictionary* dict)
{
	m_batterys.removeAllObjects();
	CCArray* batteryArray = dynamic_cast<CCArray* >(dict->objectForKey("batterys"));
	CCObject* obj = NULL;
	CCARRAY_FOREACH(batteryArray,obj){
		CCDictionary* batteryDict = dynamic_cast<CCDictionary*>(obj);
		BatteryFactory* batteryFactory = BatteryFactory::getBatteryFactory(batteryDict);
		batteryFactory->getObjectArray(batteryDict,m_batterys); 
	}
	CCARRAY_FOREACH(&m_batterys,obj){
		Battery* battery = dynamic_cast<Battery*>(obj);
		if(battery->getOffset().width == 0.0 && battery->getOffset().height == 0.0)
		{
			CCPoint position = ccpAdd(getContentSize() / 2, battery->getOffset());
			battery->setPosition(position);
			battery->setVisible(true);
		}
		addChild(battery,BATTERY_ZORDER);
	}
}

void Plane::retrieve()
{
	//回收炮塔
	while(m_batterys.count() > 0){
		GameObject* obj = dynamic_cast<GameObject*>(m_batterys.objectAtIndex(0));
		obj->retrieve();
		m_batterys.removeObject(obj,true);
	}
	HpObject::retrieve();
}

void Plane::setBullteFlySpace( CCNode* node )
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&m_batterys,obj){
		Battery* battery = dynamic_cast<Battery*>(obj);
		battery->setBullteFlySpace(node);
	}
}

bool Plane::collision(EmitObject* emitObject, const CCPoint& collisionPoint)
{
	return emitObject->changeCollisionObject(this, collisionPoint);
}

bool Plane::isDead()
{
	return (m_currentHp <= 0);
}

void Plane::runDeadAnimation()
{
	setPatterysVisible(false);
	AppConfig* config = AppConfig::shareAppConfig();
	runAction(CCSequence::create(
		config->getActionWithAnimate(USERPLANE_DEATH_ANIMATE_NAME,3.0f), 
		CCCallFuncND::create(this, callfuncND_selector(UserPlane::postMessage),(void*)m_deadMessage.getCString()), 
		NULL));	
}

void Plane::startEmitBulltes()
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&m_batterys,obj){
		Battery* battery = dynamic_cast<Battery*>(obj);
		battery->startEmitBulltes(1.0f);
	}
}

void Plane::stopEmitBulltes()
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&m_batterys,obj)
	{
		Battery* battery = dynamic_cast<Battery*>(obj);
		battery->setIsEmitBulltes(false);
	}
}

void Plane::setPatterysVisible(bool value)
{
	for(unsigned index = 0; index < m_batterys.count(); index++){
		GameObject* obj = dynamic_cast<GameObject*>(m_batterys.objectAtIndex(index));
		obj->setVisible(value);
	} 
}

const CCRect& Plane::getCollisionRect()
{
	m_collisionRect.origin = boundingBox().origin;
	m_collisionRect.origin = getParent()->convertToWorldSpace(m_collisionRect.origin);
	return m_collisionRect;
}

/*
	UserPlane
*/
UserPlane* UserPlane::_userPlane = NULL;
CCArray UserPlane::_userPlanes;

UserPlane* UserPlane::shareUserPlane()
{
	if(!_userPlane)
		_userPlane = new UserPlane();
	return _userPlane;
}
UserPlane::~UserPlane()
{
	_userPlane = NULL;
}
bool UserPlane::init(CCDictionary* dict)
{
	if(Plane::init(dict))
	{
		m_deadMessage = USERPLANE_DEAD_MESSAGE;
		_userPlanes.addObject(_userPlane);	

		CCObject* obj = NULL;
		CCARRAY_FOREACH(&m_batterys,obj)
		{
			Battery* battery = dynamic_cast<Battery*>(obj);
			battery->setCollisionObjects(&EnemyPlane::_enemyPlanes);
			battery->setLockedObject(NULL);
		}
		return true;
	}
	return false;
}

bool UserPlane::collision(EmitObject* emitObject, const CCPoint& collisionPoint)
{
	return Plane::collision(emitObject, collisionPoint);
}

void UserPlane::setHp(int hp)
{
	Plane::setHp(hp);
	postMessage(this,USERPLANE_HP_CHANGE_MSG);
}

void UserPlane::retrieve()
{
	Plane::retrieve();
	_userPlanes.removeObject(this);
}
/*
	EnemyPlane
*/
CCArray EnemyPlane::_enemyPlanes;
GameObjectPool* EnemyPlane::_factory = EnemyPlaneFactory::shareEnemyPlaneFactory();

void EnemyPlane::stopEnemyPlanesEmit()
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&EnemyPlane::_enemyPlanes,obj){
		EnemyPlane* plane = dynamic_cast<EnemyPlane*>(obj);
		plane->stopEmitBulltes();
	}
}

bool EnemyPlane::init(CCDictionary* dict)
{
	if(Plane::init(dict)){
		m_deadMessage = ENEMYPLANE_DEAD_MESSAGE;
		_enemyPlanes.addObject(this);

		m_money         = dict->valueForKey("money")->intValue();
		m_score         = dict->valueForKey("score")->intValue();
		m_aircraftTime  = dict->valueForKey("aircraftTime")->floatValue();
		m_move          = dict->valueForKey("move")->boolValue();
		//炮塔组
		CCObject* obj = NULL;
		CCARRAY_FOREACH(&m_batterys,obj){
			Battery* battery = dynamic_cast<Battery*>(obj);
			battery->setCollisionObjects(&UserPlane::_userPlanes);
			battery->setLockedObject(UserPlane::shareUserPlane());
		}

		return true;
	}
	return false;
}
GameObjectPool* EnemyPlane::getFactory()
{
	return EnemyPlane::_factory;
}
void EnemyPlane::retrieve()
{
	//从静态敌机数组中移除自身
	_enemyPlanes.removeObject(this);
	Plane::retrieve();
}
void EnemyPlane::prepareFly()
{
	scheduleOnce(schedule_selector(EnemyPlane::startFly), m_aircraftTime);
}
void EnemyPlane::startFly( float delta )
{
	startEmitBulltes();
	if(m_move)
		randomMove();
	else{
		//垂直向下移动敌机至某位置，固定不动
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		float x = getPosition().x;
		float y = winSize.height - boundingBox().size.height / 2 - 50;
		this->runAction(CCMoveTo::create(2, ccp(x,y)));
	}
}
void EnemyPlane::randomMove()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float x = boundingBox().size.width / 2.0f * getScale() + 
		rand() % int(winSize.width - boundingBox().size.width / 2.0f  * getScale());
	float y = winSize.height * 5.0f / 8.0f  + 
		rand() % int(winSize.height * 3.0f / 8.0f - boundingBox().size.height / 2.0f  * getScale());

	CCMoveTo *move     = CCMoveTo::create(3, ccp(x,y));
	CCDelayTime *delay = CCDelayTime::create(rand() % 3);
	CCCallFunc *reback = CCCallFunc::create(this, callfunc_selector(EnemyPlane::randomMove));
	CCSequence *sequee = CCSequence::create(move,delay,reback,NULL);
	this->runAction(sequee);
}
void EnemyPlane::setHp(int hp)
{
	Plane::setHp(hp);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(ENEMYPLANE_HP_CHANGE_MSG,this);
}

bool EnemyPlane::collision( EmitObject* emitObject, const CCPoint& collisionPoint )
{
	return Plane::collision(emitObject, collisionPoint);
}

/*
	EnemyPlaneFactory
*/
EnemyPlaneFactory* EnemyPlaneFactory::_factory = NULL;

EnemyPlaneFactory* EnemyPlaneFactory::shareEnemyPlaneFactory()
{
	if(!EnemyPlaneFactory::_factory){
		EnemyPlaneFactory::_factory = new EnemyPlaneFactory();
		GameObjectPool::shareGameObjectPools()->addObject(EnemyPlaneFactory::_factory);
	}
	return EnemyPlaneFactory::_factory;
}
EnemyPlaneFactory::~EnemyPlaneFactory()
{
	EnemyPlaneFactory::_factory = NULL;
}
GameObject* EnemyPlaneFactory::constructObject()
{
	return new EnemyPlane();
}