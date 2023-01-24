#include "KdAssetManager.h"

void KdAssetManager::Init()
{
	
	m_EffectAsset.Init();
	m_UIAsset.Init();
	m_ModelAsset.Init();
}

void KdAssetManager::Release()
{
	m_UIAsset.Release();
	m_EffectAsset.Release();
	m_ModelAsset.Release();
}
