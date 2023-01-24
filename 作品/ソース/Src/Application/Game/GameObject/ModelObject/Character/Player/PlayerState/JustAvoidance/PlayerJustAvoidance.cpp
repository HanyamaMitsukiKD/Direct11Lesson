#include "PlayerJustAvoidance.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Move/PlayerMove.h"

void PlayerJustAvoidance::Exit(Player& owner)
{
	owner.SetState(KdGameObject::eState::eJustAvoidance);
}

void PlayerJustAvoidance::Update(Player& owner)
{
	// à⁄ìÆèàóù
	Math::Vector3 vMove;
	Math::Vector3 nowPos = owner.GetPos();

	//à⁄ìÆ
	owner.UpdateJustAvoidance(vMove, nowPos);

	owner.QTECheck();

	owner.UpdateMatrix(nowPos);

	AnimationEndCheck(owner);
}

void PlayerJustAvoidance::SetAnimation(Player& owner)
{
	owner.SetNextAnimation("Avoidance", false);
}