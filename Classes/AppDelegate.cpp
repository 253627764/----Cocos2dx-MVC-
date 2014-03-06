#include "AppDelegate.h"
#include "Layer/GameWorldLayer.h"
#include "System/AppConfig.h"

USING_NS_CC;

AppDelegate::AppDelegate(){}

AppDelegate::~AppDelegate(){}

bool AppDelegate::applicationDidFinishLaunching() 
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);
    pDirector->setDisplayStats(true);
    pDirector->setAnimationInterval(1.0 / 60);

	//��ʼ���ֱ���ģʽ����Ƴߴ�
	AppConfig::shareAppConfig()->initResolutionMode(CCSizeMake(600, 800));

	//CCScene *pScene = TestLayer::scene();
    CCScene *pScene = GameWorldLayer::scene();
    pDirector->runWithScene(pScene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();

	AppConfig::removeAllUnusedCaches();
}

void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
	SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
