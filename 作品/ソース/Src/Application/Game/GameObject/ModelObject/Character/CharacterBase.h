#pragma once

#include"Application/Game/GameObject/ModelObject/KdModelObject.h"

class CharacterBase :public KdModelObject
{
public:
	CharacterBase() {}
	virtual ~CharacterBase() override {}

	void Update()override;

protected:

	float m_verticalMovement = 0;//垂直移動量

	// アニメーション
	KdAnimator m_animator;
};