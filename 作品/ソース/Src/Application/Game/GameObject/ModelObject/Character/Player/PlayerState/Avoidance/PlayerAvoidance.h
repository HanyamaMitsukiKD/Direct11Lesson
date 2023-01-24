#pragma once

#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/StateBase/StateBase.h"
class PlayerMove;

class PlayerAvoidance :public StateBase
{
public:
	virtual ~PlayerAvoidance() {};
	void Exit(Player& owner) override;
	virtual void Update(Player& owner);
	void SetAnimation(Player& owner)override;

	
private:

};