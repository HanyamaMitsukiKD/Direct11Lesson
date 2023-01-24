#include "ParticleFire.h"

void ParticleFire::Init()
{
	m_move.x = Rnd() * 2 - 1; //-1�`+1�̃����_��
	m_move.y = Rnd() * 2 - 1; //-1�`+1�̃����_��
	m_color = Math::Color(1, 1, 1, 0);
	m_size = 2;

	if (!m_p2DTexture) { return; }
	m_pTexture = std::make_shared< KdSquarePolygon>();
	m_pTexture->SetTexture(m_p2DTexture);
}

void ParticleFire::Update()
{
	//�L�����Ԃ̏���
	m_lifespan--;
	if (m_lifespan < 0)
	{
		m_lifespan = 0;
		if (!m_bRepeat)return;	//�J��Ԃ������Ȃ��ꍇ�͖߂�
	}

	m_pTexture->SetColor(m_color);

	//���W�X�V
	//m_move *= 0.5f;
	m_pos += m_move;

	m_size *= 1.01f;		//1%�傫������

	//�s��쐬
	Math::Matrix scaleMat, transMat, rotation;
	scaleMat = Math::Matrix::CreateScale(m_size, m_size, m_size);
	transMat = Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, m_pos.z);

	UpdateRotate();
	rotation = Math::Matrix::
		CreateRotationY(DirectX::XMConvertToRadians(m_worldRot.y));

	m_mWorld = scaleMat * rotation * transMat;
	

	// �r���{�[�h��
	//m_mWorld = scaleMat * transMat;
	//Billboard(m_mWorld);
}

