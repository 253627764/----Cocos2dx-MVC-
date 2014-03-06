#ifndef __EMIT__OBJECT__H__
#define __EMIT__OBJECT__H__

#include "GameObject.h"
enum
{
	kNormalFlyAction = 0,
	kSinFlyAction    = 1
};
/*
	�ɻ���Ϊ����
*/
class EmitObject;
class FlyActionFactory : public CCObject
{
public:
	static void releaseFlyActionFactorys();                  //�ͷŹ���������
	static CCArray _flyActionFactorys;                       //�����ж����������
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
	CC_SYNTHESIZE(float, m_radius, Radius)                     //���߰뾶
	CC_SYNTHESIZE_PASS_BY_REF(CCPoint, m_stepOffset, Target)   //����ƫ��
public:
	static SinflyActionFactory* shareSinflyActionFactory();
	~SinflyActionFactory();
	virtual void init(CCDictionary* dict);
	virtual CCActionInterval* getAction(const CCPoint& target, EmitObject* emitObject);
private:
	static SinflyActionFactory* _shareSinflyActionFactory;
};
/*
	��ײ������
*/
class CollisionDelegate
{
public:
	 //��ײ����
	virtual const CCRect& getCollisionRect() = 0; 
	 //������ײ
	virtual bool collision(EmitObject* emitObject, const CCPoint& collisionPoint) = 0;
	 //�Ƿ�����
	virtual bool isDead() = 0;
};
/*
	ӵ���ߴ�����
*/
class OwnerDelegate
{
public:
	virtual void retrieveObject(GameObject* pObject) = 0;
};
/*
	������
*/
class EmitObject: public GameObject
{
public:
	CC_SYNTHESIZE(CCNode*,m_flySpace,FlySpace)                    //���пռ�
	CC_SYNTHESIZE(float,m_speed,Speed)                            //�ٶ�
	CC_SYNTHESIZE(OwnerDelegate*,m_owner,Owner)                   //ӵ����
	CC_SYNTHESIZE(bool, m_isFlying, IsFlying)                     //�Ƿ��ڷ�
	CC_SYNTHESIZE(CCArray*, m_pCollisionObjects,CollisionObjects) //��ײĿ����
public:
	EmitObject();
	virtual ~EmitObject();
	//�ı䱻��ײ����
	virtual bool changeCollisionObject(GameObject* object, const CCPoint& collisionPoint) = 0;
	//����Ŀ��
	void flyTo(const CCPoint& target); 

	//inherited from CCNode
	virtual void update(float delta);
	//inherited from GameObject
	virtual bool init(CCDictionary* dict);
	virtual void retrieve();
protected:
	FlyActionFactory* m_pFlyActinfactory;                         //������Ϊ����	
protected:
	bool collisionTest();                                         //��ײ����
	bool boundBoxInSpace();                                       //�Ƿ���пռ䷶Χ����
	void retrieveToOwner();                                       //������ӵ����
};

#endif