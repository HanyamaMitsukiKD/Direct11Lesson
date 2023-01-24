#include "KdModelObject.h"

void KdModelObject::DrawLighting()
{
	if (!m_spModel) { return; }
	D3D.WorkShaderManager().m_standardShader.DrawModel(*m_spModel, m_mWorld);
}

void KdModelObject::SetAsset(std::string_view fileName)
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
	}

	m_spModel->Load(fileName.data());
}

void KdModelObject::SetModelDate(const std::shared_ptr<KdModelWork>& sp_model)
{
	if (!m_spModel)
	{
		m_spModel = std::make_shared<KdModelWork>();
	}

	m_spModel = sp_model;
}

void KdModelObject::UpdateRotate(Math::Vector3& nowDir, Math::Vector3& targetDir)
{
	// �L�����̐��ʕ����̃x�N�g��
	Math::Vector3 vNowDir = nowDir;

	// �ړ������̃x�N�g��
	Math::Vector3 vTargetDir = targetDir;

	vNowDir.Normalize();
	vTargetDir.Normalize();

	// Degree�p
	float nowAng = atan2(vNowDir.x, vNowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(vTargetDir.x, vTargetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// �Ԃ̊p�x
	float betweenAng = targetAng - nowAng;
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotateAng = std::clamp(betweenAng, -10.0f, 10.0f);
	m_worldRot.y += rotateAng;
}

bool KdModelObject::CollisionGround(
	const Math::Vector3& rayPos,
	const Math::Vector3& rayDir,
	float rayRange,
	CollisionMeshResult& result) const
{
	return CollisionRayToModel(rayPos, rayDir, rayRange, result);
}


bool KdModelObject::CollisionRayToModel(
	const Math::Vector3& rayPos,
	const Math::Vector3& rayDir,
	float rayRange,
	CollisionMeshResult& result)	const					
{
	if (!m_spModel) { return false; }

	//���f���̒��̍ŋߐځA�ڐG�_�̃��U���g
	CollisionMeshResult nearestRes;

	//�S�Ẵ��b�V���Ƃ̉����Ԃ��x�N�g���̍��v
	//CollisionMeshResult toatlRes;
	

	//�S�Ẵ��b�V���f�[�^�Ƒ�����
	for (UINT i = 0; i < m_spModel->GetNodes().size(); ++i)
	{
		CollisionMeshResult tempRes;

		//���b�V���f�[�^�̎擾
		//m_spModel->GetMesh(i);	//����nullptr�`�F�b�N���Ă�
		std::shared_ptr<KdMesh> spMesh = m_spModel->GetDataNodes()[i].m_spMesh;
		
		if (spMesh) {
			//�������Q�ƂȂ���̂�n��
			//���U���g���󂯎��
			KdMeshIntersect(*spMesh, rayPos, rayDir, rayRange,
				m_spModel->GetNodes()[i].m_worldTransform * m_mWorld, &tempRes);
		}

		//�����Ƃ��������߂����̂𒊏o���ĕԂ�
		if (!tempRes.m_hit) { continue; }
		if (tempRes.m_overlapDistance > nearestRes.m_overlapDistance)
		{
			nearestRes = tempRes;
		}

	}

	if (nearestRes.m_overlapDistance > result.m_overlapDistance)
	{
		result = nearestRes;
	}

	return nearestRes.m_hit;
}

bool KdModelObject::CollisionBump(DirectX::BoundingSphere& sphere, CollisionResult& result) const
{
	return CollisionSpehreToModel(sphere, result);
}

bool KdModelObject::CollisionSpehreToModel(DirectX::BoundingSphere& sphere, CollisionResult& result) const
{

	if (!m_spModel) { return false; }

	bool isHit = false;

	//���f���̒��̍ŋߐځA�ڐG�_�̃��U���g
	Math::Vector3 toatlPush;

	//�S�Ẵ��b�V���f�[�^�Ƒ�����
	for (UINT i = 0; i < m_spModel->GetNodes().size(); ++i)
	{
		CollisionMeshResult tempRes;

		//���b�V���f�[�^�̎擾
		//m_spModel->GetMesh(i);	//����nullptr�`�F�b�N���Ă�
		std::shared_ptr<KdMesh> spMesh = m_spModel->GetDataNodes()[i].m_spMesh;

		//�������Q�ƂȂ���̂�n��
		//���U���g���󂯎��
		if (!spMesh) { return false; }
		KdMeshIntersect(*spMesh, sphere,
			m_spModel->GetNodes()[i].m_worldTransform * m_mWorld, &tempRes);


		//�����������_�ŉ����Ԃ�
		if (tempRes.m_hit)
		{
			Math::Vector3 hitDir = tempRes.m_hitDir;
			toatlPush += hitDir * tempRes.m_overlapDistance;

			result.m_hitPos = tempRes.m_hitPos;

			isHit = true;
		}
	}

	if (isHit)
	{
		result.m_hit = true;
		result.m_hitDir = toatlPush;
		result.m_hitDir.Normalize();
		result.m_overlapDistance = toatlPush.Length();	//�����Ԃ�����
	}
	
	return isHit;
}

bool KdModelObject::CheckCollisionBump(const SphereInfo& info, BumpResult& result)
{

	return CollisionSpehreToModel(info, result);
}

bool KdModelObject::CollisionSpehreToModel(const SphereInfo& sphere, BumpResult& result) const
{
	if (!m_spModel) { return false; }

	bool isHit = false;

	Math::Vector3 selfPos = GetPos();
	Math::Vector3 betweenVec = sphere.m_pos - selfPos;

	float distancesSqr = betweenVec.LengthSquared();

	float hitRadius = 0.25f + sphere.m_radius;

	result.m_isHit = (distancesSqr <= (hitRadius * hitRadius));

	isHit = result.m_isHit;

	return isHit;
}

Math::Vector3 KdModelObject::GetPlayerPos() const
{
	Math::Vector3 pos;
	KdModelWork::Node* pTop = m_spModel->FindWorkNode("StartPoint_Player");

	pos = pTop->m_localTransform.Translation();

	pos.z -= 20;

	return pos;
}

Math::Vector3 KdModelObject::GetEnemy1Pos() const
{
	Math::Vector3 pos;
	KdModelWork::Node* pTop = m_spModel->FindWorkNode("StartPoint_Enemy");

	pos = pTop->m_localTransform.Translation();

	pos.z += 5;

	return pos;
}