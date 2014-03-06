#include "HpObject.h"
#include "Base/ParticleBatchNodeCache.h"
/*
	HpObject
*/
HpObject::HpObject()
	:m_maxHp(0),m_currentHp(0),m_hpParticleBatchNode(NULL)
{
}
HpObject::~HpObject()
{
	CC_SAFE_RELEASE_NULL(m_hpParticleBatchNode);
}
bool HpObject::init( CCDictionary* dict )
{
	if(GameObject::init(dict)){
		m_currentHp = dict->valueForKey("hp")->intValue();                        //Ѫ��
		m_maxHp     = m_currentHp;                                                //���Ѫ��
		//��ʼ��HP������,��ͬ����HPʱ����Ч��
		initHpParticles(dict);
		return true;
	}
	return false;
}

void HpObject::retrieve()
{
	GameObject::retrieve();
	if(m_hpParticleBatchNode){
		//����HP��̬���ӽ��
		CCObject* obj;
		CCArray* particles = m_hpParticleBatchNode->getChildren();
		CCARRAY_FOREACH(particles,obj){
			CCParticleSystemQuad* node = dynamic_cast<CCParticleSystemQuad*>(obj);
			node->setUserObject(NULL);		
		}
		m_hpParticleBatchNode->removeFromParent();
		CC_SAFE_RELEASE_NULL(m_hpParticleBatchNode);
	}
}

void HpObject::runHitParticle(const CCPoint& point)
{
	const char* hitParticleName = m_hitParticleName.getCString();
	CCParticleBatchNode* partileBatchNode = ParticleBatchNodeCache::
		sharedParticleBatchNodeCache()->particleBatchNodeByName(hitParticleName);
	CCNode* particleParent = getBatchParticleNodesParent();
	if(partileBatchNode && particleParent){
		AppConfig* config = AppConfig::shareAppConfig();
		CCParticleSystemQuad* particle = config->createParticle(hitParticleName,partileBatchNode);
		particle->setAutoRemoveOnFinish(true);
		particle->setPosition(point);
		if(partileBatchNode->getParent() != particleParent){
			if(partileBatchNode->getParent() != NULL)
				partileBatchNode->getParent()->removeChild(partileBatchNode,false); 
			particleParent->addChild(partileBatchNode);
		}
	}
}

void HpObject::initHpParticles( CCDictionary* dict )
{
	CC_SAFE_RELEASE_NULL(m_hpParticleBatchNode);
	CCDictionary* particleEffectDict = dynamic_cast<CCDictionary*>(dict->objectForKey("hpParticles"));
	if(particleEffectDict){
		CCArray* particles = dynamic_cast<CCArray*>(particleEffectDict->objectForKey("particle"));
		//������������Ⱦ�ڵ�
		const char* particleName = particleEffectDict->valueForKey("plistName")->getCString();
		AppConfig* config = AppConfig::shareAppConfig();
		setHpParticleBatchNode(config->createParticleBatchNode(particleName,particles->count()));
		//��������ƫ��λ��
		setParticlesPosition(m_hpParticleBatchNode, particles);
		//������������
		CCArray* particleNodes = m_hpParticleBatchNode->getChildren();
		for(unsigned index = 0; index < particles->count(); index++){
			CCParticleSystemQuad* particle = dynamic_cast<CCParticleSystemQuad*>(particleNodes->objectAtIndex(index));
			CCDictionary* particleDict = dynamic_cast<CCDictionary*>(particles->objectAtIndex(index));
			//�����Ӷ���TAG��֤����HP����
			float hpPercentage = particleDict->valueForKey("hpPercentage")->floatValue();
			particle->setUserObject(CCFloat::create(hpPercentage));
			particle->setVisible(false);
		}
	}
}

void HpObject::setBatchParticleNodesParent(CCNode* parentNode)
{
	GameObject::setBatchParticleNodesParent(parentNode);
	//����HP������ĸ����
	if(m_batchParticleNodeParent) m_batchParticleNodeParent->removeChild(m_hpParticleBatchNode,false);
	if(m_hpParticleBatchNode){
		CCArray* childs = parentNode->getChildren();
		if(childs && !childs->containsObject(m_hpParticleBatchNode))
			parentNode->addChild(m_hpParticleBatchNode);
	}
	m_batchParticleNodeParent = parentNode;
	adjustParticleBatchNodePosition();
}

void HpObject::adjustParticleBatchNodePosition()
{
	GameObject::adjustParticleBatchNodePosition();
	CCPoint pt = ccpSub(getPosition(),ccp(boundingBox().size.width / 2, boundingBox().size.height / 2));
	if(m_hpParticleBatchNode) m_hpParticleBatchNode->setPosition(pt);
}

void HpObject::setParticleWithHpPercent(float hpPercent)
{
	CCObject* obj = NULL;
	if(m_hpParticleBatchNode){ 
		CCArray* particles = m_hpParticleBatchNode->getChildren();
		CCARRAY_FOREACH(particles,obj){
			CCParticleSystemQuad* node = dynamic_cast<CCParticleSystemQuad*>(obj);
			float hpPercentage = dynamic_cast<CCFloat*>(node->getUserObject())->getValue();
			node->setVisible(hpPercent > 0.0f && hpPercent <= hpPercentage);
			//node->resetSystem();
		}
	}
}

void HpObject::setHp(int hp)
{
	m_currentHp = max(min(m_maxHp,hp) ,0);
	m_hpParticleBatchNode->setVisible(m_currentHp > 0);
	setParticleWithHpPercent(float(m_currentHp) / float(m_maxHp));
}

int HpObject::getHp()
{
	return m_currentHp;
}