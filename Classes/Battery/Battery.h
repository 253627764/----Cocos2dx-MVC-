#ifndef __GamePlane__Battery__
#define __GamePlane__Battery__

#include "Base/HpObject.h"
#include "Base/EmitObject.h"
/*
	炮塔类型枚举
*/
enum
{
	kBattery = 0,
	kRotateBattery = 1
};

/*
	 炮塔基类
*/
class Bullte;
class BatteryFactory;

class Battery : public HpObject, public OwnerDelegate
{
public:
	//初始角度
	CC_SYNTHESIZE_READONLY(float,m_initAngle,InitAngle)
	//炮塔后推力行为
	CC_SYNTHESIZE_RETAIN(CCFiniteTimeAction*, m_emitMoveAction, EmitMoveAction)
	//发射行为
	CC_SYNTHESIZE_RETAIN(CCFiniteTimeAction*, m_emitAction, EmitAction)
	//被锁定目标
	CC_SYNTHESIZE(GameObject*,m_lockedObject,LockedObject)
	//是否自动锁定目标
	CC_SYNTHESIZE(bool, m_autoLockObject, AutoLockObject)
	//有多个子弹发射初始位置时，是否使用随机并发子弹数量
	CC_SYNTHESIZE(bool,m_enableRandomParallelEmitCount,EnableRandomParallelEmitCount)
	//发射频率 
	CC_SYNTHESIZE(float,m_emitFrequency,EmitFrequency)
	//是否随机发射频率
	CC_SYNTHESIZE(bool,m_randomEmitFrequency,RrandomEmitFrequency)
	//是否发射子弹
	CC_SYNTHESIZE(bool,m_isEmitBulltes,IsEmitBulltes)
	//碰撞目标
	CC_PROPERTY(CCArray*,m_pCollisionObjects,CollisionObjects)
protected:          
	CCArray m_magazine;                                            //弹匣
	vector<CCPoint> m_EmitPositions;                               //子弹组（同时发射多颗子弹）发射初始位置
	unsigned int m_emitBulltesIndex;                               //发射子弹索引 
public:
	Battery();
	~Battery();
	void setBullteFlySpace(CCNode* node);                          //设置子弹的飞行空间
	void startEmitBulltes(float dt);                               //开始发射子弹
	CCFiniteTimeAction* getEmitMoveAction();                       //得到炮塔发射后推力行为
	//inherited from GameObject
	virtual bool init(CCDictionary* dict);
	virtual GameObjectPool* getFactory();
	virtual void retrieve();
	//inherited from OwnerDelegate
	virtual void retrieveObject(GameObject* pObject); 
protected:
	//获得子弹发射位置
	virtual CCPoint getEmitPosition(unsigned emitPositionIndex, Bullte* bullte);
	//发射距离
	virtual float getEmitDistance(); 
	//瞄准目标
	void aimAtObject(GameObject* obj);                       
private:
	//初始化弹匣
	void initMagazine(CCDictionary* dict);
	//初始化子弹组（同时发射多颗子弹）发射初始位置
	void initEmitPositions(CCDictionary* dict);
	//发射子弹组
	void emitBulltes();                                      
	//发射单颗子弹
	void emitBullte(unsigned emitPositionIndex, Bullte* bullte);
	//延时发射子弹
	void delayTimeEmitBulltes();  
	//瞄准目标回调函数
	void aimAtObjectCallback(float delta);                   
private:
	static GameObjectPool* _factory;                         //自身工厂对象指针

};

/*
	旋转发射炮塔类
*/
class RotateBattery : public Battery
{
public:
	CC_SYNTHESIZE(int,m_radius,Radius)                             //子弹发射半径
	CC_SYNTHESIZE(bool,m_clockWise,ClockWise)                      //是否顺时针发射
	CC_SYNTHESIZE(int,m_stepAngle,StepAngle)					   //子弹发射角度步长
public:
	RotateBattery();
	//继承自GameObject
	virtual bool init(CCDictionary* dict);
	virtual GameObjectPool* getFactory();
protected:
	//继承Battery
	virtual CCPoint getEmitPosition(unsigned emitPositionIndex, Bullte* bullte);	       		   
private:
	int m_currentAngle;                                            //子弹当前发射角度
	static GameObjectPool* _factory;                               //自身工厂对象指针
};
/*
   炮塔工厂基类
*/
class BatteryFactory : public GameObjectPool
{
public:
	static BatteryFactory* getBatteryFactory(CCDictionary* dict);
};
/*
   默认炮塔工厂类
*/
class NormalBatteryFactory : public BatteryFactory
{
private:
	static NormalBatteryFactory* _factory;
public:
	static NormalBatteryFactory* shareFactory();
	~NormalBatteryFactory();
	virtual GameObject* constructObject();
};
/*
   旋转炮塔工厂类
*/
class RotateBatteryFactory : public BatteryFactory
{
private:
	static RotateBatteryFactory *_factory;
public:
	static RotateBatteryFactory* shareFactory();
	~RotateBatteryFactory();
	virtual GameObject* constructObject();
};
#endif /* defined(__GamePlane__Battery__) */
