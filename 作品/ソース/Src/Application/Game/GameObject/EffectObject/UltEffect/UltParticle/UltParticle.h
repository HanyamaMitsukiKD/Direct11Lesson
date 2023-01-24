#pragma once

#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"

class UltParticle :public EffectBase
{
public:
	UltParticle();
	~UltParticle()override {}

	void Update()override;
	void SetMatrix(const float size, const Math::Vector3& pos, const Math::Vector3& dir);
	void DrawNoLighting()override;

	void SetModel(std::shared_ptr<KdModelData>& Model);


	float EaseOutCirc(float progress)
	{
		return (float)sqrt(1 - pow(progress - 1, 2));
	}

private:
	float m_progress = 0;		//�i���x

	Math::Vector3 m_startPos;	// �J�n�n�_
	Math::Vector3 m_endPos;		// �I���n�_
};