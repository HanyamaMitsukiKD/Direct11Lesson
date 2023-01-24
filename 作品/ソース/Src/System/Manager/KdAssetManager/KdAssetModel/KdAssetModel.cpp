#include "KdAssetModel.h"

void KdAssetModel::Init()
{
	// ステージ
	SetModelDate(m_spModel_Sky, "Asset/Model/Sky/sky.gltf");
	SetModelDate(m_spModel_Stage, "Asset/Model/Stage/stage3.gltf");
	SetModelDate(m_spModel_StageGround, "Asset/Model/Stage/stage_Ground.gltf");
	SetModelDate(m_spModel_Player, "Asset/Model/Player/robot.gltf");
	SetModelDate(m_spModel_Sword, "Asset/Model/Sword/Sword_04.gltf");
	SetModelDate(m_spModel_Enemy, "Asset/Model/Enemy01/normalEnemy3.0.gltf");
}

void KdAssetModel::Release()
{
}

void KdAssetModel::SetModelDate(std::shared_ptr<KdModelWork>& modelDate, std::string_view fileName)
{
	if (!modelDate)
	{
		modelDate = std::make_shared<KdModelWork>();
	}

	modelDate->Load(fileName.data());
}

const std::shared_ptr<KdModelWork>& KdAssetModel::GetModel_Stage() const
{
	return m_spModel_Stage;
}

const std::shared_ptr<KdModelWork>& KdAssetModel::GetModel_Sky() const
{
	return m_spModel_Sky;
}

const std::shared_ptr<KdModelWork>& KdAssetModel::GetModel_StageGround() const
{
	return m_spModel_StageGround;
}

const std::shared_ptr<KdModelWork>& KdAssetModel::GetModel_Player() const
{
	return m_spModel_Player;
}

const std::shared_ptr<KdModelWork>& KdAssetModel::GetModel_Sword() const
{
	return m_spModel_Sword;
}

const std::shared_ptr<KdModelWork>& KdAssetModel::GetModel_Enemy() const
{
	return m_spModel_Enemy;
}
