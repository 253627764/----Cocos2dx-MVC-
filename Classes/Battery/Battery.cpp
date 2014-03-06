#include "Battery.h"
#include "Bullte/Bullte.h"
#include "Layer/GameWorldLayer.h"	//调试时加的
/*
	Battery
*/
GameObjectPool* Battery::_factory = NormalBatteryFactory::shareFactory();  

Battery::Battery()
	:m_emitBulltesIndex(0),m_isEmitBulltes(false),m_autoLockObject(false),
	m_lockedObject(NULL),m_emitMoveAction(NULL),m_emitAction(NULL)
{}

Battery::~Battery()
{
	CC_SAFE_RELEASE_NULL(m_emitMoveAction);
	CC_SAFE_RELEASE_NULL(m_emitAction);
}

bool Battery::init(CCDictionary* dict)
{	
	if(HpObject::init(dict)) 
	{	
		//是否自动锁定目标
		m_autoLockObject = dict->valueForKey("autoLockObject")->boolValue();
		//初始旋转角度
		m_initAngle = dict->valueForKey("angle")->floatValue();      
		setRotation(m_initAngle);  
		//发射频率及是否随机发射频率
		m_emitFrequency = dict->valueForKey("emitFrequency")->floatValue();               
		m_emitFrequency = (m_emitFrequency == 0.0f) ? 10.0f : m_emitFrequency;
		m_randomEmitFrequency = dict->valueForKey("randomEmitFrequency")->boolValue();
		//初始化子弹组及参数
		initMagazine(dict);
		initEmitPositions(dict);
		//自动锁定对象
		if(m_autoLockObject)
			schedule(schedule_selector(Battery::aimAtObjectCallback),0.1f);		
		return true;
	}
	return false;
}

void Battery::initMagazine(CCDictionary* dict)
{
	m_magazine.removeAllObjects();
	CCDictionary * magazineDict = dynamic_cast<CCDictionary *>(dict->objectForKey("magazine"));   
	if(BullteFactory::shareFactory()->getObjectArray(magazineDict, m_magazine)){
		CCObject* obj = NULL;
		CCARRAY_FOREACH(&m_magazine,obj){
			Bullte* bullte = dynamic_cast<Bullte*>(obj);
			bullte->setOwner(this);
			bullte->setVisible(false);
			this->addChild(bullte,FLY_BULLTE_ZORDER);
		}	
	}
}

void Battery::initEmitPositions(CCDictionary* dict)
{
	m_EmitPositions.clear();
	CCArray* emitPositions = dynamic_cast<CCArray*>(dict->objectForKey("emitPositions"));
	if(emitPositions){
		for(unsigned index = 0; index < emitPositions->count(); index++){
			CCDictionary* emitPositionDict = dynamic_cast<CCDictionary*>(emitPositions->objectAtIndex(index));
			CCPoint emitPositionOffset = CCPointFromString(emitPositionDict->valueForKey("offset")->getCString());
			m_EmitPositions.push_back(emitPositionOffset);
		}
	}else{
		CCPoint emitPositionOffset = ccp(getContentSize().width  / 2, getContentSize().height + 20);
		m_EmitPositions.push_back(emitPositionOffset);
	}
	m_enableRandomParallelEmitCount = dict->valueForKey("enableRandomParallelEmitCount")->boolValue();
}

void Battery::setBullteFlySpace( CCNode* node )
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&m_magazine,obj){
		Bullte* bullte = dynamic_cast<Bullte*>(obj);
		bullte->setFlySpace(node);
	}
}

CCFiniteTimeAction* Battery::getEmitMoveAction()
{
	//原点一>后一>进->恢复原点
	int     distance   = 5;
	CCPoint oldPoint   = getPosition();
	float   radian     = CC_DEGREES_TO_RADIANS(getRotation());
	CCPoint startPoint = getPosition() + ccp(sinf(radian),cosf(radian)) * distance * -1;
	CCPoint endPoint   = getPosition() + ccp(sinf(radian),cosf(radian)) * distance * 1;
	CCArray actions;
	actions.addObject(CCMoveTo::create(0.1f,startPoint));
	actions.addObject(CCMoveTo::create(0.05f,endPoint));
	actions.addObject(CCMoveTo::create(0.1f,oldPoint));
	//arrayOfActions.addObject(CCCallFunc::create(this, callfunc_selector(Battery::runTimeingAction)));
	CCSequence* pAction = CCSequence::create(&actions);
	return pAction;
}

void Battery::retrieveObject(GameObject* pObject)
{
	Bullte* bullte = dynamic_cast<Bullte*>(pObject);
	if(bullte && bullte->getOwner() == this){
		bullte->setVisible(false);
		bullte->setPosition(CCPointZero);
		bullte->removeFromParent();
		this->addChild(bullte,FLY_BULLTE_ZORDER);
	}
}

void Battery::setCollisionObjects(CCArray* var)
{
	m_pCollisionObjects = var; 
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&m_magazine,obj)
	{
		Bullte* bullte = dynamic_cast<Bullte*>(obj);
		bullte->setCollisionObjects(var);
	}
}

CCArray* Battery::getCollisionObjects()
{
	return m_pCollisionObjects;
}

void Battery::aimAtObject(GameObject* obj)
{
    CCPoint location = this->getParent()->convertToWorldSpace(this->getPosition());
	float angle = ccpAngleSigned(ccpSub(obj->getPosition(), location), CCPointMake(0, 1));  
    this->setRotation(CC_RADIANS_TO_DEGREES(angle));
}

void Battery::emitBulltes()
{	
	//并行发射子弹数量
	int randomParallerEmitCount = (m_enableRandomParallelEmitCount) ? 
		rand() % m_EmitPositions.size() + 1 : m_EmitPositions.size();
	//让子弹飞
	for(unsigned emitPositionIndex = 0; emitPositionIndex < m_EmitPositions.size(); emitPositionIndex++){
		for(int flyBullteindex = 0; flyBullteindex < randomParallerEmitCount;){	
			Bullte* bullte = dynamic_cast<Bullte*>(m_magazine.objectAtIndex(m_emitBulltesIndex));
			if(bullte && !bullte->getIsFlying()){			
				emitBullte(emitPositionIndex,bullte);
				flyBullteindex++;
				//发射音效
				//SimpleAudioEngine::sharedEngine()->playEffect(USERPLANE_DEFAULT_EMIT_SE_FILE, false);
			}
			m_emitBulltesIndex = (m_emitBulltesIndex + 1) % m_magazine.count();
			if(m_emitBulltesIndex == m_magazine.count() - 1) return;
			//if(m_magazine.count() == 1) m_emitBulltesIndex = 1;
		}
	}
}

void Battery::emitBullte(unsigned emitPositionIndex, Bullte* bullte)
{
	AppConfig* config = AppConfig::shareAppConfig();
	//发射位置
	CCPoint emitPosition = getEmitPosition(emitPositionIndex,bullte);
	bullte->setPosition(emitPosition);
	config->printPoint(emitPosition);
	//目标位置
	float emitAngle = -getRotation();
	CCPoint targetPosition = config->getTargetPosition(emitPosition,getEmitDistance(),emitAngle);	
	bullte->flyTo(targetPosition);
}

cocos2d::CCPoint Battery::getEmitPosition(unsigned emitPositionIndex, Bullte* bullte)
{
	AppConfig* config = AppConfig::shareAppConfig();
	CCPoint emitPosition = config->getTransformPosition(
		this,m_EmitPositions[emitPositionIndex],bullte->getFlySpace());
	return emitPosition;
}

float Battery::getEmitDistance()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	return ccp(winSize.width,winSize.height).getLength(); //世界最大距离
}

void Battery::aimAtObjectCallback(float delta)
{
	if(m_lockedObject) aimAtObject(m_lockedObject); 
}

void Battery::startEmitBulltes(float dt)
{
	setIsEmitBulltes(true);
	
	CCArray actions;
	actions.addObject(CCDelayTime::create(dt));
	actions.addObject(CCCallFunc::create(this,callfunc_selector(Battery::delayTimeEmitBulltes)));
	CCSequence* sequene = CCSequence::create(&actions);

	if(!m_emitAction || m_emitAction->isDone())
	{
		setEmitMoveAction(getEmitMoveAction());
		setEmitAction(CCSpawn::create(getEmitMoveAction(),sequene,NULL));
		this->runAction(m_emitAction);
	}
	else
	{
		this->runAction(sequene);
	}
		
}

void Battery::delayTimeEmitBulltes()
{
	if(m_isEmitBulltes){
		emitBulltes(); 
		//得到发射间隔时间
		float timer = 1.0f/m_emitFrequency;
		if(m_randomEmitFrequency){
			int intTimer = int(timer);
			intTimer = (intTimer) ? intTimer : 1; 
			timer = rand() % intTimer + 1;
		}
		startEmitBulltes(timer);
	}
}

void Battery::retrieve()
{
	//取消自动锁定
	if(m_autoLockObject)
		unschedule(schedule_selector(Battery::aimAtObjectCallback));		
	//重置发射子弹标志
	setIsEmitBulltes(false);
	//将还在世界飞的子弹从子弹匣数组中移出，让子弹继续飞，否则将与炮塔一起移出世界
	while(m_magazine.count() > 0){
		Bullte* bullte = dynamic_cast<Bullte*>(m_magazine.objectAtIndex(0));
		//无论子弹是否属于弹匣，子弹的拥有者都需要重置空
		bullte->setOwner(NULL);		
		//将属于炮塔（没有飞出去）的子弹进行回收
		if(getChildren()->containsObject(bullte)) bullte->retrieve();     
		m_magazine.removeObject(bullte,true);
	}
	HpObject::retrieve();
}

GameObjectPool* Battery::getFactory()
{
	return Battery::_factory;
}

/*
	RotateBattery
*/
GameObjectPool* RotateBattery::_factory = RotateBatteryFactory::shareFactory();  

GameObjectPool* RotateBattery::getFactory()
{
	return RotateBattery::_factory;
}
RotateBattery::RotateBattery():
	m_radius(0),m_stepAngle(10),m_clockWise(false),m_currentAngle(0)
{}

bool RotateBattery::init( CCDictionary* dict )
{
	if(Battery::init(dict)){
		m_radius    = dict->valueForKey("radius")->intValue();
		m_radius    = !m_radius ? 20: m_radius; 
		m_stepAngle =  dict->valueForKey("stepAngle")->intValue();
		m_stepAngle = !m_stepAngle ? 10: m_stepAngle; 
		m_clockWise = dict->valueForKey("clockWise")->boolValue();

		return true;
	}
	return false;
}

cocos2d::CCPoint RotateBattery::getEmitPosition(unsigned emitPositionIndex, Bullte* bullte)
{
	CCPoint emitPosition = Battery::getEmitPosition(emitPositionIndex, bullte);
	float delta = m_radius * cos(CC_DEGREES_TO_RADIANS(m_currentAngle));
	m_clockWise ? emitPosition.x += delta : emitPosition.x -= delta;
	m_currentAngle = (m_currentAngle + m_stepAngle) % 360;
	return emitPosition;
}

/*
	BatteryFactory
*/
BatteryFactory* BatteryFactory::getBatteryFactory(CCDictionary* dict)
{
	int batteryType = dict->valueForKey("batteryType")->intValue();
	BatteryFactory* batteryFactory = NULL;
	switch (batteryType){
	case kRotateBattery:
		batteryFactory = RotateBatteryFactory::shareFactory();
		break;
	case kBattery:
	default:
		batteryFactory = NormalBatteryFactory::shareFactory();
		break;
	}
	return batteryFactory;
}
/*
	NormalBatteryFactory
*/
NormalBatteryFactory* NormalBatteryFactory::_factory;
NormalBatteryFactory* factory = NULL;

NormalBatteryFactory* NormalBatteryFactory::shareFactory()
{
	if(!NormalBatteryFactory::_factory){
		NormalBatteryFactory::_factory = new NormalBatteryFactory();
		GameObjectPool::shareGameObjectPools()->addObject(_factory);
	}
	return NormalBatteryFactory::_factory;

}
NormalBatteryFactory::~NormalBatteryFactory()
{
	NormalBatteryFactory::_factory = NULL;
}
GameObject* NormalBatteryFactory::constructObject()
{
	return new Battery();
}
/*
	RotateBatteryFactory
*/
RotateBatteryFactory* RotateBatteryFactory::_factory = NULL;

RotateBatteryFactory* RotateBatteryFactory::shareFactory()
{
	if(!RotateBatteryFactory::_factory){
		RotateBatteryFactory::_factory = new RotateBatteryFactory();
		GameObjectPool::shareGameObjectPools()->addObject(_factory);
	}
	return RotateBatteryFactory::_factory;
}
RotateBatteryFactory::~RotateBatteryFactory()
{
	RotateBatteryFactory::_factory = NULL;
}
GameObject* RotateBatteryFactory::constructObject()
{
	return new RotateBattery();
}