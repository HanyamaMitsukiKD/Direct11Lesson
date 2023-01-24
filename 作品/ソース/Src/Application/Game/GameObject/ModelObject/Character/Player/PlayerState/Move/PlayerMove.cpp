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
	// �ړ�����
	Math::Vector3 vMove;
	Math::Vector3 nowPos = owner.GetPos();

	//�ړ��̍X�V
	owner.UpdateMove(vMove, nowPos);

	if (owner.GetMoveFlg())
	{
		Math::Vector3 avoidanceDir = owner.GetMatrix().Backward();
		owner.UpdateRotate(avoidanceDir, vMove);
	}

	// ����A�N�V���������邩�ǂ���
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (!spaseFlg)
		{
			//����J�n
			owner.ChangeActionState(std::make_shared<PlayerAvoidance>());
			KdGameAudioManager::GetInstance().avoidanceSE();	//�W�����vSE��炷	
			owner.SetAvoidanceDir(owner.GetMatrix().Backward());
			spaseFlg = true;

			//�W���X�g������ǂ���
			if (owner.JustAvoidanceFlg())
			{
				//�W���X�g��������J�n
				owner.ChangeActionState(std::make_shared<PlayerJustAvoidance>());
				owner.QTEExit();
			}

		}
	}
	else
	{
		spaseFlg = false;
	}

	//���U��
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		// ���U���J�n
		// �W���X�g������ǂ�������
		bool justCounterFlg = owner.JustCounterFlg();

		if (justCounterFlg)
		{
			// �W���X�g�J�E���^�[
			owner.ChangeActionState(std::make_shared<PlayerJustAttack>());
		}
		else
		{
			owner.ChangeActionState(std::make_shared<PlayerAttack>());
		}
	}

	// �_���[�W����
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