#include "PlayerUlt.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Move/PlayerMove.h"

void PlayerUlt::Exit(Player& owner)
{
	owner.SetState(KdGameObject::eState::eUlt);
}

void PlayerUlt::Update(Player& owner)
{
	owner.SetObjectSpeed(0.7f);
	owner.UpdateUltAttackEffect();

	AnimationEndCheck(owner);
}

void PlayerUlt::SetAnimation(Player& owner)
{
	owner.SetNextAnimation("Ult", false);
}
