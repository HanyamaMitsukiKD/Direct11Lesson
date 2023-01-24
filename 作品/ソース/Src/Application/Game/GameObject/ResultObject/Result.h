#pragma once

#include"System/GameObject/KdGameObject.h"

class Result :public KdGameObject
{
public:
	Result() {};
	~Result() override {};

	void Init()override;
	virtual void Update()override;
	virtual void DrawSprite() override;
	void SetAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName);

private:
	std::shared_ptr<KdTexture>	m_spMessageTexture = nullptr;
	std::shared_ptr<KdTexture>	m_spTextTexture = nullptr;

	Math::Vector2 m_MessageTexPos;

	long recX;
	bool m_mailOpenFlg = false;

	// SE
	bool mailSE = false;
	bool messageSE = false;
};