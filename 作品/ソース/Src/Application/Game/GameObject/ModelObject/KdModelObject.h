#pragma once

#include"System/GameObject/KdGameObject.h"

class KdModelObject :public KdGameObject
{
public:
	KdModelObject() {}
	virtual ~KdModelObject() override {};


	virtual void Update()override {};
	virtual void DrawLighting()override;
	void SetAsset(std::string_view fileName) override;
	void SetModelDate(const std::shared_ptr<KdModelWork>& sp_model);

	// Œü‚¢‚Ä‚é•ûŒüA‘Šè‚Ì•ûŒü
	virtual void UpdateRotate(Math::Vector3& nowDir,Math::Vector3& targetDir);

	bool CollisionGround(
		const Math::Vector3& rayPos,
		const Math::Vector3& rayDir,
		float rayRange,
		CollisionMeshResult& result) const override;

	bool CollisionBump(
		DirectX::BoundingSphere& sphere,
		CollisionResult& result) const override;

	bool CheckCollisionBump(const SphereInfo& info,
		BumpResult& result)override;

	Math::Vector3 GetPlayerPos() const;
	Math::Vector3 GetEnemy1Pos() const;

	const std::shared_ptr<KdModelWork> GetModelWork() const override { return m_spModel; }

protected:
	//Ray ‘Î Model‚Ì“–‚½‚è”»’è
	bool CollisionRayToModel(
		const Math::Vector3& rayPos,
		const Math::Vector3& rayDir,
		float rayRange,
		CollisionMeshResult& result) const ;

	bool CollisionSpehreToModel(
		DirectX::BoundingSphere& sphere,
		CollisionResult& result) const;


	bool CollisionSpehreToModel(
		const SphereInfo& sphere,
		BumpResult& result) const;



protected:
	std::shared_ptr<KdModelWork>	m_spModel = nullptr;

	Math::Vector3	m_worldRot;
	Math::Matrix	m_trans;
	Math::Matrix	m_Rotation;
};