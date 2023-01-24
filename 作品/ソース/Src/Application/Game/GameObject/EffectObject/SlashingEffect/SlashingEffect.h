#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class SlashingEffect :public EffectBase
{
public:
	SlashingEffect();
	~SlashingEffect()override {}

	void Update(Math::Vector3& Pos)override;
	void SetMatrixS(const Math::Vector3& pos);

	void DrawBright() override;

private:
	Math::Matrix m_Trans;
};