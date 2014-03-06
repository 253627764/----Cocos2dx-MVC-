#ifndef  _APP_CONFIG_H_
#define  _APP_CONFIG_H_

#include "System\GameDefine.h"

class AppConfig : public CCObject
{
public:
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize,m_frameSize,FrameSize)            //屏幕分辨率
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize,m_winSize,WinSize)                //设计窗口尺寸
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize,m_visibleSize,VisibleSize)        //可视尺寸
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCPoint,m_visibleOrigin,VisibleOrigin)   //可视起始点
	CC_SYNTHESIZE_READONLY_PASS_BY_REF(CCSize,m_lsSize,LsSize)                  //设计尺寸
	//批渲染处理节点
	CC_SYNTHESIZE_RETAIN(CCSpriteBatchNode*,m_globalBatchNode,GlobalBatchNode)
	
public:
	static AppConfig* shareAppConfig();
	
	AppConfig();
	~AppConfig();
	
	void initResolutionMode(const CCSize& lsSize);  //用设计尺寸初始化分辨率模式
	void initCache();
	//返回世界空间范围
	const CCRect& getWorldBoundBox();

	//清理无用缓存
	static void removeAllUnusedCaches();
	//清理自身
	static void purgeShareAppConfig();
	//清理游戏对象
	static void purgeGameObjects();
	//清理所有缓存
	static void purgeAllCaches();

	//初始化粒子批渲染节点及缓存
	static void initParticleBatchNodes();
	//生成粒子效果
	static CCParticleSystemQuad* createParticle(const char* plistFileName, CCParticleBatchNode* node);
	//生成指定数量粒子结点的批渲染节点
	static CCParticleBatchNode* createParticleBatchNode(const char * plistFileName, unsigned particleNodeCount);

	//初始化动画
	static void initAnimations();
	//创建动画及动画缓存
	static bool createAnimation(const char* aniName, const char* frameFormat, int startIndex, int endIndex);
	//通过动画名生成动画行为
	static CCFiniteTimeAction* getActionWithAnimate(const char* animateName, float duration);

	//通过距离，开始位置，角度得到目标坐标 
	static CCPoint getTargetPosition(const CCPoint& startPoint, float distance, float angle);
	//获得其它结点转换后的空间位置
	static CCPoint getTransformPosition(CCNode* node ,CCPoint& position, CCNode* transformNode);

	//打印点信息
	static void printPoint(const CCPoint& point);
	//打印尺寸信息
	static void printSize(const CCSize& size);
	//打印矩形信息
	static void printRect(const CCRect& rect);
private:
	static AppConfig* _appConfig; 
	CCRect m_worldBoundBox;
};

#endif