#pragma once

class Player;

class StateBase
{
public:
	virtual ~StateBase() {};
	virtual void Enter(Player& owner) {}
	virtual void Exit(Player& owner) {}
	virtual void Update(Player& owner) = 0;
	virtual void SetAnimation(Player& owner){};

	void AnimationEndCheck(Player& owner);

private:

};