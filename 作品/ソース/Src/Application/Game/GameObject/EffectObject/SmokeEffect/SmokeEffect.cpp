#include "SmokeEffect.h"

ParticleSmoke::ParticleSmoke()
{
	m_lifeSpan = 60;	//1•b”ò‚Ô‚æ‚¤‚É
	m_speed = 0.5f;
}

void ParticleSmoke::Update()
{
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}

	//‘O•ûŒü
	Math::Vector3 forward = m_mWorld.Backward();
	forward.Normalize();

	m_mWorld *=
		Math::Matrix::CreateTranslation(forward * m_speed);

	UpdateTrail();
}

void ParticleSmoke::SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir)
{
	m_mWorld = Math::Matrix::CreateScale(1.1f) *
		Math::Matrix::CreateWorld(pos, -dir, Math::Vector3::Up);
}
