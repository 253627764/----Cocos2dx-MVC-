#ifndef __playPlane__bullte__
#define __playPlane__bullte__

#include "Base/EmitObject.h"

/*
	子弹类
*/
class Battery;



class Bullte: public EmitObject
{
public:
	CC_SYNTHESIZE(int, m_attack, Attack)              //攻击力
public:
	Bullte();
	//inherited from GameObject
	virtual bool init(CCDictionary* dict);
	virtual GameObjectPool* getFactory();
	//inherited from EmitObject
	virtual bool changeCollisionObject(GameObject* object, const CCPoint& collisionPoint);

protected:
	static GameObjectPool* _factory;  //self Factory Point
};
/*
   子弹工厂类
*/
class BullteFactory : public GameObjectPool
{
private:
	static BullteFactory* _factory;
public:
	static BullteFactory* shareFactory();
	~BullteFactory();
	virtual GameObject * constructObject();
	bool getBullteArray(CCDictionary* dict, CCArray& ret, CCNode* owner);
};
#endif /* defined(__playPlane__bullte__) */