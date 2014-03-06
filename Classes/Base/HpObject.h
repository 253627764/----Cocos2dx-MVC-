#ifndef __HP_OBJECT__H
#define __HP_OBJECT__H

#include "Base/GameObject.h"

/*
	HP(��Ѫ��)��
*/
class HpObject: public GameObject
{
public:
	CC_SYNTHESIZE_READONLY(int, m_maxHp, MaxHp)                            //���Ѫ��
	CC_PROPERTY(int, m_currentHp, Hp)                                      //��ǰѪ��
	//��ײ��Ч�����ļ���
	CC_SYNTHESIZE_PASS_BY_REF(CCString, m_hitParticleName, HitParticleName);
	//Hp��ͬ������ʾ����Ч����������Ⱦ���ӽڵ�
	CC_SYNTHESIZE_RETAIN(CCParticleBatchNode*, m_hpParticleBatchNode, HpParticleBatchNode)
public:
	HpObject();
	~HpObject();
	//inherited from GameObject
	virtual bool init(CCDictionary* dict);
	virtual void retrieve();
	//inherited from GameObject
	void setBatchParticleNodesParent(CCNode* parentNode);

	//���б�����Ķ���
	virtual void runDeadAnimation(){};
	//������ײ������Ч
	virtual void runHitParticle(const CCPoint& point);
protected:
	CCString m_deadMessage;
protected:
	virtual void adjustParticleBatchNodePosition();                   //У����̬���Ӻ�HP����Ⱦ�ڵ��λ��
private:
	void initHpParticles(CCDictionary* dict);                         //��ʼ��HP������,��ͬ����HPʱ����Ч��
	void setParticleWithHpPercent(float hpPercent);                   //���ò�ͬHP����������Ч��	

};

#endif