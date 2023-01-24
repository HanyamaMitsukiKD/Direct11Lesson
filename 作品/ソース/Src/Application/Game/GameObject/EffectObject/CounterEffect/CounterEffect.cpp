#include "CounterEffect.h"
#include"Application/Game/KdGameSystem.h"
#include"Application/main.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"

Counter::Counter()
{
	m_lifeSpan = 70;	//1秒飛ぶように
	m_speed = 0.5f;
}

void Counter::Update()
{
	// ２秒経ったら消す
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}

	std::shared_ptr<Player>	sp_player=Application::Instance().GetGameSystem()->GetPlayer();
	Math::Vector3 playerPos = sp_player->GetPos();

	if (!ultFlg)
	{
		// ジャスト回避なら
		// 公転
		//原点（親惑星地点）からどれだけ離れた位置にいるか
		// 公転
		m_playerMat = Math::Matrix::CreateScale(0.5) * Math::Matrix::CreateTranslation(playerPos);

		
		Math::Vector3 transPos = m_Trans.Translation();
		transPos.y += 0.1f;
		m_Trans.Translation(transPos);
		m_Trans *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(15.0f));
		
		m_mWorld = m_Trans * m_playerMat;
	}
	else
	{
		// Ultなら
		zpos += 0.5f;

		Math::Matrix trans = Math::Matrix::CreateTranslation(Math::Vector3(0, 0, zpos));
		m_Trans *= Math::Matrix::CreateRotationY(DirectX::XMConvertToRadians(30.0f));
		m_mWorld = trans * m_Trans * m_playerMat;
	}

	UpdateTrail();
}

void Counter::SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir)
{
	m_mWorld = Math::Matrix::CreateScale(0.1f) *
		Math::Matrix::CreateWorld(pos, -dir, Math::Vector3::Up);

	m_Trans = Math::Matrix::CreateTranslation(Math::Vector3(0, 0, -5));
	m_playerMat = Math::Matrix::CreateScale(0.1f) * Math::Matrix::CreateTranslation(pos);

	zpos = -5;
}

void Counter::DrawBright()
{
	DrawEffect();
}
