#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class ThunderEffect :public EffectBase
{
public:
	ThunderEffect();
	~ThunderEffect()override {}

	void Update()override;
	void SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir)override;


	void SetPlayerMat(const Math::Matrix& mat)
	{
		m_playerMat = mat;
	}

private:
	Math::Matrix m_playerMat;

	Math::Matrix m_Trans;
	Math::Matrix m_Rotation;

	Math::Vector3 rotationDir;
	int m_lifeSpanMax;
};