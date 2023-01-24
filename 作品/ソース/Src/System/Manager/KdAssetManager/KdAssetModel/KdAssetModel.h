#pragma once

class KdAssetModel
{
public:
	KdAssetModel() {}
	~KdAssetModel() { Release(); }

	// ‰Šú‰»
	void Init();
	// ‰ğ•ú
	void Release();

	void SetModelDate(std::shared_ptr<KdModelWork>& modelDate, std::string_view fileName);

	const std::shared_ptr<KdModelWork>& GetModel_Stage() const;
	

	const std::shared_ptr<KdModelWork>& GetModel_Sky() const;
	const std::shared_ptr<KdModelWork>& GetModel_StageGround() const;
	const std::shared_ptr<KdModelWork>& GetModel_Player() const;
	const std::shared_ptr<KdModelWork>& GetModel_Sword() const;
	const std::shared_ptr<KdModelWork>& GetModel_Enemy() const;

private:
	std::shared_ptr<KdModelWork>	m_spModel_Sky = nullptr;
	std::shared_ptr<KdModelWork>	m_spModel_Stage = nullptr;
	std::shared_ptr<KdModelWork>	m_spModel_StageGround = nullptr;
	std::shared_ptr<KdModelWork>	m_spModel_Player = nullptr;
	std::shared_ptr<KdModelWork>	m_spModel_Sword = nullptr;
	std::shared_ptr<KdModelWork>	m_spModel_Enemy = nullptr;
};