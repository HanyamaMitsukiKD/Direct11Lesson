#include "ParticleFire.h"

void ParticleFire::Init()
{
	m_move.x = Rnd() * 2 - 1; //-1～+1のランダム
	m_move.y = Rnd() * 2 - 1; //-1～+1のランダム
	m_color = Math::Color(1, 1, 1, 0);
	m_size = 2;

	if (!m_p2DTexture) { return; }
	m_pTexture = std::make_shared< KdSquarePolygon>();
	m_pTexture->SetTexture(m_p2DTexture);
}

void ParticleFire::Update()
{
	//有効期間の処理
	m_lifespan--;
	if (m_lifespan < 0)
	{
		m_lifespan = 0;
		if (!m_bRepeat)return;	//繰り返しをしない場合は戻る
	}

	m_pTexture->SetColor(m_color);

	//座標更新
	//m_move *= 0.5f;
	m_pos += m_move;

	m_size *= 1.01f;		//1%大きくする

	//行列作成
	Math::Matrix scaleMat, transMat, rotation;
	scaleMat = Math::Matrix::CreateScale(m_size, m_size, m_size);
	transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);

	UpdateRotate();
	rotation = Math::Matrix::
		CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));

	m_mWorld = scaleMat * rotation * transMat;
	

	// ビルボード化
	//m_mWorld = scaleMat * transMat;
	//Billboard(m_mWorld);
}

