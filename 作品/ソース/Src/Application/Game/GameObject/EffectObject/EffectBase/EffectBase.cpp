#include "EffectBase.h"

void EffectBase::SetAsset(std::string_view fileName)
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdTrailPolygon>();
	}

	//　トレイルポリゴンにテクスチャセット
	m_spPoly->Load(fileName);

	// ビルボードパターン：どの角度で撃っても表示される
	m_spPoly->
		SetPattern(KdTrailPolygon::Trail_Pattern::eBillboard);
}

void EffectBase::SetAsset2D(const std::shared_ptr<KdTexture>& tex)
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdTrailPolygon>();
	}

	//　トレイルポリゴンにテクスチャセット
	m_spPoly->SetTexture(tex);

	// ビルボードパターン：どの角度で撃っても表示される
	m_spPoly->
		SetPattern(KdTrailPolygon::Trail_Pattern::eBillboard);
}

void EffectBase::DrawEffect()
{
	if (!m_spPoly) { return; }

	// ワールド座標はあげない。２倍移動してします
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

	// 5フレームだけ飛ぶ
	if (m_spPoly->GetNumPoints() > m_trailMax)
	{
		m_spPoly->DelPoint_Back();
	}
}