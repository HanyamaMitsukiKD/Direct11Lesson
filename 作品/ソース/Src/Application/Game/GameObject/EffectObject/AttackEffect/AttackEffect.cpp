#include "AttackEffect.h"

Attack::Attack()
{
	m_lifeSpan = 30;	//２秒飛ぶように
	m_speed = 1.0f;
}

void Attack::Update()
{
	// ２秒経ったら消す
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}


	//前方向
	Math::Vector3 forward = m_mWorld.Backward();

	//長さ１にする（正規化）
	forward.Normalize();

	m_mWorld *=
		Math::Matrix::CreateTranslation(forward * m_speed);

	UpdateTrail();
}

void Attack::SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir)
{
	m_mWorld = Math::Matrix::CreateScale(0.1f) *
		Math::Matrix::CreateWorld(pos, -dir, Math::Vector3::Up);
}