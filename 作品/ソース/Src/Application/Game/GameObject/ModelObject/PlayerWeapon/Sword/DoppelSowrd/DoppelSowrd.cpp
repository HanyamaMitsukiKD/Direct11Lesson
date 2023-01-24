#include "DoppelSowrd.h"

void DoppelSowrd::Update()
{
	Math::Matrix LocalPos;
	LocalPos = Math::Matrix::CreateTranslation(Math::Vector3(0, 0, 3.0f));

	m_mWorld = LocalPos* m_mWorld;
}

void DoppelSowrd::DrawLighting()
{
	D3D.WorkShaderManager().m_standardShader.DrawModel(*m_spModel, m_mWorld);
}