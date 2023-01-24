#include "StageSelect.h"

void StageSelect::DrawSprite()
{
	if (!m_spTexture) { return; }
	D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spTexture.get(), 0, 0);
}

void StageSelect::SetAsset(std::string_view fileName)
{
	if (!m_spTexture)
	{
		m_spTexture = std::make_shared<KdTexture>();
	}
	m_spTexture->Load(fileName.data());
}
