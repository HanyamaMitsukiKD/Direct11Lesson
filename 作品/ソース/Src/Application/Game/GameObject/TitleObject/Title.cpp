#include "Title.h"

void Title::Init()
{
	SetAsset(m_spTexture,"Asset/Texture/Title/Title.png");
	SetAsset(m_spStartTexture, "Asset/Texture/Title/gameStart.png");
	SetAsset(m_spRogoTexture, "Asset/Texture/Title/rogo.png");
	SetAsset(m_spKeyTexture, "Asset/Texture/Title/Key.png");

	m_startTexPos = { 0,-100 };
	m_alpha = 1;
	m_alphaSpd = 0.01f;
}

void Title::Update()
{
	// Á‚¦‚½‚è‚·‚éˆ—
	if (m_alpha >= 1.0f || m_alpha <= 0.1f)
	{
		m_alphaSpd *= -1;
	}

	m_alpha += m_alphaSpd;
}

void Title::DrawSprite()
{
	if (!m_spTexture) { return; }
	D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spTexture.get(), 0, 0);
	D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spRogoTexture.get(), 0, 50);

	Math::Color col = { 1,1,1,m_alpha };
	Math::Rectangle rec = { 0,0,630,100 };
	Math::Rectangle Keyrec = { 0,0,482,85 };

	D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spStartTexture.get(), (int)m_startTexPos.x, (int)m_startTexPos.y, &rec, &col);
	D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spKeyTexture.get(), (int)m_startTexPos.x, (int)m_startTexPos.y - 100, &Keyrec, &col);
}

void Title::SetAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName)
{
	if (!sp_Tex)
	{
		sp_Tex = std::make_shared<KdTexture>();
	}

	sp_Tex->Load(fileName.data());
}
