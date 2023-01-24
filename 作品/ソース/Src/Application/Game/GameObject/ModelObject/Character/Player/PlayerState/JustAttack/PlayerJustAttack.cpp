#include "PlayerJustAttack.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Move/PlayerMove.h"

void PlayerJustAttack::Exit(Player& owner)
{
	owner.SetState(KdGameObject::eState::eJustAvoidance);
}

void PlayerJustAttack::Update(Player& owner)
{
	// à⁄ìÆèàóù
	Math::Vector3 nowPos = owner.GetPos();

	owner.QTECheck();

	owner.UpdateMatrix(nowPos);

	AnimationEndCheck(owner);
}

void PlayerJustAttack::SetAnimation(Player& owner)
{
	owner.SetNextAnimation("NearJustCounter", false);
}