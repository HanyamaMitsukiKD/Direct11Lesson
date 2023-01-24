#include "ParticleEffect.h"

ParticleEffect::ParticleEffect()
{
	m_lifeSpan = 180;
	m_speed = 0.8f;
}

void ParticleEffect::Update()
{
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

void ParticleEffect::SetMatrix(const float size, const Math::Vector3& pos, const Math::Vector3& dir)
{
	m_mWorld = Math::Matrix::CreateScale(size) *
		Math::Matrix::CreateWorld(pos, -dir, Math::Vector3::Up);
}
