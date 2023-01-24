#pragma once


#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class UltEffect :public EffectBase
{
public:
	UltEffect();
	~UltEffect()override {}

	void Update()override;
	void SetMatrix(const float size, const float zpos, const Math::Vector3& pos, const Math::Vector3& dir);

	void SetPlayerMat(const Math::Matrix& mat)
	{
		m_playerMat = mat;
	}

	void SetPoly(std::shared_ptr<KdTrailPolygon> Poly)
	{
		m_spPoly = Poly;
	}


private:
	Math::Matrix m_playerMat;

	Math::Matrix m_Trans;
	Math::Matrix m_Rotation;

	Math::Vector3 rotationDir;
	int m_lifeSpanMax;
};