#include "ParticleBase.h"

void ParticleBase::DrawNoLighting()
{
	if (m_lifespan > 0)
	{
		D3D.WorkShaderManager().m_effectShader.DrawPolygon(*m_pTexture, m_mWorld);
	}
}

void ParticleBase::Emit(Math::Vector3 a_pos, Math::Vector3 a_move, float a_size, Math::Color a_color, int a_lifespan, bool a_bRepeat)
{
	m_pos = a_pos;
	m_move = a_move;
	m_size = a_size;
	m_color = a_color;
	m_lifespan = a_lifespan;
	m_bRepeat = a_bRepeat;
}

void ParticleBase::SetTexture(std::shared_ptr<KdTexture>& apTexture)
{
	if (apTexture == nullptr)return;

	if (!m_p2DTexture)
	{
		m_p2DTexture = std::make_shared<KdTexture>();
	}

	m_p2DTexture = apTexture;
}

float ParticleBase::Rnd()
{
	return rand() / (float)RAND_MAX;
}

void ParticleBase::UpdateRotate()
{
	// エネミーの正面方向のベクトル
	Math::Vector3 nowDir = m_mWorld.Backward();

	// プレイヤーのベクトル
	Math::Vector3 targetDir;
	targetDir = m_playerPos - m_mWorld.Translation();

	nowDir.Normalize();
	targetDir.Normalize();

	// Degree角
	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// 間の角度
	float betweenAng = targetAng - nowAng;
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotateAng = std::clamp(betweenAng, -10.0f, 10.0f);
	m_worldRot.y += rotateAng;
}

void ParticleBase::Billboard(Math::Matrix& mat)
{
	// ビルボード化
	// カメラの情報
	Math::Matrix mCam = D3D.WorkShaderManager().m_cb7_Camera.Get().mView.Invert();
	Math::Vector3 pos = mat.Translation();
	Math::Vector3 targetDir = pos - mCam.Translation();
	targetDir.Normalize();
	Math::Vector3 gaisekiX, gaisekiY;
	targetDir.Cross(Math::Vector3::Up, gaisekiX);
	targetDir.Cross(gaisekiX, gaisekiY);

	mat = Math::Matrix::Matrix(gaisekiX, gaisekiY, targetDir);
}
