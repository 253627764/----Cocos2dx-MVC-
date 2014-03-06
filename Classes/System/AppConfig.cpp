#include "cocos2d.h"
#include "AppConfig.h"
#include "Bullte/Bullte.h"
#include "Layer/GameWorldLayer.h"
#include "Plane/Plane.h"
#include "Base/ParticleBatchNodeCache.h"

AppConfig* AppConfig::_appConfig = NULL; 

AppConfig* AppConfig::shareAppConfig()
{
	if(NULL == AppConfig::_appConfig)
	{
		AppConfig::_appConfig = new AppConfig();
	}
	return AppConfig::_appConfig;
}
AppConfig::AppConfig():m_globalBatchNode(NULL)
{
	initCache();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	m_worldBoundBox = CCRectMake(0,0,winSize.width,winSize.height);
	//SimpleAudioEngine::sharedEngine()->preloadEffect(USERPLANE_DEFAULT_EMIT_SE_FILE); //预加载发射音效
}

AppConfig::~AppConfig()
{	
	CC_SAFE_RELEASE(m_globalBatchNode);
	AppConfig::_appConfig = NULL;
}

void AppConfig::initResolutionMode(const CCSize& lsSize)
{
	m_frameSize = CCEGLView::sharedOpenGLView()->getFrameSize();

	m_lsSize = lsSize;
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(m_lsSize.width ,m_lsSize.height, kResolutionShowAll);

	m_winSize = CCDirector::sharedDirector()->getWinSize();
	m_visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	m_visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCLog("FrameSize: width[%f], height[%f]", m_frameSize.width,m_frameSize.height);
	CCLog("WinSize: width[%f], height[%f]",m_winSize.width, m_winSize.height);
	CCLog("VisibleSize: width[%f], height[%f]", m_visibleSize.width,m_visibleSize.height);
	CCLog("VisibleOrigin: x[%f], y[%f]", m_visibleOrigin.x, m_visibleOrigin.y);

}

CCFiniteTimeAction* AppConfig::getActionWithAnimate(const char* animateName, float duration)
{
	CCAnimation* pAnimation = CCAnimationCache::sharedAnimationCache()->animationByName(animateName);
	if(pAnimation){
		CCAnimate* pAnimate = CCAnimate::create(pAnimation);
		pAnimate->setDuration(duration);
		pAnimation->setRestoreOriginalFrame(false);
		return pAnimate;
	}
	return NULL;
}

void AppConfig::initCache()
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(BACKGROUND_PLIST);
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(GLOBAL_PLIST);
	setGlobalBatchNode(CCSpriteBatchNode::create(GLOBAL_IMAGE));
	initAnimations();
	initParticleBatchNodes();
}

void AppConfig::removeAllUnusedCaches() 
{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();  
	CCTextureCache::sharedTextureCache()->removeUnusedTextures(); 
	GameObjectPool::removeAllUnusedObjects();
}

void AppConfig::purgeAllCaches() 
{
	CCAnimationCache::purgeSharedAnimationCache();  
	CCSpriteFrameCache::purgeSharedSpriteFrameCache();
	CCTextureCache::purgeSharedTextureCache();
	ParticleBatchNodeCache::purgeSharedParticleBatchNodeCache();
}

void AppConfig::purgeShareAppConfig()
{
	CC_SAFE_RELEASE_NULL(_appConfig);
}

void AppConfig::purgeGameObjects()
{
	FlyActionFactory::releaseFlyActionFactorys();
	GameObjectPool::releaseAllGameObjects(true);
	UserPlane::shareUserPlane()->release();
}

void AppConfig::initParticleBatchNodes()
{
	ParticleBatchNodeCache* cache = ParticleBatchNodeCache::sharedParticleBatchNodeCache();
	CCParticleBatchNode* node = AppConfig::createParticleBatchNode(NORMAL_HIT_PARTICLE_PLIST,0);
	if(node)
		cache->addParticleBatchNode(node,NORMAL_HIT_PARTICLE_PLIST);
}

CCParticleBatchNode* AppConfig::createParticleBatchNode(const char * plistFileName, 
														unsigned particleNodeCount)
{
	CCParticleSystemQuad* particle = CCParticleSystemQuad::create(plistFileName);	
	CCParticleBatchNode* particleBatchNode = CCParticleBatchNode::createWithTexture(particle->getTexture());
	for(unsigned index = 0; index < particleNodeCount; index++)
		createParticle(plistFileName,particleBatchNode);
	return particleBatchNode;
}

CCParticleSystemQuad* AppConfig::createParticle(const char* plistFileName, 
												CCParticleBatchNode* node)
{
	CCParticleSystemQuad* particle = CCParticleSystemQuad::create(plistFileName);
	if(node) node->addChild(particle);
	return particle;
}

bool AppConfig::createAnimation(const char* aniName, const char* frameFormat, 
								int startIndex, int endIndex)
{	
	char szImageFileName[128] = {0};
	CCAnimation* pAnimation = CCAnimation::create();
	if(pAnimation)
	{
		////创建动画信息类并从缓存中读取动画信息
		CCSpriteFrameCache *fc = CCSpriteFrameCache::sharedSpriteFrameCache();
		for (int i = startIndex; i <= endIndex; i++)
		{
			sprintf(szImageFileName, frameFormat, i);
			pAnimation->addSpriteFrame(fc->spriteFrameByName(szImageFileName));
		}
		pAnimation->setDelayPerUnit(0.2f);//必须设置否则不会动态播放
		pAnimation->setRestoreOriginalFrame(true);//是否回到第一帧 
		//循环次数 -1无限循环 cocos2d Bug:CCSpriteBatchNode中不可使用，使用CCRepeatForever
		//pAnimation->setLoops(-1);
		CCAnimationCache::sharedAnimationCache()->addAnimation(pAnimation,aniName);
		return true;
	}
	return false;
}

void AppConfig::initAnimations()
{
	createAnimation(COIN_ANIMATE_NAME,COIN_ANIMATE_FORMAT,1,36);
	createAnimation(USERPLANE_DEATH_ANIMATE_NAME,USERPLANE_DEATH_ANIMATE_FORMAT,1,13);
	createAnimation(ENEMYPLANE_DEATH_ANIMATE_NAME,ENEMYPLANE_DEATH_ANIMATE_FORMAT,1,12);
}

CCPoint AppConfig::getTransformPosition(CCNode* node ,CCPoint& position, CCNode* transformNode)
{	
	CCPoint worldPoint = node->convertToWorldSpace(position);
	CCPoint transformPoint = transformNode->convertToNodeSpace(worldPoint);
	return transformPoint;
}

CCPoint AppConfig::getTargetPosition(const CCPoint& startPoint, float distance, float angle)
{
	CCPoint targetPosition = startPoint + ccp(0,distance);
	targetPosition = targetPosition.rotateByAngle(startPoint,CC_DEGREES_TO_RADIANS(angle));
	return targetPosition;
}

const CCRect& AppConfig::getWorldBoundBox()
{
	return m_worldBoundBox;
}

void AppConfig::printPoint(const CCPoint& point)
{
	CCLOG("CCPoint Info, X:%f, Y:%f", point.x, point.y);
}

void AppConfig::printSize(const CCSize& size)
{
	CCLOG("CCSize Info, width:%f, height:%f", size.width, size.height);
}

void AppConfig::printRect(const CCRect& rect)
{
	
	CCLOG("CCRect Info, origin.x:%f, origin.y:%f, size.width:%f, size.height:%f",
		rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
}