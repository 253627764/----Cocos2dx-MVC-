#include "GameObject.h"

/*
	GameObject
*/
GameObject::GameObject()
	:m_batchParticleNodeParent(NULL),m_offset(CCPointZero)
{}
GameObject::~GameObject()
{
	
}
bool GameObject::init(CCDictionary* dict)
{
	m_name  = dict->valueForKey("name")->getCString(); 
	//初始化图像
	CCSpriteFrame* sf = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(m_name.getCString());
	if (sf && CCSprite::initWithSpriteFrame(sf))
	{
		//缩放比例
		m_dispScale = dict->valueForKey("scale")->floatValue();
		m_dispScale = (m_dispScale <= 0) ? 1 : m_dispScale;
		setScale(m_dispScale);
		//偏移
		m_offset = CCSizeFromString(dict->valueForKey("offset")->getCString()); 
		setPosition(m_offset);
		//静态粒子效果组
		initStaticParticles(dict);

		return true;
	}
	return false;
}
void GameObject::initStaticParticles( CCDictionary* dict )
{
	m_particleBatchNodes.removeAllObjects();
	CCParticleBatchNode* particleBatchNode = NULL;
	CCArray* particleEffects = dynamic_cast<CCArray*>(dict->objectForKey("staticParticles"));
	if(particleEffects){
		CCObject* obj = NULL;
		//遍历粒子效果数组生成粒子批量渲染节点
		CCARRAY_FOREACH(particleEffects,obj){
			//从粒子效果属性中获取粒子数组
			CCDictionary* particleEffectDict = dynamic_cast<CCDictionary*>(obj);
			CCArray* particles = dynamic_cast<CCArray*>(particleEffectDict->objectForKey("particle"));
			//生成指定数量粒子结点的批渲染节点，将粒子批渲染节点加入管理数组
			const char* particleName = particleEffectDict->valueForKey("plistName")->getCString();
			AppConfig* config = AppConfig::shareAppConfig();
			particleBatchNode = config->createParticleBatchNode(particleName,particles->count());
			particleBatchNode->setVisible(false);
			m_particleBatchNodes.addObject(particleBatchNode);
			//设置粒子偏移位置
			setParticlesPosition(particleBatchNode, particles);
		}
	}
}
CCNode* GameObject::getBatchParticleNodesParent()
{
	return m_batchParticleNodeParent;
}
void GameObject::setBatchParticleNodesParent(CCNode* parentNode)
{
	CCObject* obj = NULL;
	//设置静态粒子组的父结点
	CCARRAY_FOREACH(&m_particleBatchNodes,obj){
		CCParticleBatchNode* node = dynamic_cast<CCParticleBatchNode*>(obj);
		if(m_batchParticleNodeParent) m_batchParticleNodeParent->removeChild(node,false);
		CCArray* childs = parentNode->getChildren();
		if(childs && !childs->containsObject(node))
			parentNode->addChild(node);
	}
	m_batchParticleNodeParent = parentNode;
	adjustParticleBatchNodePosition();
}
void GameObject::setParticlesPosition(CCParticleBatchNode* node, CCArray* positions)
{
	CCArray* particleNodes = node->getChildren();
	for(unsigned index = 0; index < positions->count(); index++){
		CCParticleSystemQuad* particle = dynamic_cast<CCParticleSystemQuad*>(particleNodes->objectAtIndex(index));
		CCDictionary* particleDict = dynamic_cast<CCDictionary*>(positions->objectAtIndex(index));
		CCPoint position = CCPointFromString(particleDict->valueForKey("offset")->getCString());	
		particle->setPosition(position);
	}
}
void GameObject::setPosition(const CCPoint &position)
{
	CCSprite::setPosition(position);
	adjustParticleBatchNodePosition();
}
void GameObject::adjustParticleBatchNodePosition()
{
	CCPoint pt = ccpSub(getPosition(),ccp(boundingBox().size.width / 2, boundingBox().size.height / 2));
	if(m_batchParticleNodeParent){
		CCObject* obj = NULL;
		CCARRAY_FOREACH(&m_particleBatchNodes,obj){
			//将粒子批渲染节点的位置调整为相对父层的自身位置（0，0）
			CCParticleBatchNode* node = dynamic_cast<CCParticleBatchNode*>(obj);		
			node->setPosition(pt);
		}
	}
}
void GameObject::retrieve()
{
	removeAllChildren();
	if(m_batchParticleNodeParent){
		CCObject* obj = NULL;
		//回收静态粒子结点
		CCARRAY_FOREACH(&m_particleBatchNodes,obj){
			CCParticleBatchNode* node = dynamic_cast<CCParticleBatchNode*>(obj);
			node->removeFromParent();
		}
		m_particleBatchNodes.removeAllObjects();
	}
	GameObjectPool* factory = getFactory();
	if(factory) factory->retrieveObject(this);
}

void GameObject::setStaticParticleVisible( bool bVisible )
{
	CCObject* obj = NULL;
	CCARRAY_FOREACH(&m_particleBatchNodes,obj){
		CCParticleBatchNode* particleBatchNode = dynamic_cast<CCParticleBatchNode*>(obj);
		particleBatchNode->setVisible(bVisible);
	}
}

void GameObject::postMessage(CCNode* pSender,void* data)
{
	char* pStr = (char*)(data);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(pStr,this);
}

/*
	GameObjectPool
*/
CCArray* GameObjectPool::_gameObjectPools = NULL;

CCArray* GameObjectPool::shareGameObjectPools()
{
	if(!GameObjectPool::_gameObjectPools)
	{
		GameObjectPool::_gameObjectPools = CCArray::create();
		_gameObjectPools->retain();
	}
	return GameObjectPool::_gameObjectPools;
}
void GameObjectPool::releaseAllGameObjects(bool clean)
{
	for(unsigned index = 0; index < GameObjectPool::_gameObjectPools->count(); index++){
		GameObjectPool* gameObectPool =  
			dynamic_cast<GameObjectPool*>(GameObjectPool::_gameObjectPools->objectAtIndex(index));
		if(gameObectPool) gameObectPool->release();
	}
	GameObjectPool::_gameObjectPools->removeAllObjects();
	if(clean) CC_SAFE_RELEASE_NULL(GameObjectPool::_gameObjectPools);
}
void GameObjectPool::removeAllUnusedObjects()
{
	for(unsigned index = 0; index < GameObjectPool::_gameObjectPools->count(); index++){
		GameObjectPool* gameObectPool =  
			dynamic_cast<GameObjectPool*>(GameObjectPool::_gameObjectPools->objectAtIndex(index));
		if(gameObectPool) gameObectPool->removeUnusedObjects();
	}
}
void GameObjectPool::removeUnusedObjects()
{
	m_objects.removeAllObjects();
}
GameObjectPool::~GameObjectPool()
{
	//m_objectsInuse.removeAllObjects();
	//m_objects.removeAllObjects();
}
void GameObjectPool::makeObjects(unsigned int count)
{
	for(unsigned int i=0; i < count; i++){
		GameObject *object = constructObject();
		m_objects.addObject(object);
		object->release();
	}
}
bool GameObjectPool::retrieveObject(GameObject* object)
{
	if(m_objectsInuse.containsObject(object))
	{
		m_objectsInuse.fastRemoveObject(object);
		m_objects.addObject(object);
		return true;
	}
	return false;
}
bool GameObjectPool::retrieveObjectArray(CCArray& arr)
{
	bool ret = true;
	for(unsigned int i = 0; i < arr.count(); i++){
		GameObject* object = dynamic_cast<GameObject*>(arr.objectAtIndex(i));
		retrieveObject(object);
	}
	arr.removeAllObjects();
	return ret;
}
GameObject* GameObjectPool::getObject(CCDictionary* dict)
{
	if (0 == this->m_objects.count())
	{
		makeObjects(10);
	}
	GameObject *object = dynamic_cast<GameObject*>(m_objects.objectAtIndex(0));
	if(object->init(dict))
	{
		m_objectsInuse.addObject(object);
		m_objects.fastRemoveObjectAtIndex(0);
		return object;
	}
	return NULL;
}
bool GameObjectPool::getObjectArray(CCDictionary* dict, CCArray& ret)
{
	unsigned int count = dict->valueForKey("count")->intValue();
	count = (0 == count) ? 1 : count; 
	if(count > m_objects.count())
	{
		makeObjects(count);
	}
	for(unsigned int i=0 ;i < count; i++)
	{
		ret.addObject(getObject(dict));	
	}
	return true;
}
