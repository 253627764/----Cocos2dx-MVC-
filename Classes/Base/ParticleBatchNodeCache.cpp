#include "particleBatchNodeCache.h"

ParticleBatchNodeCache* ParticleBatchNodeCache::s_pSharedParticleBatchNodeCache = NULL;

ParticleBatchNodeCache* ParticleBatchNodeCache::sharedParticleBatchNodeCache()
{
	if (! s_pSharedParticleBatchNodeCache)
	{
		s_pSharedParticleBatchNodeCache = new ParticleBatchNodeCache();
		s_pSharedParticleBatchNodeCache->init();
	}
	return s_pSharedParticleBatchNodeCache;
}

void ParticleBatchNodeCache::purgeSharedParticleBatchNodeCache()
{
	CC_SAFE_RELEASE_NULL(s_pSharedParticleBatchNodeCache);
}

bool ParticleBatchNodeCache::init()
{
	m_pParticleBatchNodes = new CCDictionary();
	return true;
}

ParticleBatchNodeCache::ParticleBatchNodeCache()
	: m_pParticleBatchNodes(NULL)
{
}

ParticleBatchNodeCache::~ParticleBatchNodeCache()
{
	CC_SAFE_RELEASE(m_pParticleBatchNodes);
}

void ParticleBatchNodeCache::addParticleBatchNode(CCParticleBatchNode* node, const char * name)
{
	m_pParticleBatchNodes->setObject(node, name);
}

void ParticleBatchNodeCache::removeParticleBatchNodeByName(const char* name)
{
	if (! name)	return;
	m_pParticleBatchNodes->removeObjectForKey(name);
}

CCParticleBatchNode* ParticleBatchNodeCache::particleBatchNodeByName(const char* name)
{
	return (CCParticleBatchNode*)m_pParticleBatchNodes->objectForKey(name);
}