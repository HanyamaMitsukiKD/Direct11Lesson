#include "PlayerDamage.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"

void PlayerDamage::Exit(Player& owner)
{
	owner.SetState(KdGameObject::eState::eDamage);
}

void PlayerDamage::Update(Player& owner)
{
	// à⁄ìÆèàóù
	Math::Vector3 vMove;
	Math::Vector3 nowPos = owner.GetPos();

	owner.UpdateDamageMove(vMove, nowPos);

	owner.UpdateMatrix(nowPos);

	AnimationEndCheck(owner);
}

void PlayerDamage::SetAnimation(Player& owner)
{
	owner.SetNextAnimation("Damage", false);
}