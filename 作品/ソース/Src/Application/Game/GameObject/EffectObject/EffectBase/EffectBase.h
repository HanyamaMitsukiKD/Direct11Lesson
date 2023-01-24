#pragma once
#include<Application/Game/GameObject/ModelObject/KdModelObject.h>


class EffectBase :public KdModelObject
{
public:
	EffectBase() {}
	virtual ~EffectBase()override {}	//デストラクタはオーバーライド必須
	
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

	// 弾の軌跡用
	std::shared_ptr<KdTrailPolygon> m_spPoly = nullptr;

	// 5フレームだけ表示
	int m_trailMax = 15;

	int m_lifeSpan = 30;	//２秒飛ぶように
	float m_speed = 0.5f;
};