#include "Result.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"

void Result::Init()
{
	SetAsset(m_spMessageTexture, "Asset/Texture/Result/Message.png");
	SetAsset(m_spTextTexture, "Asset/Texture/Result/text.png");

	m_MessageTexPos = { -100,0 };

	m_mailOpenFlg = false;
	recX =120;

	mailSE = false;
	messageSE = false;
}

void Result::Update()
{
	if (m_MessageTexPos.x >= 0)
	{
		m_mailOpenFlg = true;
	}
	else
	{
		if (!mailSE)
		{
			KdGameAudioManager::GetInstance().mailSE();
			mailSE = true;
		}

		m_MessageTexPos.x++;
	}

	if (m_mailOpenFlg)
	{
		if (recX <= 1280)
		{
			recX += 30;

			if (!messageSE)
			{
				KdGameAudioManager::GetInstance().messageSE();
				messageSE = true;
			}
		}
	}
}

void Result::DrawSprite()
{
	if (m_mailOpenFlg)
	{
		Math::Rectangle rec = { 0,0,recX,720 };
		long i =  ((recX - 1280) / 2);
		
		D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spTextTexture.get(), i, 0, recX, 720);
	}

	D3D.WorkShaderManager().m_spriteShader.DrawTex(m_spMessageTexture.get(), (int)m_MessageTexPos.x, (int)m_MessageTexPos.y);
}

void Result::SetAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName)
{
	if (!sp_Tex)
	{
		sp_Tex = std::make_shared<KdTexture>();
	}

	sp_Tex->Load(fileName.data());
}