#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class ParticleEffect :public EffectBase
{
public:
	ParticleEffect();
	~ParticleEffect()override {}

	void Update()override;
	void SetMatrix(const float size, const Math::Vector3& pos, const Math::Vector3& dir);

	float EaseOutCirc(float progress)
	{
		return (float)sqrt(1 - pow(progress - 1, 2));
	}

private:
	float m_progress = 0;		//êiíªìx
};