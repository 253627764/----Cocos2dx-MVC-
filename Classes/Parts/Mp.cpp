#include "Mp.h"

Mp* Mp::create(int maxMp,float mpWidth)
{
	Mp *pRet = new Mp(maxMp,mpWidth);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}
Mp::Mp(int maxMp,float mpWidth)
{
	m_mpSampleCount =  0;
	m_mpSampleSize  =  CCSizeZero;
	m_maxMp         =  maxMp;
	m_mp            =  m_maxMp;		
	m_mpWidth       =  mpWidth;
	m_oneMp         =  m_mpWidth / m_maxMp;
}
bool Mp::init()
{
	
	CCNode::init();
	if(getMpSampleInfo(HP_IMAGE))
	{
		//初始化循环节点类
		// TO DO : GLOBAL_IMAGE加载了没有必要加载的文件，可考虑替换为进度类
		bnodeMp = CCLoopParallaxNode::create(GLOBAL_IMAGE, kLeft);
		bnodeMp->addChild(HP_IMAGE, 0, ccp(1.0f,1.0f), ccp(0,0),0,m_mpSampleCount);
		addChild(bnodeMp);
		
		setContentSize(bnodeMp->getContentSize());     //同步尺寸
		return true;
	}
	return false;
}
bool Mp::getMpSampleInfo(const char* szMpImageFile)
{
	CCSprite* csMpSample = CCSprite::createWithSpriteFrameName(szMpImageFile);
	if(csMpSample)
	{
		m_mpSampleSize = CCSizeMake(csMpSample->getContentSize().width, csMpSample->getContentSize().height);
		m_mpSampleCount = m_mpWidth / m_mpSampleSize.width;
		return true;
	}
	return false;
}
void Mp::setMp(int value)
{
	value = (value > m_maxMp) ? m_maxMp : (value < 0) ? 0 : value;
	int changeMp = 0;
	if(value > m_mp){
		bnodeMp->setDirect(kRight);
		changeMp = value - m_mp;
	}
	else if(value < m_mp){
		bnodeMp->setDirect(kLeft);	
		changeMp = m_mp - value;
	}
	bnodeMp->moveBy(m_oneMp * changeMp,1.0f,0);
	m_mp = value;
}
void Mp::increaseMp(int value)
{
	int mp = ((m_mp + value) > m_maxMp) ? m_maxMp : m_mp + value;
	int diff = mp - m_mp;
	if(diff > 0)
	{
		bnodeMp->setDirect(kRight);	
		bnodeMp->moveBy(m_oneMp * value,1.0f,false);
		m_mp = mp;
	}
}
void Mp::reduceMp(int value)
{
	int mp = ((m_mp - value) < 0) ? 0 : m_mp - value;
	int diff = m_mp - mp;
	if(diff > 0)
	{
		bnodeMp->setDirect(kLeft);	
		bnodeMp->moveBy(m_oneMp * value,1.0f,0);
		m_mp = mp;
	}
}
void Mp::visit()
{
	glEnable(GL_SCISSOR_TEST);  
	CCPoint point = convertToWorldSpace(CCPointZero);
	CCDirector::sharedDirector()->getOpenGLView()->setScissorInPoints(point.x, point.y, getContentSize().width, getContentSize().height);
	CCNode::visit();
	glDisable(GL_SCISSOR_TEST);
}