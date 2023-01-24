#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class Attack :public EffectBase
{
public:
	Attack();
	~Attack()override {}

	void Update()override;
	void SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir)override;

	void DrawBright() override;

private:
	
};