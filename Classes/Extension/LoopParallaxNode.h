#ifndef __LOOPPARALLAXNODE_H__   
#define __LOOPPARALLAXNODE_H__ 

#include "cocos2d.h"

/*
	  作用：背景滚动类，允许指定精灵数量按指定方向排列并进行运行（可无限循环运动），已优化使用ccspriteBatchNode
	  作者：陈文杰
	  时间：2014-02-21
*/
USING_NS_CC;


enum
{
   kUp    =   1,
   kDown  =   2,
   kLeft  =   4,
   kRight =   8,
};

class CCPointObject;

class  CCLoopParallaxNode: public CCSpriteBatchNode 
{
    /** array that holds the offset / ratio of the children */
    CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCArray, m_ParallaxArray, ParallaxArray)  //精灵数组 
	CC_SYNTHESIZE(int, m_direct, Direct)                                         //移动方向
	CC_SYNTHESIZE_READONLY(float, m_scale, Scale)                                //缩放比例
public:

	/*
		作用：创建对象，使用该对象必须将先将批渲染节点相关图像的plist文件先读入桢缓存
		参数：szImageFile 图像文件名
			  direct      方向(kup,kDown,kLeft,kRight)
	*/
    static CCLoopParallaxNode * create(const char* szImageFile, int direct);
	/*
		作用：增加孩子
		参数：spriteFrameName 桢名
		      zOrder          层次序
			  ratio           速率
			  offset          初始偏移
			  loop			  是否循环
			  spriteCount     精灵数量
	*/
    void addChild(const char* spriteFrameName, unsigned zOrder, const CCPoint& ratio, const CCPoint& offset,\
		bool loop, unsigned spriteCount);
	/*
		作用：根据桢名格式与种子参数得到桢名，通过桢名及其它参数初始化孩子并增加孩子
		参数：spriteFrameName 桢名格式
		      zOrder          层次序
			  ratio           速率
			  offset          初始偏移
			  loop			  是否循环
			  spriteCount     精灵数量
			  seedStart       种子起始数
			  seedRange       种子范围

	*/
	void addChild(const char* formatSpriteFrameName, unsigned z, const CCPoint& ratio, \
		const CCPoint& offset, bool loop, unsigned spriteCount,int seedStart,int seedRange);
	
	//开始运动，可指定是否循环
	void moveBy(unsigned int step, float duration, bool repeat);
	//继承CCNode
	virtual void removeChild(CCNode* child, bool cleanup);
    virtual void removeAllChildrenWithCleanup(bool cleanup);
    virtual void visit(void);
private:
	CCPoint m_zeroWorldPosition;         //原点世界坐标
	CCPoint m_tLastPosition;             //最后一次的移动位置
private:
	//初始化节点
	void initNode(CCNode* node, CCArray& nodeArr, unsigned zOrder, int tag, 
		const CCPoint& ratio,const CCPoint& offset, bool loop, unsigned int spriteCount);
	//设置节点位置
	void setNodePosition(CCPointObject*);
	//设置节点尺寸
	void setNodeSize(CCNode* node);
	//增加单独一个孩子
	void addChild(const char* spriteFrameName, unsigned zOrder, const CCPoint& ratio, 
		const CCPoint& offset, bool loop, unsigned spriteCount, unsigned index); 
};
#endif
