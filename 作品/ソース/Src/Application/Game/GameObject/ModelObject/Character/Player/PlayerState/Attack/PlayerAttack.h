#pragma once

#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/StateBase/StateBase.h"
class PlayerMove;

class PlayerAttack :public StateBase
{
public:
	virtual ~PlayerAttack() {};
	void Exit(Player& owner) override;
	virtual void Update(Player& owner);
	virtual void SetAnimation(Player& owner)override;

	void ChangeMove(Player& player);
	void PlaySwordSe(Player& owner);

	enum SwordAttack
	{
		SwordAttack1,
		SwordAttack2,
		SwordAttack3
	};

private:
	bool m_attackAnimotionContinueFlg = false;
	bool endAnimationFlg = false;
	bool m_AttackKeyFlg = false;
	bool m_attackFlg=false;
	int m_count = 0;

	SwordAttack m_attackState = SwordAttack1;
};