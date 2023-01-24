#include "UltEffect.h"

UltEffect::UltEffect()
{
	m_lifeSpan = 180;
	m_speed = 0.8f;
}

void UltEffect::Update()
{
	// ÇQïbåoÇ¡ÇΩÇÁè¡Ç∑
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}

	//ëOï˚å¸
	Math::Vector3 forward;


	// 10
	int randNum = rand() % 11;


	if (randNum > 5)
	{
		forward = rotationDir;
	}

	if (randNum > 7)
	{
		Math::Vector3 randDir;

		randDir.x += (rand() % 200 - 100) * 0.0005f;
		randDir.y += (rand() % 200 - 100) * 0.0005f;
		randDir.z += (rand() % 200 - 100) * 0.0005f;

		randDir.Normalize();

		forward = randDir;
	}

	if (randNum < 5)
	{
		forward = m_mWorld.Backward();
	}

	//í∑Ç≥ÇPÇ…Ç∑ÇÈÅiê≥ãKâªÅj
	forward.Normalize();

	m_mWorld *=
		Math::Matrix::CreateTranslation(forward * m_speed);

	UpdateTrail();
}

void UltEffect::SetMatrix(const float size, const float zpos, const Math::Vector3& pos, const Math::Vector3& dir)
{
	m_mWorld = Math::Matrix::CreateScale(size) *
		Math::Matrix::CreateWorld(pos, -dir, Math::Vector3::Up);

	rotationDir.x += (rand() % 200 - 100) * 0.0005f;
	rotationDir.y += (rand() % 200 - 100) * 0.0005f;
	rotationDir.z += (rand() % 200 - 100) * 0.0005f;

	rotationDir.Normalize();

	m_lifeSpanMax = m_lifeSpan;
}

void UltEffect::DrawBright()
{
	DrawEffect();
}
