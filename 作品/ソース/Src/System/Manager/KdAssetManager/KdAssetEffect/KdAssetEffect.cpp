#include "KdAssetEffect.h"

void KdAssetEffect::Init()
{
	// ‰æ‘œ
	SetTexAsset(m_UltTex, "Asset/Texture/Bullet3.png");
	SetTexAsset(m_ThunderTex, "Asset/Texture/Bullet2.png");
	SetTexAsset(m_UltThunderTex, "Asset/Texture/Bullet4.png");
	SetTexAsset(m_SwordEffectTex, "Asset/Texture/swordEffect.png");
	SetTexAsset(m_BulletTex, "Asset/Texture/Bullet.png");
}

void KdAssetEffect::Release()
{
}

void KdAssetEffect::SetTexAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName)
{
	if (!sp_Tex)
	{
		sp_Tex = std::make_shared<KdTexture>();
	}

	sp_Tex->Load(fileName.data());
}

const std::shared_ptr<KdTexture>& KdAssetEffect::GetBulletTex() const
{
	return m_BulletTex;
}

const std::shared_ptr<KdTexture>& KdAssetEffect::GetUltTex() const
{
	return m_UltTex;
}

const std::shared_ptr<KdTexture>& KdAssetEffect::GetThunderTex() const
{
	return m_ThunderTex;
}

const std::shared_ptr<KdTexture>& KdAssetEffect::GetUltThunderTex() const
{
	 return m_UltThunderTex;
}

const std::shared_ptr<KdTexture>& KdAssetEffect::GetSwordEffectTex() const
{
	return m_SwordEffectTex;
}
