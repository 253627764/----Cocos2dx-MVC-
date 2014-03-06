#include "Bullte.h"
#include "Battery/Battery.h"
#include "Plane/Plane.h"
#include "Layer/GameWorldLayer.h"

/*
	Bullte
*/
GameObjectPool* Bullte::_factory = BullteFactory::shareFactory();

Bullte::Bullte()
	:m_attack(1)
{}

bool Bullte::init(CCDictionary* dict)
{
	if(EmitObject::init(dict)){
		m_attack  = dict->valueForKey("attack")->intValue();
		return true;
	}	
	return false;
}

GameObjectPool* Bullte::getFactory()
{
	return Bullte::_factory;
}

bool Bullte::changeCollisionObject(GameObject* object, const CCPoint& collisionPoint)
{
	HpObject* hpObject = dynamic_cast<HpObject*>(object);
	if(hpObject) hpObject->setHp(hpObject->getHp() - m_attack);
	if(hpObject->getHp() > 0.0f){
		hpObject->runHitParticle(collisionPoint);
		return true;
	}
	else
	{
		hpObject->setStaticParticleVisible(false);
		hpObject->cleanup();
		hpObject->runDeadAnimation();
		return false;
	}
}

/*
	BullteFactory
*/
BullteFactory* BullteFactory::_factory = NULL;

BullteFactory* BullteFactory::shareFactory()
{
	if(!BullteFactory::_factory){
		BullteFactory::_factory = new BullteFactory();
		GameObjectPool::shareGameObjectPools()->addObject(BullteFactory::_factory);
	}
	return BullteFactory::_factory;
}
BullteFactory::~BullteFactory()
{
	BullteFactory::_factory = NULL;
}
GameObject * BullteFactory::constructObject()
{
	GameObject* obj =  new Bullte();
	return obj; 
}
