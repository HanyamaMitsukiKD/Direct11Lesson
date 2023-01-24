#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class ParticleSmoke :public EffectBase
{
public:
	ParticleSmoke();
	~ParticleSmoke()override {}

	void Update()override;
	void SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir)override;

	void DrawNoLighting() override;

private:


};