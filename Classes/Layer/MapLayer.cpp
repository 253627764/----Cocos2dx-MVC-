#include "MapLayer.h"

bool MapLayer::init()
{
	if(CCLayer::init())
	{
		srand((int)time(0));
		backgroundNode = CCLoopParallaxNode::create(BACKGROUND_IMAGE, kDown);
		
		const char* imageFormat = "%d.png";
		//三层背景，每层2列
		createChild(imageFormat, 3 ,ccp(1.0f,1.0f),2,1,false);
		createChild(imageFormat, 6, ccp(2.0f,2.0f),5,6,true);
		createChild(imageFormat, 4, ccp(3.0f,3.0f),11,4,true);
		backgroundNode->moveBy(100,1.0f,1);
		addChild(backgroundNode);
		setZOrder(-1);
		return true;
	}
	return false;
}
void MapLayer::createChild(const char* frameNameFormat, unsigned spriteCount, const CCPoint& ratioPt, 
							 unsigned seedStart, unsigned seekRange, bool rightAlignment)
{
	float width = 0;
	float winWidth = CCDirector::sharedDirector()->getWinSize().width;
	int randseed = (rand() % seekRange) + seedStart;
	const char* frameName = CCString::createWithFormat(frameNameFormat,randseed)->getCString();
	CCSprite* sprite = CCSprite::createWithSpriteFrameName(frameName);
	//左边背景
	backgroundNode->addChild(frameNameFormat, 0, ratioPt, CCPointZero, true, spriteCount, seedStart, seekRange);
	//右边背景
	if(rightAlignment) width = winWidth - sprite->boundingBox().size.width;
	else width = sprite->boundingBox().size.width;
	backgroundNode->addChild(frameNameFormat, 0, ratioPt, ccp(width,0), true, spriteCount, seedStart, seekRange);
}
