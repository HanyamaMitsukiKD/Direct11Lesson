#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class SwordEffect :public EffectBase
{
public:
	SwordEffect();
	~SwordEffect()override {}

	void DrawNoLighting() override;

	void Update(Math::Vector3& startPos, Math::Vector3& endPos, Math::Matrix& mat);
	void SetMatrix();

private:
	float m_BetweenDir;
};