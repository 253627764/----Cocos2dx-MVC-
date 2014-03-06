#ifndef _MAP_LAYER_H__
#define _MAP_LAYER_H__

#include "System/GameDefine.h"
#include "Extension/LoopParallaxNode.h"

//背景地图类
class MapLayer: public CCLayer
{
public:
	virtual bool init();
	CREATE_FUNC(MapLayer)
	
private:
	CCLoopParallaxNode * backgroundNode;  //节点循环滚动类
private:
	void createChild(const char* frameName, unsigned spriteCount, const CCPoint& ratioPt, 
		unsigned seedStart, unsigned seekRange, bool rightAlignment);
};
#endif