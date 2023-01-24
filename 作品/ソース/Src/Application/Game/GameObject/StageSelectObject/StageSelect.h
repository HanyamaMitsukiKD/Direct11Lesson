#pragma once

#include"System/GameObject/KdGameObject.h"

class StageSelect :public KdGameObject
{
public:
	StageSelect() {};
	~StageSelect() override {};

	virtual void Update()override {};
	virtual void DrawSprite() override;
	void SetAsset(std::string_view fileName) override;

private:
	std::shared_ptr<KdTexture>	m_spTexture = nullptr;
};