#ifndef __GamePlane__IntervalAction__
#define __GamePlane__IntervalAction__

#include "cocos2d.h"

USING_NS_CC;
/*
    自定义定时行为
*/
class ActionInterval : public CCActionInterval
{
	CC_SYNTHESIZE(bool, m_bRotateTarget, RotateTarget)
protected:
	CCPoint m_startPosition;
protected:
	void adjustTargetRotate(const CCPoint& dirPoint); 
	virtual void adjustTargetPosition(float time){};
	virtual CCObject* copyWithZone(CCZone* pZone);
	virtual void update(float time);
};
/*
  SIN正弦曲线
*/
class SineBy : public ActionInterval
{
public:
	static SineBy* create(float t, CCPoint endPoint,float fA);
    bool init(float t, CCPoint endPoint,float fA);
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    
    virtual CCActionInterval* reverse(void);
protected:
	virtual void adjustTargetPosition(float delta);    
protected:
	CCPoint m_endPosition;
    float   m_fW;                                              //2*PI周期的X步长
    float   m_fA;                                              //振幅
};
/*
  圆形
*/
class CircleBy : public ActionInterval
{
	CC_SYNTHESIZE(bool, m_bClock, Clock)
public:
	static CircleBy* create(float t, CCPoint dirPoint,float fB);
    bool init(float t, CCPoint dirPoint,float fB);

    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual CCActionInterval* reverse(void);
protected:
	virtual void adjustTargetPosition(float delta);
protected:
    CCPoint m_dirPoint;
    float   m_fA;
    float   m_fB;
};

/*
   实现椭圆运动轨迹
   需要空间位置（中心点坐标）、长半轴（a）、和短半轴（b）（或者半焦距（c））三个量确定椭圆
*/
typedef struct _ellipseConfig {    // 定义一个结构来包含确定椭圆的参数 
	CCPoint centerPosition;        //中心点坐标
	float aLength;                 //椭圆a长，三角斜边
	float cLength;                 //椭圆c长，三角底边  
} ellipseConfig;  

class EllipseTo : public ActionInterval  
{  
public:  
	static EllipseTo *create(float t, const ellipseConfig& c);   
	bool init(float t, const ellipseConfig& c);  

	//继承自父类 
	virtual void adjustTargetPosition(float time);
	virtual CCObject* copyWithZone(CCZone* pZone);
protected:  
	ellipseConfig m_sConfig;  

};
#endif