#ifndef __GameObject__H__
#define __GameObject__H__

#include "System/GameDefine.h"
#include "System/AppConfig.h"

/*
	游戏类
*/
class GameObjectPool;

class GameObject : public CCSprite
{
public:
	CC_SYNTHESIZE_PASS_BY_REF(CCString,m_name,Name)                           //桢名
	CC_SYNTHESIZE_READONLY(CCSize,m_offset,Offset)                            //偏移
	CC_SYNTHESIZE(float,m_dispScale,DispScale)  							  //缩放比例
	CC_PROPERTY(CCNode*, m_batchParticleNodeParent, BatchParticleNodesParent) //批渲染粒子父节点
public:
	GameObject();
	virtual ~GameObject();
	virtual bool init(CCDictionary* dict);                                    //从字典初始化
	virtual GameObjectPool* getFactory(){return NULL;}                        //得到自身工厂对象指针					   
	virtual void retrieve();                                                  //回收清理
	void setStaticParticleVisible(bool bVisible);                             //设置静态粒子组的可见性
	//发送通知消息
	void postMessage(CCNode* pSender,void* data);
	//继承CCNode
	virtual void setPosition(const CCPoint &position);
protected:
	CCArray m_particleBatchNodes;                                             //静态粒子组
protected:
	virtual void adjustParticleBatchNodePosition();                           //校正粒子节点位置
	void setParticlesPosition(CCParticleBatchNode* node, CCArray* position);  //设置粒子批渲染节点的子节点的位置
private:
	void initStaticParticles(CCDictionary* dict);                             //初始化静态粒子组

};
/*
	游戏对象池
*/
class GameObjectPool : public CCObject
{
public:	
	static CCArray* shareGameObjectPools();					 //获取游戏对象池数组
	static void releaseAllGameObjects(bool clean);           //释放所有池子对象组中游戏对象组
	static void removeAllUnusedObjects();                    //移除所有没有用的对象
	~GameObjectPool();
	GameObject* getObject(CCDictionary* dict);               //获取单个对象
	bool getObjectArray(CCDictionary* dict, CCArray& ret);   //根据字典获取对象
	bool retrieveObject(GameObject* val);                    //回收单个对象
	bool retrieveObjectArray(CCArray& arr);                  //回收对象数组
	void removeUnusedObjects();                              //移除无用缓存对象
protected:
	virtual GameObject * constructObject() = 0;              //构造对象方法，需要子类继承构造不同类对象
	void makeObjects(unsigned int);                          //构造多个对象
private:
	static CCArray* _gameObjectPools;                        //游戏对象池数组
	CCArray m_objects;                                       //对象数组池
	CCArray m_objectsInuse;                                  //被使用对象数组
};

#endif /* defined(__GamePlane__GameObject__) */
