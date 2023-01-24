#include "KdDebugManager.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/normalEnemy/normalEnemy.h"

void KdDebug::Update()
{
	if (GetAsyncKeyState(VK_TAB))
	{
		if (!m_pushedTab)
		{
			m_isActive = !m_isActive;		//���̃t���O�̋t��������Ƃ����Ӗ�
			//���ɂ����Ȃ�
			ShowCursor(m_isActive);
		}

		m_pushedTab = true;
	}
	else
	{
		m_pushedTab = false;
	}
}

void KdDebug::Init()
{
}

void KdDebug::Draw()
{
	PlayerDebugDraw();
	EnemyDebugDraw();

	if (ImGui::Begin(u8"�I�u�W�F�N�g�X�s�[�h"))
	{
		for (const std::shared_ptr<KdGameObject>& spObj :
			Application::Instance().GetGameSystem()->GetGameObjects())
		{
			if (!spObj) { return; }

			float a = spObj->GetObjectSpeed();
			ImGui::Text("%.7f", a);
		}
	}
	ImGui::End();
}

void KdDebug::PlayerDebugDraw()
{
	const std::shared_ptr<Player>& spPlayer =
		Application::Instance().GetGameSystem()->GetPlayer();

	if (!spPlayer) { return; }

	if (ImGui::Begin(u8"�v���C���["))
	{
		float hp = 0;
		ImGui::Text(u8"Player  ���W");
		Math::Vector3 pos = spPlayer->GetPos();
		ImGui::Text("x = %.3f y = %.3f z = %.3f", pos.x, pos.y, pos.z);


		ImGui::Text(u8"Player��� :");
		switch (spPlayer->GeteState())
		{
		case KdGameObject::eAvoidance:
			ImGui::Text(u8"���");
			break;

		case KdGameObject::eJustAvoidance:
			ImGui::Text(u8"�W���X�g���");
			break;


		case KdGameObject::eMove:
			ImGui::Text(u8"�ړ�");
			break;

		case KdGameObject::eDamage:
			ImGui::Text(u8"�_���[�W");
			break;

		case KdGameObject::eAttack:
			ImGui::Text(u8"�U��");
			break;

		case KdGameObject::eUlt:
			ImGui::Text(u8"�E���g");
			break;

		default:
			break;
		}

		if (spPlayer->GetAttackMotionFlg())
		{
			ImGui::Text(u8"�U�����t���O:true");
		}
		else
		{
			ImGui::Text(u8"�U�����t���O:false");
		}

		if (spPlayer->GetAnimotionContinueFlg())
		{
			ImGui::Text(u8"�U���p���t���O:true");
		}
		else
		{
			ImGui::Text(u8"�U���p���t���O:false");
		}

		ImGui::Text(u8"Player�U����� :");
		switch (spPlayer->GetAttackAnimationState())
		{
		case Player::eNoAnimationAttack:
			ImGui::Text("NOAttack");

			break;

		case Player::eAnimationAttack1:
			ImGui::Text("Attack1");
			break;


		case Player::eAnimationAttack2:
			ImGui::Text("Attack2");
			break;

		case Player::eAnimationAttack3:
			ImGui::Text("Attack3");

			break;

		default:
			break;
		}

		ImGui::Text(u8"PlayerSpd :%.3f", spPlayer->GetPlayerMoveSpd());
		ImGui::Text(u8"OutLineValue :%.3f", spPlayer->GetOutLineValue());
	}
	ImGui::End();
}

void KdDebug::EnemyDebugDraw()
{
	const std::shared_ptr<normalEnemy>& spEnemy =
		Application::Instance().GetGameSystem()->GetEnemy();

	if (!spEnemy) { return; }

	if (ImGui::Begin(u8"�G�l�~�["))
	{
		Math::Vector3 pos = Math::Vector3::Zero;
		ImGui::Text(u8"�G�l�~�[  ���W");
		if (!spEnemy)
		{
			ImGui::Text("x = %d", pos.x);
			ImGui::Text("y = %d", pos.y);
			ImGui::Text("z = %d", pos.z);
		}
		else
		{
			Math::Vector3 pos = spEnemy->GetPos();
			ImGui::Text("x = %.3f", pos.x);
			ImGui::Text("y = %.3f", pos.y);
			ImGui::Text("z = %.3f", pos.z);
		}

		ImGui::Text(u8"�G�l�~�[  HP:%d", spEnemy->GetHP());
		ImGui::Text(u8"�X�s�[�h: %.3f", spEnemy->GetMoveSpd());
	}
	ImGui::End();
}
