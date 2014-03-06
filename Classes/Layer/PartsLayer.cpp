#include "Layer/PartsLayer.h"
#include "Parts/Mp.h"
#include "Plane/Plane.h"
#include "Layer/EnemyPlaneLayer.h"
#include "Base/ParticleBatchNodeCache.h"

bool PartsLayer::init()
{
	if ( !CCLayer::init() ) return false;
	initPauseButton();
	initPlaneMp();
	initCoinAni();
	initCoinLabel();
	initScore();

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(PartsLayer::userPlaneHpChangeCallBack),USERPLANE_HP_CHANGE_MSG,NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(PartsLayer::enemyPlaneDeathCallBack),ENEMYPLANE_HP_CHANGE_MSG,NULL);
	
	return true;
}

PartsLayer* PartsLayer::_partsLayer = NULL; 

PartsLayer* PartsLayer::sharePartsLayer()
{
	if(!PartsLayer::_partsLayer)
	{
		PartsLayer::_partsLayer = new PartsLayer();
		_partsLayer->init();
	}
	return PartsLayer::_partsLayer;
}

PartsLayer::~PartsLayer()
{
	_partsLayer = NULL;
}

void PartsLayer::userPlaneHpChangeCallBack(CCObject *obj)
{
	HpObject* userPlane = dynamic_cast<HpObject*>(obj);
	if(userPlane){
		int hp = userPlane->getHp();
		if(hp) m_pMp->setMp(hp);
		else m_pMp->setVisible(false);
	}
}
void PartsLayer::enemyPlaneDeathCallBack(CCObject *obj)
{
	EnemyPlane* enemyPlane = dynamic_cast<EnemyPlane*>(obj);
	if(enemyPlane && enemyPlane->getHp() <= 0){
		int score = getScore() + enemyPlane->getScore();
		setScore(score);
	}
}
void PartsLayer::initPauseButton()
{	
	m_pPauseItem = CCMenuItemImage::create();
	m_pPauseItem->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(PAUSE_IMAGE));
	m_pPauseItem->setTarget(this,menu_selector(PartsLayer::pauseCallback));
	CCPoint pos = ccp(m_pPauseItem->getContentSize().width / 2,
		this->getContentSize().height-m_pPauseItem->getContentSize().height / 2);
	m_pPauseItem->setPosition(pos);
	m_pPauseItem->setScale(0.7f);
	CCMenu* pMenu = CCMenu::create(m_pPauseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu,256);
}
void PartsLayer::pauseCallback(CCObject* pSender)
{
	//AppConfig::shareAppConfig()->purgeGameObjects();
	//AppConfig::shareAppConfig()->purgeShareAppConfig();

	const char* name = "userPlane/hit.plist";
	CCParticleBatchNode* partileBatchNode = ParticleBatchNodeCache::
		sharedParticleBatchNodeCache()->particleBatchNodeByName(name);

	AppConfig* config = AppConfig::shareAppConfig();
	CCParticleSystemQuad* particle = config->createParticle(name,partileBatchNode);
	particle->setAutoRemoveOnFinish(true);
	particle->setPosition(ccp(100,100));
	if(partileBatchNode->getParent() != this)
	{
		partileBatchNode->removeFromParent();
		addChild(partileBatchNode);
	}


	//static int hp = 600;
	//m_pMp->setMp(hp);
	//hp-=100;
	//EnemyPlaneLayer::staticenemyPlane->setHp(0);
	//UserPlane * up = UserPlane::shareUserPlane();
	//int angle = -180;
	//angle = (angle + 1) % 360;
	//up->setHp(10);
	//static float angle = up->getRotation();
	//angle += 10;
	//float rad = CC_DEGREES_TO_RADIANS(angle);
	//up->setRotation(angle);
	/*
	CCPoint startPt, endPt;
	startPt = up->getPosition();
	CCLog("ptx:%f,pty:%f",startPt.x,startPt.y);

	float distance = startPt.y / cosf(CC_DEGREES_TO_RADIANS(-150));
	endPt = startPt + ccp(0,-distance);
	CCLog("ptx:%f,pty:%f",endPt.x,endPt.y);

    endPt = endPt.rotateByAngle(startPt,CC_DEGREES_TO_RADIANS(-150));
	CCLog("ptx:%f,pty:%f",endPt.x,endPt.y);
	CCMoveTo* mb = CCMoveTo::create(0,endPt);
	up->runAction(mb);
	*/


//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
//	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
//#else
//	CCDirector::sharedDirector()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	exit(0);
//#endif
//#endif

}
void PartsLayer::initPlaneMp()
{
	int userPlaneHp = UserPlane::shareUserPlane()->getMaxHp();
	float mp_width = getContentSize().width - m_pPauseItem->getContentSize().width * 1.2;
	m_pMp = Mp::create(userPlaneHp,mp_width);

	CCPoint pos= ccp(m_pPauseItem->getContentSize().width * 1.2, 
		getContentSize().height - (m_pMp->getMpSampleSize().height / 2) - m_pPauseItem->getContentSize().height / 2);
	m_pMp->setPosition(pos);

	addChild(m_pMp);
}
void PartsLayer::initCoinAni()
{
	//金币动画
	CCAnimation* pAnimation = CCAnimationCache::sharedAnimationCache()->animationByName(COIN_ANIMATE_NAME);
	CCAnimate* pAnimate = CCAnimate::create(pAnimation);
	pAnimate->setDuration(1.0f);
	//初始化动画精灵
	CCObject* obj = pAnimation->getFrames()->objectAtIndex(0);
	CCSpriteFrame* frame = dynamic_cast<CCAnimationFrame *>(obj)->getSpriteFrame();
	m_pCoinAni = CCSprite::createWithSpriteFrame(frame);
	m_pCoinAni->runAction(CCRepeatForever::create(pAnimate));

	float scale = 0.6f;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint pos = ccp(m_pCoinAni->getContentSize().width / 2 * scale + 2\
		,size.height - m_pPauseItem->getContentSize().height * 1.5);

	m_pCoinAni->setPosition(pos);
	m_pCoinAni->setScale(scale);
	addChild(m_pCoinAni);
}
void PartsLayer::initCoinLabel()
{
	m_pCoinLabel = CCLabelAtlas::create("0",NUM_FONT_IMAGE,FONT_WIDTH,FONT_HEIGHT,'0');
	if(m_pCoinLabel)
	{
		float scale = 0.25f;		
		CCPoint offPos = ccp(m_pCoinLabel->boundingBox().size.width /2 * scale + 4 , 
			-m_pCoinLabel->boundingBox().size.height / 2 * scale);
		CCPoint pos = ccpAdd(m_pCoinAni->getPosition(), offPos);
		m_pCoinLabel->setPosition(pos);
		m_pCoinLabel->setScale(scale);
		addChild(m_pCoinLabel);
	}
}
void PartsLayer::initScore()
{
	float scale = 0.35f;
	m_pScore = CCLabelAtlas::create("0", NUM_FONT_IMAGE,FONT_WIDTH, FONT_HEIGHT, '0');
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint pos = ccp(size.width - m_pScore->getContentSize().width * scale - 20,\
		size.height - m_pPauseItem->getContentSize().height * 0.7f - m_pScore->getContentSize().height * 0.4f);
	
	m_pScore->setPosition(pos);
	m_pScore->setScale(scale);
	setScore(0);
	addChild(m_pScore);
}
int PartsLayer::getScore()
{
	return m_score;
}
void PartsLayer::setScore(int var)
{
	CCString str;
	str.initWithFormat("%d",var);
	m_pScore->setString(str.getCString());

	float scale = 0.35f;
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint pos = ccp(size.width - m_pScore->getContentSize().width * scale - 20,\
		size.height - m_pPauseItem->getContentSize().height * 0.7f - m_pScore->getContentSize().height * 0.4f);

	m_pScore->setPosition(pos);
	m_score = var;
}
void PartsLayer::setCoin(int val)
{
	m_coin = val;
	CCString str;
	str.initWithFormat("%d",val);
	m_pCoinLabel->setString(str.getCString());
}
int PartsLayer::getCoin()
{
	return m_coin;
}
