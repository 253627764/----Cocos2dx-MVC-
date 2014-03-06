#ifndef __MP_H__
#define __MP_H__


#include "System/GameDefine.h"
#include "Extension/LoopParallaxNode.h"


class CCLoopParallaxNode;
//战机血量类
class Mp : public CCNode
{
	CC_SYNTHESIZE_READONLY(int, m_maxMp, MaxMp)                               //进度最大值
	CC_SYNTHESIZE_READONLY(int, m_mp, Mp)                                     //当前血量
	CC_SYNTHESIZE_READONLY(float, m_mpWidth, MpWidth)                         //血量的宽度
	CC_SYNTHESIZE_READONLY(int, m_mpSampleCount, MpSampleCount)               //mp样本精灵数量
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize, m_mpSampleSize, MpSampleSize)  //mp样本精灵宽度
	CC_SYNTHESIZE_READONLY(float, m_oneMp, OneMp)                             //单元长度
public:
	static Mp* create(int maxMp,float mpWidth);
	Mp(int maxMp,float mpWidth);

	virtual bool init(); 
	virtual void visit();                              
public:
	void increaseMp(int value);   //增加血量
	void reduceMp(int value);     //减少血量
	void setMp(int value);        //设置血量
private:	
	CCLoopParallaxNode * bnodeMp; //循环节点
private:
	bool getMpSampleInfo(const char* szMpImageFile);
};

#endif