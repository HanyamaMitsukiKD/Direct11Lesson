#pragma once


#include"System/GameObject/KdGameObject.h"

class ParticleBase :public KdGameObject
{
public:
	ParticleBase(){}
	virtual ~ParticleBase()override {}

	virtual void Init(){}
	virtual void Update(Math::Vector3 pos){}
	virtual void DrawNoLighting()override;

	virtual void Emit(Math::Vector3 a_pos, Math::Vector3 a_move,
		float a_size = 1, Math::Color a_color = Math::Color(1, 1, 1, 1),
		int a_lifespan = 100, bool a_bRepeat = false);			
											
	virtual void SetTexture(std::shared_ptr<KdTexture>& apTexture);	

	float Rnd();		//0～1のランダム値

	void SetPlayerPos(Math::Vector3& playerpos)
	{
		m_playerPos = playerpos;
	}

	void UpdateRotate();
	void Billboard(Math::Matrix& mat);

protected:
	Math::Vector3	m_pos;		//座標
	Math::Vector3	m_move;		//移動量
	float			m_size;		//サイズ
	Math::Color		m_color;	//色
	int				m_lifespan;	//有効期間（活動限界）
	bool			m_bRepeat;	//繰り返し用のフラグ（リピート）
	Math::Matrix	m_mWorld;	//行列

	std::shared_ptr<KdSquarePolygon> m_pTexture = nullptr;	
	std::shared_ptr<KdTexture> m_p2DTexture = nullptr;	

	Math::Vector3	m_playerPos;
	Math::Vector3	m_worldRot;
};