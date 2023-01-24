#pragma once

#include"System/GameObject/KdGameObject.h"

class Title :public KdGameObject
{
public:
	Title() {};
	~Title() override {};

	void Init() override;
	virtual void Update()override;
	virtual void DrawSprite() override;
	void SetAsset(std::shared_ptr<KdTexture>& sp_Tex,std::string_view fileName);
	
private:
	std::shared_ptr<KdTexture>	m_spTexture = nullptr;
	std::shared_ptr<KdTexture>	m_spStartTexture = nullptr;
	std::shared_ptr<KdTexture>	m_spRogoTexture = nullptr;
	std::shared_ptr<KdTexture>	m_spKeyTexture = nullptr;

	Math::Vector2 m_startTexPos;
	float m_alpha;
	float m_alphaSpd;
};