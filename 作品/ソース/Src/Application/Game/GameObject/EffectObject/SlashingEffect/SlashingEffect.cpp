#include "SlashingEffect.h"

SlashingEffect::SlashingEffect()
{
	m_lifeSpan = 30;	//ÇQïbîÚÇ‘ÇÊÇ§Ç…
	m_speed = 0.5f;
}

void SlashingEffect::Update(Math::Vector3& Pos)
{
	// ÇQïbåoÇ¡ÇΩÇÁè¡Ç∑
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}

	Math::Vector3 pos = Pos;
	
	Math::Matrix local = Math::Matrix::CreateTranslation(pos);
	Math::Matrix scale = Math::Matrix::CreateScale(0.5);

	m_mWorld = scale * local * m_Trans;

	UpdateTrail();
}

void SlashingEffect::SetMatrixS(const Math::Vector3& pos)
{
	m_Trans = Math::Matrix::CreateTranslation(pos);
}

void SlashingEffect::DrawBright()
{
	DrawEffect();
}
