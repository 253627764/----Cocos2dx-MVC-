#include "System/AppConfig.h"
#include "Plane/Plane.h"
#include "Layer/MapLayer.h"
#include "Layer/PartsLayer.h"
#include "Layer/UserPlaneLayer.h"
#include "Layer/EnemyPlaneLayer.h"
#include "Layer/GameWorldLayer.h"


GameWorldLayer* GameWorldLayer::_gameWorldLayer = NULL;

CCScene* GameWorldLayer::scene()
{
    CCScene *scene = CCScene::create(); 
	
	GameWorldLayer* gameWorld = shareGameWorldLayer();
	gameWorld->init();
	gameWorld->autorelease();

    scene->addChild(gameWorld);
    return scene;
}
GameWorldLayer* GameWorldLayer::shareGameWorldLayer()
{
	if(!_gameWorldLayer) _gameWorldLayer = new GameWorldLayer();
	return _gameWorldLayer;
}
bool GameWorldLayer::init()
{
    if ( !CCLayer::init() )
        return false;

	CCSpriteBatchNode* batchNode = AppConfig::shareAppConfig()->getGlobalBatchNode();
	addChild(batchNode,4);                        //全局批量渲染节点
	addChild(MapLayer::create(),-1);            //背景地图节点
	addChild(EnemyPlaneLayer::create(),1);      //敌机控制层
	addChild(UserPlaneLayer::create(),2);       //用户飞机控制层
	addChild(PartsLayer::sharePartsLayer(),3);           //杂项配件节点（按钮，积分，血量，金钱等）
    return true;
}
