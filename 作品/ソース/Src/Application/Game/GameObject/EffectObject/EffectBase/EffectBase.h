#pragma once
#include<Application/Game/GameObject/ModelObject/KdModelObject.h>


class EffectBase :public KdModelObject
{
public:
	EffectBase() {}
	virtual ~EffectBase()override {}	//�f�X�g���N�^�̓I�[�o�[���C�h�K�{
	
	virtual void Update(Math::Vector3& Pos){}
	void SetAsset(std::string_view fileName)override;
	void SetAsset2D(const std::shared_ptr<KdTexture>& tex);
	void DrawEffect();
	
	void SetDrawFrameMax(int trailMax);

	virtual void SetMatrix(const Math::Vector3& pos, const Math::Vector3& dir) {};

	void lifeSpan(const int lifeSpan)
	{
		m_lifeSpan = lifeSpan;
	}

	void Speed(const float speed)
	{
		m_speed = speed;
	}

	void UpdateTrail();

protected:

	// �e�̋O�՗p
	std::shared_ptr<KdTrailPolygon> m_spPoly = nullptr;

	// 5�t���[�������\��
	int m_trailMax = 15;

	int m_lifeSpan = 30;	//�Q�b��Ԃ悤��
	float m_speed = 0.5f;
};