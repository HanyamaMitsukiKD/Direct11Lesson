#pragma once

#include"System/Manager/KdAssetManager/KdAssetUI/KdAssetUI.h"
#include"System/Manager/KdAssetManager/KdAssetEffect/KdAssetEffect.h"
#include"System/Manager/KdAssetManager/KdAssetModel/KdAssetModel.h"

class KdAssetManager
{
public:

	static KdAssetManager& GetInstance()
	{
		static KdAssetManager instance;
		return instance;
	}

	KdAssetUI		m_UIAsset;			// UI
	KdAssetEffect	m_EffectAsset;		// エフェクト
	KdAssetModel	m_ModelAsset;		// モデル

	void Init();

private:
	KdAssetManager() {}
	~KdAssetManager() { Release(); }

	void Release();
};