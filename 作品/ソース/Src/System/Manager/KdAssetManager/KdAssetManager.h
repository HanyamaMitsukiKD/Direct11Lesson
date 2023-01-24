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
	KdAssetEffect	m_EffectAsset;		// �G�t�F�N�g
	KdAssetModel	m_ModelAsset;		// ���f��

	void Init();

private:
	KdAssetManager() {}
	~KdAssetManager() { Release(); }

	void Release();
};