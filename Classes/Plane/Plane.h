#ifndef __PLANE__H__
#define __PLANE__H__

#include "Base/HpObject.h"
#include "Base/EmitObject.h"

/*
  飞机基类
*/
class Plane : public HpObject, public CollisionDelegate
{
public:

protected:
    CCArray m_batterys;      //炮塔组
	CCRect m_collisionRect;  //碰撞范围
public:
	//设置子弹的飞行空间
	void setBullteFlySpace(CCNode* node);
	//开始发射子弹 
	void startEmitBulltes();    
	//停止发射子弹
	void stopEmitBulltes();
	//设置炮塔的可见性
	void setPatterysVisible(bool value);

	 //inherited from GameObject
    virtual bool init(CCDictionary* dict);
	virtual void retrieve(); 

	//inherited from HpObject
	virtual void runDeadAnimation();

	//inherited from CollisionDelegate
	virtual const CCRect& getCollisionRect(); 
	virtual bool collision(EmitObject* emitObject, const CCPoint& collisionPoint);
	virtual bool isDead();
private:
	//初始化炮塔组
	void initBatterys(CCDictionary* dict);
	
};
/*
	用户战机
*/
class UserPlane : public Plane
{
public:
	static CCArray _userPlanes;                            //用户机组，用于敌机攻击对象组
public:
	virtual ~UserPlane();
	static UserPlane* shareUserPlane();
	//继承自GameObject
	virtual bool init(CCDictionary* dict);  
	virtual void setHp(int hp);
	virtual void retrieve();
	//inherited from CollisionDelegate
	bool collision(EmitObject* emitObject, const CCPoint& collisionPoint);
private:
	UserPlane(){};
	

	static UserPlane* _userPlane;
};
/*
	敌机
*/
class EnemyPlane: public Plane
{
public:
	CC_SYNTHESIZE(int, m_money, Money)                       //金钱
	CC_SYNTHESIZE(int, m_score, Score)                       //积分
	CC_SYNTHESIZE(float, m_aircraftTime, AircraftTime)       //飞机出场时间
	CC_SYNTHESIZE(bool, m_move, Move)                        //是否移动
public:
	static CCArray _enemyPlanes;                             //全部敌机组，用于用户飞机攻击对象组
	static void stopEnemyPlanesEmit();                       //停止所有敌机的发射
public:
	//继承自GameObject
	virtual bool init(CCDictionary* dict);
	virtual GameObjectPool* getFactory();
	virtual void retrieve(); 
	virtual void setHp(int hp);
	//inherited from CollisionDelegate
	virtual bool collision(EmitObject* emitObject, const CCPoint& collisionPoint);
	//起飞
	virtual void startFly(float delta);
	//准备起飞
	void prepareFly();
private:
	//随机移动
	void randomMove();

private:
	static GameObjectPool* _factory;                               //自身工厂对象指针
};
/*
   敌机工厂类
*/
class EnemyPlaneFactory : public GameObjectPool
{
public:
	static EnemyPlaneFactory* shareEnemyPlaneFactory();
	virtual GameObject * constructObject();
	~EnemyPlaneFactory();
private:
	static EnemyPlaneFactory *_factory;
};
#endif /* defined(__playPlane__plane__) */
