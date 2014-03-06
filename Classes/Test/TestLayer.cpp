#include "TestLayer.h"

CCScene* TestLayer::scene()
{
    CCScene *scene = CCScene::create();
    TestLayer *layer = TestLayer::create();
    scene->addChild(layer);
    return scene;
}

CCSprite* pSprite = NULL;
// on "init" you need to initialize your instance
bool TestLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(TestLayer::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello World", "Thonburi", 34);

    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    // position the label on the center of the screen
    pLabel->setPosition( ccp(size.width / 2, size.height - 20) );

    // add the label as a child to this layer
    this->addChild(pLabel, 1);
    
    
    pSprite = CCSprite::create("Icon-72.png");
    pSprite->setPosition( ccp(size.width/2, 0) );
    //pSprite->setPosition( ccp(size.width/2, 0) );
    this->addChild(pSprite, 0);
    CCLOG("%f,%f",pSprite->getPosition().x,pSprite->getPosition().y);
    //SineBy* pSineBy = SineBy::create(1, ccp(0,180), 66);
    //CCRepeat* pRepeat = CCRepeat::create(pSineBy, 2);
    //pSprite->runAction(pRepeat);
    
    //CircleBy* pCircleBy1 = CircleBy::create(3, ccp(-200,0), 100);
    //pCircleBy1->setClock(false);
    //BWCircleBy* pCircleBy2 = BWCircleBy::create(3, ccp(200,0), 100);
    //pCircleBy2->setClock(true);
    //BWCircleBy* pCircleBy3 = BWCircleBy::create(3, ccp(0,200), 100);
    //pCircleBy2->setClock(true);
    //BWCircleBy* pCircleBy4 = BWCircleBy::create(3, ccp(0,-200), 100);
    //pCircleBy2->setClock(true);
    //CCSequence* pSeq = CCSequence::create(pCircleBy1,pCircleBy2,pCircleBy3,pCircleBy4,NULL);
    
    //CCRepeat* pRepeat = CCRepeat::create(pSeq, 2);
    //pSprite->runAction(pCircleBy1);

    
    schedule(schedule_selector(TestLayer::pushPoint));
    return true;
}

void TestLayer::pushPoint(float delta)
{
    m_vTouchPoint.push_back(pSprite->getPosition());
    
    return;
}

void TestLayer::draw()
{
    ccPointSize(5);
	ccDrawColor4B(0,0,255,128);
    for(unsigned int i=0;i<m_vTouchPoint.size();i++)
    {
        CCPoint pos = m_vTouchPoint[i];
        ccDrawPoint(pos);
    }
}

void TestLayer::menuCloseCallback(CCObject* pSender)
{
    //CCDirector::sharedDirector()->end();
	CCLOG("%f,%f",pSprite->getPosition().x,pSprite->getPosition().y);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
