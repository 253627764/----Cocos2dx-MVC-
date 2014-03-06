#ifndef __EMIT__OBJECT__H__
#define __EMIT__OBJECT__H__

#include "GameObject.h"
enum
{
	kNormalFlyAction = 0,
	kSinFlyAction    = 1
};
/*
	飞机行为工厂
*/
class EmitObject;
class FlyActionFactory : public CCObject
{
public:
	static void releaseFlyActionFactorys();                  //释放工厂对象组
	static CCArray _flyActionFactorys;                       //飞行行动对象池数组
	static FlyActionFactory* getFlyActinFactory(CCDictionary* dict);
	virtual void init(CCDictionary* dict){};
	virtual CCActionInterval* getAction(const CCPoint& target, EmitObject* emitObject){return NULL;};
};

class LineflyActionFactory: public FlyActionFactory
{
public:
	static LineflyActionFactory* shareLineflyActionFactory();
	~LineflyActionFactory();
	virtual void init(CCDictionary* dict){}
	virtual CCActionInterval* getAction(const CCPoint& target, EmitObject* emitObject);
private:
	static LineflyActionFactory* _shareLineflyActionFactory;
};

class SinflyActionFactory: public FlyActionFactory
{                          
	CC_SYNTHESIZE(float, m_radius, Radius)                     //曲线半径
	CC_SYNTHESIZE_PASS_BY_REF(CCPoint, m_stepOffset, Target)   //步进偏移
public:
	static SinflyActionFactory* shareSinflyActionFactory();
	~SinflyActionFactory();
	virtual void init(CCDictionary* dict);
	virtual CCActionInterval* getAction(const CCPoint& target, EmitObject* emitObject);
private:
	static SinflyActionFactory* _shareSinflyActionFactory;
};
/*
	碰撞代理类
*/
class CollisionDelegate
{
public:
	 //碰撞矩形
	virtual const CCRect& getCollisionRect() = 0; 
	 //触发碰撞
	virtual bool collision(EmitObject* emitObject, const CCPoint& collisionPoint) = 0;
	 //是否死亡
	virtual bool isDead() = 0;
};
/*
	拥有者代理类
*/
class OwnerDelegate
{
public:
	virtual void retrieveObject(GameObject* pObject) = 0;
};
/*
	发射类
*/
class EmitObject: public GameObject
{
public:
	CC_SYNTHESIZE(CCNode*,m_flySpace,FlySpace)                    //飞行空间
	CC_SYNTHESIZE(float,m_speed,Speed)                            //速度
	CC_SYNTHESIZE(OwnerDelegate*,m_owner,Owner)                   //拥有者
	CC_SYNTHESIZE(bool, m_isFlying, IsFlying)                     //是否在飞
	CC_SYNTHESIZE(CCArray*, m_pCollisionObjects,CollisionObjects) //碰撞目标组
public:
	EmitObject();
	virtual ~EmitObject();
	//改变被碰撞对象
	virtual bool changeCollisionObject(GameObject* object, const CCPoint& collisionPoint) = 0;
	//飞向目标
	void flyTo(const CCPoint& target); 

	//inherited from CCNode
	virtual void update(float delta);
	//inherited from GameObject
	virtual bool init(CCDictionary* dict);
	virtual void retrieve();
protected:
	FlyActionFactory* m_pFlyActinfactory;                         //飞行行为工厂	
protected:
	bool collisionTest();                                         //碰撞测试
	bool boundBoxInSpace();                                       //是否飞行空间范围里面
	void retrieveToOwner();                                       //回收至拥有者
};

#endif