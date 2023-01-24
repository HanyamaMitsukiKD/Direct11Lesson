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
			m_isActive = !m_isActive;		//今のフラグの逆を代入するという意味
			//閉じにくくなる
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

	if (ImGui::Begin(u8"オブジェクトスピード"))
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

	if (ImGui::Begin(u8"プレイヤー"))
	{
		float hp = 0;
		ImGui::Text(u8"Player  座標");
		Math::Vector3 pos = spPlayer->GetPos();
		ImGui::Text("x = %.3f y = %.3f z = %.3f", pos.x, pos.y, pos.z);


		ImGui::Text(u8"Player状態 :");
		switch (spPlayer->GeteState())
		{
		case KdGameObject::eAvoidance:
			ImGui::Text(u8"回避");
			break;

		case KdGameObject::eJustAvoidance:
			ImGui::Text(u8"ジャスト回避");
			break;


		case KdGameObject::eMove:
			ImGui::Text(u8"移動");
			break;

		case KdGameObject::eDamage:
			ImGui::Text(u8"ダメージ");
			break;

		case KdGameObject::eAttack:
			ImGui::Text(u8"攻撃");
			break;

		case KdGameObject::eUlt:
			ImGui::Text(u8"ウルト");
			break;

		default:
			break;
		}

		if (spPlayer->GetAttackMotionFlg())
		{
			ImGui::Text(u8"攻撃中フラグ:true");
		}
		else
		{
			ImGui::Text(u8"攻撃中フラグ:false");
		}

		if (spPlayer->GetAnimotionContinueFlg())
		{
			ImGui::Text(u8"攻撃継続フラグ:true");
		}
		else
		{
			ImGui::Text(u8"攻撃継続フラグ:false");
		}

		ImGui::Text(u8"Player攻撃状態 :");
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

	if (ImGui::Begin(u8"エネミー"))
	{
		Math::Vector3 pos = Math::Vector3::Zero;
		ImGui::Text(u8"エネミー  座標");
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

		ImGui::Text(u8"エネミー  HP:%d", spEnemy->GetHP());
		ImGui::Text(u8"スピード: %.3f", spEnemy->GetMoveSpd());
	}
	ImGui::End();
}
