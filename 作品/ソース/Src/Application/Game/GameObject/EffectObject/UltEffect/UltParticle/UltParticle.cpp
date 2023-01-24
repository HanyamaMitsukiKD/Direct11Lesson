#include "UltParticle.h"

UltParticle::UltParticle()
{
	m_lifeSpan = 180;
	m_speed = 1.0f / (60.0f * 1);

	m_collisionFlg = false;
}

void UltParticle::Update()
{
	if (m_lifeSpan-- <= 0)
	{
		m_needDelete = true;
	}

	m_progress += m_speed;

	float progress = std::clamp(m_progress, 0.0f, 1.0f);
	float spd;

	spd = (1 - EaseOutCirc(progress)) * ((1 - progress) * 0.07f);



	//前方向
	Math::Vector3 forward = m_mWorld.Backward();

	//長さ１にする（正規化）
	forward.Normalize();

	m_mWorld *=
		Math::Matrix::CreateTranslation(forward * spd);
}

void UltParticle::SetMatrix(const float size, const Math::Vector3& pos, const Math::Vector3& dir)
{
	m_mWorld = Math::Matrix::CreateScale(0.1f) *
		Math::Matrix::CreateWorld(pos, -dir, Math::Vector3::Up);

	// 5秒で終着地点へ移動するスピード
	m_speed = 1.0f / (60.0f * 2);
}

void UltParticle::DrawBright()
{
	if (!m_spPoly) { return; }

	// ワールド座標はあげない。２倍移動してします
	D3D.WorkShaderManager().m_effectShader.DrawModel(*m_spModel, m_mWorld);
}

void UltParticle::SetModel(std::shared_ptr<KdModelData>& Model)
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
	}
	m_spModel->SetModel(Model);
}
