#include "PlayerMove.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Avoidance/PlayerAvoidance.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Attack/PlayerAttack.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/JustAttack/PlayerJustAttack.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/JustAvoidance/PlayerJustAvoidance.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Damage/PlayerDamage.h"

void PlayerMove::Exit(Player& owner)
{
	owner.SetState(KdGameObject::eState::eMove);
	spaseFlg = false;
}

void PlayerMove::Update(Player& owner)
{
	// 移動処理
	Math::Vector3 vMove;
	Math::Vector3 nowPos = owner.GetPos();

	//移動の更新
	owner.UpdateMove(vMove, nowPos);

	if (owner.GetMoveFlg())
	{
		Math::Vector3 avoidanceDir = owner.GetMatrix().Backward();
		owner.UpdateRotate(avoidanceDir, vMove);
	}

	// 回避アクションをするかどうか
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (!spaseFlg)
		{
			//回避開始
			owner.ChangeActionState(std::make_shared<PlayerAvoidance>());
			KdGameAudioManager::GetInstance().avoidanceSE();	//ジャンプSEを鳴らす	
			owner.SetAvoidanceDir(owner.GetMatrix().Backward());
			spaseFlg = true;

			//ジャスト回避かどうか
			if (owner.JustAvoidanceFlg())
			{
				//ジャスト回避処理開始
				owner.ChangeActionState(std::make_shared<PlayerJustAvoidance>());
				owner.QTEExit();
			}

		}
	}
	else
	{
		spaseFlg = false;
	}

	//剣攻撃
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		// 剣攻撃開始
		// ジャスト回避かどうか判定
		bool justCounterFlg = owner.JustCounterFlg();

		if (justCounterFlg)
		{
			// ジャストカウンター
			owner.ChangeActionState(std::make_shared<PlayerJustAttack>());
		}
		else
		{
			owner.ChangeActionState(std::make_shared<PlayerAttack>());
		}
	}

	// ダメージ判定
	if (owner.GeteState() == KdGameObject::eState::eDamage)
	{
		owner.ChangeActionState(std::make_shared<PlayerDamage>());
	}

	owner.UpdateMatrix(nowPos);
}

void PlayerMove::SetAnimation(Player& owner)
{
	if (owner.GetMoveFlg())
	{
		owner.SetNextAnimation("Move", true);
	}
	else
	{
		owner.SetNextAnimation("StandBY", true);
	}
}