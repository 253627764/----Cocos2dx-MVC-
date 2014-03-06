#include "LoopParallaxNode.h"

USING_NS_CC;

class CCPointObject : public CCObject
{
	CC_SYNTHESIZE(bool, m_loop, Loop)                                    
	CC_SYNTHESIZE(unsigned int, m_spriteCount, SpriteCount) //镜像数量
	CC_SYNTHESIZE(CCPoint, m_tRatio, Ratio)
	CC_SYNTHESIZE(CCPoint, m_tOffset, Offset)
	CC_SYNTHESIZE(CCNode *,m_pChild, Child)                       // weak ref
public:
	static CCPointObject * pointWithCCPoint(CCNode* child, CCPoint ratio, CCPoint offset, bool loop, unsigned int spriteCount)
	{
		CCPointObject *pRet = new CCPointObject();
		pRet->initWithCCPoint(child, ratio, offset, loop, spriteCount);
		pRet->autorelease();
		return pRet;
	}
	bool initWithCCPoint(CCNode* child, CCPoint ratio, CCPoint offset, bool loop, unsigned int spriteCount)
	{
		m_tRatio = ratio;
		m_tOffset = offset;
		m_pChild = child;
		m_loop = loop;
		m_spriteCount = spriteCount;
		return true;
	}
};
CCLoopParallaxNode * CCLoopParallaxNode::create(const char* szImageFile, int direct)
{
	CCLoopParallaxNode *pRet = new CCLoopParallaxNode();
	if(pRet && pRet->initWithFile(szImageFile, kDefaultSpriteBatchCapacity))
	{
		pRet->m_direct = direct;
		pRet->m_tLastPosition = CCPointMake(-100,-100);
		pRet->m_scale  = 1;
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}
void CCLoopParallaxNode::setNodeSize(CCNode* node)
{
	node->setAnchorPoint(CCPointZero);

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if (winSize.width < node->getContentSize().width)
	{
	    m_scale = winSize.width / node->getContentSize().width;
		node->setScale(m_scale);
		float x = node->getContentSize().width * m_scale;
		float y = node->getContentSize().height * m_scale;
		node->setContentSize(ccp(x,y));
	}
}
void CCLoopParallaxNode::addChild(const char* spriteFrameNameFormat, unsigned zOrder, const CCPoint& ratio, \
								  const CCPoint& offset, bool loop, unsigned spriteCount,\
								  int seedStart,int seedRange)
{
	CCPoint ratioPt2 =  ccp(2.0f,2.0f);
	int randseed = 0;
	for (unsigned index = 0;index < spriteCount; index++)
	{
		randseed = (rand() % seedRange) + seedStart;
		CCString* name = CCString::createWithFormat(spriteFrameNameFormat,randseed);
		addChild(name->getCString(), zOrder, ratio, offset, loop, spriteCount, index);	
	}
}
void CCLoopParallaxNode::addChild(const char* spriteFrameName, unsigned zOrder, const CCPoint& ratio, 
								  const CCPoint& offset, bool loop, unsigned spriteCount, unsigned index)
{
	CCSprite *child = NULL;
	CCPoint newOffset;

	child = CCSprite::createWithSpriteFrameName(spriteFrameName);

	switch(m_direct)
	{
	case kUp:
		newOffset = ccp(offset.x, offset.y - child->getContentSize().height * index * m_scale);
		break;
	case kDown:
		newOffset = ccp(offset.x , offset.y + child->getContentSize().height * index * m_scale);
		break;
	case kLeft:
		newOffset = ccp(offset.x + child->getContentSize().width * index * m_scale, offset.y);
		break;
	case kRight:
		newOffset = ccp(offset.x - child->getContentSize().width * index * m_scale, offset.y);
		break;
	}
	initNode(child, m_ParallaxArray, zOrder, child->getTag() ,ratio, newOffset, loop, spriteCount);
	setContentSize(child->getContentSize() * spriteCount);
}
void CCLoopParallaxNode::addChild(const char* spriteFrameName, unsigned zOrder, const CCPoint& ratio, \
	const CCPoint& offset, bool loop, unsigned int spriteCount)
{
	for (unsigned index = 0;index < spriteCount; index++)
		addChild(spriteFrameName, zOrder, ratio, offset, loop, spriteCount, index);
}
void CCLoopParallaxNode::initNode(CCNode* node, CCArray& nodeArr, unsigned zOrder, int tag, \
	const CCPoint& ratio, const CCPoint& offset, bool loop, unsigned int spriteCount)
{
	setNodeSize(node);
	//根据精灵结点对象、速率和偏移生成点对象
	CCPointObject *obj = CCPointObject::pointWithCCPoint(node, ratio, offset, loop, spriteCount);
	//将点对象添加到点对象数组中
	nodeArr.addObject(dynamic_cast<CCObject*>(obj));
	//初始化结点位置
	CCPoint pos = m_obPosition;
	pos.x = pos.x * ratio.x + offset.x;
	pos.y = pos.y * ratio.y + offset.y;
	node->setPosition(pos);
	CCSpriteBatchNode::addChild(node, zOrder, tag);
}
void CCLoopParallaxNode::removeChild(CCNode* child, bool cleanup)
{
	for( unsigned int i=0;i < m_ParallaxArray.count();i++){
		CCPointObject *point = dynamic_cast<CCPointObject *>(m_ParallaxArray.objectAtIndex(i));
		if( point->getChild()->isEqual(child)){
			for(unsigned int j = 0; j < point->getSpriteCount(); j++){
				point = dynamic_cast<CCPointObject *>(m_ParallaxArray.objectAtIndex(i+j));
				CCSpriteBatchNode::removeChild(point->getChild(), cleanup);
				m_ParallaxArray.removeObjectAtIndex(j,true);
			}			
		break;
		}
	}
}
void CCLoopParallaxNode::removeAllChildrenWithCleanup(bool cleanup)
{
	m_ParallaxArray.removeAllObjects();
	CCSpriteBatchNode::removeAllChildrenWithCleanup(cleanup);
}
void CCLoopParallaxNode::moveBy(unsigned step, float duration, bool repeat)
{
	CCPoint stepOffset;
	switch(m_direct)
	{
	case kUp:
		stepOffset = ccp(0, 1.0f) * step;
		break;
	case kDown:
		stepOffset = ccp(0, -1.0f) * step;
		break;
	case kLeft:
		stepOffset = ccp(-1.0f, 0) * step;
		break;
	case kRight:
		stepOffset = ccp(1.0f, 0) * step;
		break;
	}
	CCActionInterval* go    = CCMoveBy::create(duration, stepOffset);
	CCFiniteTimeAction* seq = CCSequence::create(go, NULL);
	if(repeat)
	{
		CCActionInterval * ai = CCRepeatForever::create((CCActionInterval*) seq);
		runAction(ai);
	}
	else runAction(seq);
}
void CCLoopParallaxNode::visit()
{	
	m_zeroWorldPosition = this->convertToWorldSpace(CCPointZero);
	if( ! m_zeroWorldPosition.equals(m_tLastPosition) )
	{
		for(unsigned int i=0; i < m_ParallaxArray.count(); i++ ) 
		{
			CCPointObject *point = dynamic_cast<CCPointObject*>(m_ParallaxArray.objectAtIndex(i));
			setNodePosition(point);
		}
		m_tLastPosition = m_zeroWorldPosition;
	}
	CCSpriteBatchNode::visit();
}
void CCLoopParallaxNode::setNodePosition(CCPointObject* point)
{
	CCNode * node = point->getChild();
	if (point->getLoop())
	{
		float nodeWidth   =  node->getContentSize().width ;
		float nodeHeight  =  node->getContentSize().height;
		float xPosition   =  convertToWorldSpace(node->getPosition()).x;
		float yPosition   =  convertToWorldSpace(node->getPosition()).y;
		float worldWidth  =  node->getParent()->getParent()->getContentSize().width;
		float worldHeight =  node->getParent()->getParent()->getContentSize().height;
		switch(m_direct)
		{
		case kUp:
			if(yPosition > worldHeight)  
				point->setOffset(ccpSub(point->getOffset(), ccp(0,node->getContentSize().height * point->getSpriteCount())));
			break;
		case kDown:
			if(yPosition < -nodeHeight) 
				point->setOffset(ccpAdd(point->getOffset(), ccp(0,node->getContentSize().height * point->getSpriteCount())));
			break;
		case kRight:
			if(xPosition > worldWidth)  
				point->setOffset(ccpSub(point->getOffset(), ccp(node->getContentSize().width * point->getSpriteCount(),0)));
			break;
		case kLeft:
			if(xPosition < -nodeWidth) 
				point->setOffset(ccpAdd(point->getOffset(), ccp(node->getContentSize().width * point->getSpriteCount(),0)));
			break;
		}
	}
	float x = -m_zeroWorldPosition.x + m_zeroWorldPosition.x * point->getRatio().x + point->getOffset().x;
	float y = -m_zeroWorldPosition.y + m_zeroWorldPosition.y * point->getRatio().y + point->getOffset().y;            
	node->setPosition(ccp(x,y));
}