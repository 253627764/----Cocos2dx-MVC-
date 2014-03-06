#ifndef __PARTSLAYER_H__
#define __PARTSLAYER_H__

#include "System/GameDefine.h"

class Mp;

class PartsLayer: public CCLayer
{
	CC_PROPERTY(int,m_score,Score)
	CC_PROPERTY(int,m_coin,Coin)
public:
	static PartsLayer* sharePartsLayer();
	~PartsLayer();
	virtual bool init();  
private:
	PartsLayer(){};
	void initPauseButton();
	void pauseCallback(CCObject* pSender);
	void initPlaneMp();
	void initCoinAni();
	void initCoinLabel();
	void initScore();
	void userPlaneHpChangeCallBack(CCObject *obj);
	void enemyPlaneDeathCallBack(CCObject *obj);
	

private:
	static PartsLayer* _partsLayer; 

	CCMenuItemImage* m_pPauseItem;
	Mp* m_pMp;
	CCSprite* m_pCoinAni;
	CCLabelAtlas* m_pCoinLabel;
	CCLabelAtlas* m_pScore;
};

#endif