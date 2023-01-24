#pragma once

#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/StateBase/StateBase.h"

class PlayerMove:public StateBase
{
public:
	virtual ~PlayerMove() {};
	void Exit(Player& owner) override;
	virtual void Update(Player& owner);

	void SetAnimation(Player& owner)override;

private:

	bool spaseFlg = false;
	bool m_moveFlg = false;
	bool m_attackClickFlg = false;
};