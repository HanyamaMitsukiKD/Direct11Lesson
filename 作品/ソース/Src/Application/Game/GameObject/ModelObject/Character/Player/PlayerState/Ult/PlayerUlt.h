#pragma once

#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/StateBase/StateBase.h"
class PlayerMove;

class PlayerUlt :public StateBase
{
public:
	virtual ~PlayerUlt() {};
	void Exit(Player& owner) override;
	virtual void Update(Player& owner);
	virtual void SetAnimation(Player& owner)override;


private:

};