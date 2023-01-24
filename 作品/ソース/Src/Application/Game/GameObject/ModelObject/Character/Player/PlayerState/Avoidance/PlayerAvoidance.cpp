#include "PlayerAvoidance.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Move/PlayerMove.h"

void PlayerAvoidance::Exit(Player& owner)
{
	owner.SetState(KdGameObject::eState::eAvoidance);
	owner.SetTrueMoveFlg();
}

void PlayerAvoidance::Update(Player& owner)
{
	// à⁄ìÆèàóù
	Math::Vector3 vMove;
	Math::Vector3 nowPos = owner.GetPos();

	owner.UpdateAvoidance(vMove, nowPos);

	owner.UpdateMatrix(nowPos);

	AnimationEndCheck(owner);
}

void PlayerAvoidance::SetAnimation(Player& owner)
{
	owner.SetNextAnimation("Avoidance", false);
}