#include "ActionInterval.h"

/*
  CustomActionInterval
*/
void ActionInterval::adjustTargetRotate(const CCPoint& dirPoint)
{
	if(m_bRotateTarget)
	{
		float fRadian = ((dirPoint.x > 0) && (dirPoint.y != 0)) ? \
			M_PI_2 - atanf(dirPoint.y / dirPoint.x) : -M_PI_2 - atanf(dirPoint.y / dirPoint.x);
		m_pTarget->setRotation(CC_RADIANS_TO_DEGREES(fRadian));
	}
}
void ActionInterval::update(float time)
{
	if (m_pTarget)
	{
		CCPoint curTargetPos = m_pTarget->getPosition();
		adjustTargetPosition(time);
		CCPoint newTargetPos = m_pTarget->getPosition();
		CCPoint dirPoint = ccpSub(newTargetPos, curTargetPos);
		adjustTargetRotate(dirPoint);
	}
}

CCObject* ActionInterval::copyWithZone( CCZone* pZone )
{
	CCZone* pNewZone = NULL;
	ActionInterval* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		pCopy = (SineBy*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new ActionInterval();
		pZone = pNewZone = new CCZone(pCopy);
	}

	CCActionInterval::copyWithZone(pZone);

	pCopy->m_bRotateTarget = m_bRotateTarget;

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

/*
  SineBy
*/
SineBy* SineBy::create(float t, CCPoint endPoint,float fA)
{
    SineBy *pSineBy = new SineBy();
    pSineBy->init(t, endPoint,fA);
	pSineBy->setRotateTarget(true);
    pSineBy->autorelease();
    return pSineBy;
}
bool SineBy::init(float t, CCPoint endPoint,float fA)
{
    if (ActionInterval::initWithDuration(t))
    {
        m_endPosition = endPoint;
        m_fA = fA;
        return true;
    }
    return false;
}
void SineBy::startWithTarget(CCNode *pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
    m_fW = 2 * M_PI / ccpLength(m_endPosition);
}
CCObject* SineBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    SineBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        pCopy = (SineBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new SineBy();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    ActionInterval::copyWithZone(pZone);
    
    pCopy->m_endPosition = m_endPosition;
	pCopy->m_fA = m_fA;

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}
void SineBy::adjustTargetPosition(float time)
{
	float fX = ccpLength(m_endPosition) * time;
	float fY = m_fA * sinf(m_fW * fX);
	//float fC = sqrtf(fX * fX + fY * fY);

	//float fRadian = (m_endPosition.x < 0) ? \
	//	atanf(fY / fX) + atanf(m_endPosition.y / m_endPosition.x) + float(M_PI) :\
	//	atanf(fY / fX) + atanf(m_endPosition.y / m_endPosition.x);
	//float fRadian = ccpAngleSigned(m_endPosition, CCPointMake(-1, 0)); 
	double fRadian =  M_PI_2;

	//CCPoint targetPos = ccp(fC * cos(fRadian) + m_startPosition.x, fC * sin(fRadian) + m_startPosition.y);
    CCPoint targetPos = ccpAdd(m_startPosition, ccp(fX,fY)).rotateByAngle(m_startPosition, fRadian);

	m_pTarget->setPosition(targetPos);
}
CCActionInterval* SineBy::reverse(void)
{
    SineBy *pAction = SineBy::create(m_fDuration, m_endPosition,m_fA);
    return pAction;
}

/*
	CircleBy
*/
CircleBy* CircleBy::create(float t, CCPoint dirPoint,float fB)
{
    CircleBy *pCircleBy = new CircleBy();
    pCircleBy->init(t, dirPoint,fB);
	pCircleBy->setRotateTarget(true);
    pCircleBy->autorelease();
    
    return pCircleBy;
}
bool CircleBy::init(float t, CCPoint dirPoint,float fB)
{
    if (ActionInterval::initWithDuration(t))
    {
        m_dirPoint = dirPoint;
        m_fA = ccpLength(dirPoint);
        m_fB = fB;
        m_bClock = false;
        return true;
    }
    return false;
}
void CircleBy::startWithTarget(CCNode *pTarget)
{
    ActionInterval::startWithTarget(pTarget);
    m_startPosition = pTarget->getPosition();
    float fDistance = ccpLength(m_dirPoint);
    m_fA = fDistance * 0.5;
}
CCObject* CircleBy::copyWithZone(CCZone *pZone)
{
    CCZone* pNewZone = NULL;
    CircleBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        pCopy = (CircleBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CircleBy();
        pZone = pNewZone = new CCZone(pCopy);
    }
    
    ActionInterval::copyWithZone(pZone);
    
	pCopy->m_dirPoint = m_dirPoint;
	pCopy->m_fB = m_fB;
	pCopy->m_bClock = m_bClock;

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}
void CircleBy::adjustTargetPosition(float time)
{
	time = m_bClock ? (1 - time) : time;
	float fRadianMin = (m_dirPoint.x > 0) ? -M_PI : 0;
	float fRadianMax = 2 * float(M_PI);

	float fRadian = fRadianMin + fRadianMax * time;

	float fX = (m_dirPoint.x > 0) ? m_fA * cosf(fRadian) + m_fA : m_fA * cosf(fRadian) - m_fA;
	float fY = m_fB * sinf(fRadian);      
	float fC = sqrtf(powf(fX, 2)+powf(fY, 2));

	float fRotRadian = (fX < 0) ? \
		atanf(fY / fX) + atanf(m_dirPoint.y / m_dirPoint.x) + M_PI : \
		atanf(fY / fX) + atanf(m_dirPoint.y / m_dirPoint.x);

	CCPoint targetPosition = ccp( fC * cos(fRotRadian) + m_startPosition.x,\
		fC * sin(fRotRadian) + m_startPosition.y);
	m_pTarget->setPosition(targetPosition);
}
CCActionInterval* CircleBy::reverse(void)
{
    CircleBy *pAction = CircleBy::create(m_fDuration, m_dirPoint,m_fB);
	pAction->setClock(!m_bClock);
    return pAction;
}
/*
	EllipseTo
*/
EllipseTo * EllipseTo::create( float t, const ellipseConfig& c )
{
	EllipseTo *pEllipseTo = new EllipseTo();  
	pEllipseTo->init(t, c);  
	pEllipseTo->autorelease();  

	return pEllipseTo;  
}
bool EllipseTo::init( float t, const ellipseConfig& c )
{
	if (ActionInterval::initWithDuration(t))  
	{  
		m_sConfig = c;  
		return true;  
	}  
	return false;  
}
void EllipseTo::adjustTargetPosition( float time )
{
	float x = -m_sConfig.aLength * cos(2 * M_PI * time);
	float y = sqrt(powf(m_sConfig.aLength, 2) - powf(m_sConfig.cLength, 2)) \
		* sin(2 * M_PI * time);
	m_pTarget->setPosition(ccpAdd(m_sConfig.centerPosition, ccp(x, y)));  
}
CCObject* EllipseTo::copyWithZone( CCZone* pZone )
{
	CCZone* pNewZone = NULL;
	EllipseTo* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		pCopy = (EllipseTo*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new EllipseTo();
		pZone = pNewZone = new CCZone(pCopy);
	}
	ActionInterval::copyWithZone(pZone);
	pCopy->m_sConfig = m_sConfig;

	CC_SAFE_DELETE(pNewZone);
	return pCopy;
}

