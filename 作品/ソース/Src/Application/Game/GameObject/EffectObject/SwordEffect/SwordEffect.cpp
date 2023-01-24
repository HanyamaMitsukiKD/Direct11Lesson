#include "SwordEffect.h"

SwordEffect::SwordEffect()
{
	m_lifeSpan = 30;	
	m_speed = 0.5f;
}

void SwordEffect::DrawNoLighting()
{
	DrawEffect();
}

void SwordEffect::Update(Math::Vector3& startPos, Math::Vector3& endPos,Math::Matrix& mat)
{
	// ２秒経ったら消す
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}

	// スタートとエンドの間をランダムで取得
	Math::Vector3 effectPos = startPos - endPos;
	effectPos.Normalize();
	
	Math::Matrix local = Math::Matrix::CreateTranslation(effectPos * m_BetweenDir);
	Math::Matrix scale = Math::Matrix::CreateScale(0.1f);

	Math::Matrix swordMat = mat;

	m_mWorld = scale * swordMat * local;

	UpdateTrail();
}

void SwordEffect::SetMatrix()
{
	m_BetweenDir = (float)rand() / 32767.0f + 0.3;
	m_lifeSpan = (int)60 * m_BetweenDir;
}
