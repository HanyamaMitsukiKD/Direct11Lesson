#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class Counter :public EffectBase
{
public:
	Counter();
	~Counter()override {}

	void Update()override;
	void SetMatrixS(const float z,const float size,const Math::Vector3& pos, const Math::Vector3& dir);
	void SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir)override;


	void SetPlayerMat(const Math::Matrix& mat)
	{
		m_playerMat = mat;
	}

	void SetUltFlg(bool flg)
	{
		ultFlg = flg;
	}

private:
	bool ultFlg = false;
	float zpos = 0;

	Math::Matrix m_playerMat;

	Math::Matrix m_Trans;
	Math::Matrix m_Rotation;
};