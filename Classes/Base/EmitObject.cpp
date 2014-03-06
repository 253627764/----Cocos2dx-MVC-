#include "EmitObject.h"
#include "Extension/ActionInterval.h"

/*
	FlyActionFactory
*/
CCArray FlyActionFactory::_flyActionFactorys;

void FlyActionFactory::releaseFlyActionFactorys()
{
	for(unsigned index = 0; index < FlyActionFactory::_flyActionFactorys.count(); index++){
		FlyActionFactory* flyActionFactory =  
			dynamic_cast<FlyActionFactory*>(FlyActionFactory::_flyActionFactorys.objectAtIndex(0));
		if(flyActionFactory) flyActionFactory->release();
		FlyActionFactory::_flyActionFactorys.removeObject(flyActionFactory,true);
	}
}
FlyActionFactory* FlyActionFactory::getFlyActinFactory(CCDictionary* dict)
{
	CCDictionary * flyActionDict = dynamic_cast<CCDictionary *>(dict->objectForKey("flyAction"));
	//������Ϊ����
	int actionType = (!flyActionDict) ? kNormalFlyAction : flyActionDict->valueForKey("flyActionType")->intValue(); 
	FlyActionFactory* pFlyActinfactory = NULL;
	switch (actionType){
	case kSinFlyAction:
		pFlyActinfactory = SinflyActionFactory::shareSinflyActionFactory();
		break;
	case kNormalFlyAction:
	default:
		pFlyActinfactory = LineflyActionFactory::shareLineflyActionFactory();
		break;
	}
	pFlyActinfactory->init(flyActionDict);
	return pFlyActinfactory;
}
/*
	LineflyActionFactory
*/
LineflyActionFactory* LineflyActionFactory::_shareLineflyActionFactory = NULL;

LineflyActionFactory* LineflyActionFactory::shareLineflyActionFactory()
{
	if(!_shareLineflyActionFactory){
		_shareLineflyActionFactory = new LineflyActionFactory();
		_flyActionFactorys.addObject(_shareLineflyActionFactory);
	}
	return _shareLineflyActionFactory;
	
}
LineflyActionFactory::~LineflyActionFactory()
{
	LineflyActionFactory::_shareLineflyActionFactory = NULL;
}
CCActionInterval* LineflyActionFactory::getAction(const CCPoint& target, EmitObject* emitObject)
{
	float time = target.getDistance(emitObject->getPosition()) /  emitObject->getSpeed();
	CCActionInterval* flyAction = CCMoveTo::create(time,target);
	return flyAction;
}
/*
	SinflyActionFactory
*/
SinflyActionFactory* SinflyActionFactory::_shareSinflyActionFactory = NULL;

SinflyActionFactory* SinflyActionFactory::shareSinflyActionFactory()
{
	if(!_shareSinflyActionFactory){
		_shareSinflyActionFactory = new SinflyActionFactory();
		_flyActionFactorys.addObject(_shareSinflyActionFactory);
	}
	return _shareSinflyActionFactory;
}
SinflyActionFactory::~SinflyActionFactory()
{
	_shareSinflyActionFactory = NULL;
}
void SinflyActionFactory::init(CCDictionary* dict)
{
	m_radius = dict->valueForKey("radius")->floatValue();    
	m_stepOffset = CCPointFromString(dict->valueForKey("stepOffset")->getCString()) ;   
}
CCActionInterval* SinflyActionFactory::getAction(const CCPoint& target, EmitObject* emitObject)
{
	float time = target.getDistance(emitObject->getPosition()) /  emitObject->getSpeed();
	SineBy* pSineBy = SineBy::create(time / 4, m_stepOffset, m_radius);
	//pSineBy->setRotateTarget(false);
	CCActionInterval *flyAction = CCRepeat::create(pSineBy, -1);
	return flyAction;
}
/*
	EmitObject
*/
EmitObject::EmitObject():
	m_speed(1),m_isFlying(false),m_owner(NULL),
	m_pCollisionObjects(NULL),m_flySpace(NULL)
{}

EmitObject::~EmitObject()
{}

bool EmitObject::init(CCDictionary* dict)
{  
	if(GameObject::init(dict)){
		m_speed = dict->valueForKey("speed")->floatValue();
		m_pFlyActinfactory = FlyActionFactory::getFlyActinFactory(dict);
		return true;
	}	
	return false;
}

void EmitObject::retrieve()
{
	setIsFlying(false);
	GameObject::retrieve();
}

void EmitObject::flyTo(const CCPoint& target)
{	
	//���÷���״̬
	setVisible(true);
	setIsFlying(true);
	//�л����пռ�
	removeFromParent();
	m_flySpace->addChild(this);
	//�ӵ�ͷҪ����Ŀ��
	float angle = ccpAngleSigned(ccpSub(target, getPosition()), CCPointMake(0, 1));  
	setRotation(CC_RADIANS_TO_DEGREES(angle));
	//���ӵ�������
	CCActionInterval* m_flyAction = m_pFlyActinfactory->getAction(target, this);
	runAction(m_flyAction);
	//��ʱ����ӵ�״̬
	schedule(schedule_selector(EmitObject::update),0.1f);
}

bool EmitObject::collisionTest()
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(m_pCollisionObjects,obj){
		CollisionDelegate* pCollisionDelegate = dynamic_cast<CollisionDelegate*>(obj);
		if(pCollisionDelegate && !pCollisionDelegate->isDead()){
			const CCRect& collisionRect = pCollisionDelegate->getCollisionRect();
			CCPoint WorldPoint = getParent()->convertToWorldSpace(getPosition());
			if(collisionRect.containsPoint(WorldPoint))
				return pCollisionDelegate->collision(this,WorldPoint);
		}	
	}
	return false;
}

bool EmitObject::boundBoxInSpace()
{
	//CCRect worldRect = m_flySpace->boundingBox();
	CCRect worldRect = AppConfig::shareAppConfig()->getWorldBoundBox();
	CCRect bullteRect = boundingBox();
	return worldRect.intersectsRect(bullteRect);
}

void EmitObject::update(float delta)
{
	GameObject::update(delta);
	if(getIsFlying()){
		if(collisionTest() || !boundBoxInSpace()){
			setIsFlying(false);
			retrieveToOwner();
			unschedule(schedule_selector(EmitObject::update));	
		}
	}
}

void EmitObject::retrieveToOwner()
{
	OwnerDelegate* owner = getOwner();
	if(owner)
		owner->retrieveObject(const_cast<EmitObject*>(this));
	else{
		removeFromParent();
		retrieve();
	}
}