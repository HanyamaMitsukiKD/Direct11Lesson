#pragma once

#include"Application/Game/GameObject/ModelObject/KdModelObject.h"

class CharacterBase :public KdModelObject
{
public:
	CharacterBase() {}
	virtual ~CharacterBase() override {}

	void Update()override;

protected:

	float m_verticalMovement = 0;//�����ړ���

	// �A�j���[�V����
	KdAnimator m_animator;
};