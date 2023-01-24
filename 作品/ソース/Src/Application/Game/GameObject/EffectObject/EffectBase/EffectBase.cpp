#include "EffectBase.h"

void EffectBase::SetAsset(std::string_view fileName)
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdTrailPolygon>();
	}

	//�@�g���C���|���S���Ƀe�N�X�`���Z�b�g
	m_spPoly->Load(fileName);

	// �r���{�[�h�p�^�[���F�ǂ̊p�x�Ō����Ă��\�������
	m_spPoly->
		SetPattern(KdTrailPolygon::Trail_Pattern::eBillboard);
}

void EffectBase::SetAsset2D(const std::shared_ptr<KdTexture>& tex)
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdTrailPolygon>();
	}

	//�@�g���C���|���S���Ƀe�N�X�`���Z�b�g
	m_spPoly->SetTexture(tex);

	// �r���{�[�h�p�^�[���F�ǂ̊p�x�Ō����Ă��\�������
	m_spPoly->
		SetPattern(KdTrailPolygon::Trail_Pattern::eBillboard);
}

void EffectBase::DrawEffect()
{
	if (!m_spPoly) { return; }

	// ���[���h���W�͂����Ȃ��B�Q�{�ړ����Ă��܂�
	D3D.WorkShaderManager().m_effectShader.
		DrawPolygon(*m_spPoly, Math::Matrix::Identity);
}

void EffectBase::SetDrawFrameMax(int trailMax)
{
	m_trailMax = trailMax;
}

void EffectBase::UpdateTrail()
{
	if (!m_spPoly) { return; }

	m_spPoly->AddPoint(m_mWorld);

	// 5�t���[���������
	if (m_spPoly->GetNumPoints() > m_trailMax)
	{
		m_spPoly->DelPoint_Back();
	}
}