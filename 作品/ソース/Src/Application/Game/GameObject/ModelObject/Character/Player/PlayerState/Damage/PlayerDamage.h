#pragma once

#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/StateBase/StateBase.h"
class PlayerMove;

class PlayerDamage :public StateBase
{
public:
	virtual ~PlayerDamage() {};
	void Exit(Player& owner) override;
	virtual void Update(Player& owner);
	virtual void SetAnimation(Player& owner)override;


private:

};