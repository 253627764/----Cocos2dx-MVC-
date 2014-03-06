#ifndef __HP_OBJECT__H
#define __HP_OBJECT__H

#include "Base/GameObject.h"

/*
	HP(有血量)类
*/
class HpObject: public GameObject
{
public:
	CC_SYNTHESIZE_READONLY(int, m_maxHp, MaxHp)                            //最大血量
	CC_PROPERTY(int, m_currentHp, Hp)                                      //当前血量
	//碰撞特效粒子文件名
	CC_SYNTHESIZE_PASS_BY_REF(CCString, m_hitParticleName, HitParticleName);
	//Hp不同比例显示粒子效果结点的批渲染粒子节点
	CC_SYNTHESIZE_RETAIN(CCParticleBatchNode*, m_hpParticleBatchNode, HpParticleBatchNode)
public:
	HpObject();
	~HpObject();
	//inherited from GameObject
	virtual bool init(CCDictionary* dict);
	virtual void retrieve();
	//inherited from GameObject
	void setBatchParticleNodesParent(CCNode* parentNode);

	//运行被消灭的动画
	virtual void runDeadAnimation(){};
	//运行碰撞粒子特效
	virtual void runHitParticle(const CCPoint& point);
protected:
	CCString m_deadMessage;
protected:
	virtual void adjustParticleBatchNodePosition();                   //校正静态粒子和HP批渲染节点的位置
private:
	void initHpParticles(CCDictionary* dict);                         //初始化HP粒子组,不同比例HP时粒子效果
	void setParticleWithHpPercent(float hpPercent);                   //设置不同HP比例的粒子效果	

};

#endif