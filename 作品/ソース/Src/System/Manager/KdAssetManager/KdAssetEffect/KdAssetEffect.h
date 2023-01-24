#pragma once

class KdAssetEffect
{
public:
	KdAssetEffect() {}
	~KdAssetEffect() { Release(); }

	// ‰Šú‰»
	void Init();
	// ‰ğ•ú
	void Release();

	void SetTexAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName);


	const std::shared_ptr<KdTexture>& GetBulletTex()const;
	const std::shared_ptr<KdTexture>& GetUltTex()const;
	const std::shared_ptr<KdTexture>& GetThunderTex()const;
	const std::shared_ptr<KdTexture>& GetUltThunderTex()const;
	const std::shared_ptr<KdTexture>& GetSwordEffectTex()const;



private:
	std::shared_ptr<KdTexture> m_UltTex = nullptr;
	std::shared_ptr<KdTexture> m_BulletTex = nullptr;
	std::shared_ptr<KdTexture> m_ThunderTex = nullptr;
	std::shared_ptr<KdTexture> m_UltThunderTex = nullptr;
	std::shared_ptr<KdTexture> m_SwordEffectTex = nullptr;
};