#include "AttackEffect.h"

Attack::Attack()
{
	m_lifeSpan = 30;	//‚Q•b”ò‚Ô‚æ‚¤‚É
	m_speed = 1.0f;
}

void Attack::Update()
{
	// ‚Q•bŒo‚Á‚½‚çÁ‚·
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}


	//‘O•ûŒü
	Math::Vector3 forward = m_mWorld.Backward();

	//’·‚³‚P‚É‚·‚éi³‹K‰»j
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