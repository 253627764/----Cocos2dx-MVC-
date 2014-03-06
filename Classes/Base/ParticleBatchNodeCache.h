#ifndef __PARTICLE_BATCH_NODE_H__
#define __PARTICLE_BATCH_NODE_H__

#include "System/GameDefine.h"

class ParticleBatchNodeCache: public CCObject
{
public:
    ParticleBatchNodeCache();
    ~ParticleBatchNodeCache();
	bool init();
    static ParticleBatchNodeCache* sharedParticleBatchNodeCache();
    static void purgeSharedParticleBatchNodeCache();
    void addParticleBatchNode(CCParticleBatchNode* node, const char * name);
    void removeParticleBatchNodeByName(const char* name);
    CCParticleBatchNode* particleBatchNodeByName(const char* name);   
private:
    CCDictionary* m_pParticleBatchNodes;
    static ParticleBatchNodeCache* s_pSharedParticleBatchNodeCache;
};

#endif 