#include "StateBase.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Move/PlayerMove.h"

void StateBase::AnimationEndCheck(Player& owner)
{
	bool animotionContinueFlg = owner.GetAnimationEndFlg();
	if (!animotionContinueFlg) { return; }

	owner.ChangeActionState(std::make_shared<PlayerMove>());
}
