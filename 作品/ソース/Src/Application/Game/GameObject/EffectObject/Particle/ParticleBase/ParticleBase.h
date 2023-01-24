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

	float Rnd();		//0�`1�̃����_���l

	void SetPlayerPos(Math::Vector3& playerpos)
	{
		m_playerPos = playerpos;
	}

	void UpdateRotate();
	void Billboard(Math::Matrix& mat);

protected:
	Math::Vector3	m_pos;		//���W
	Math::Vector3	m_move;		//�ړ���
	float			m_size;		//�T�C�Y
	Math::Color		m_color;	//�F
	int				m_lifespan;	//�L�����ԁi�������E�j
	bool			m_bRepeat;	//�J��Ԃ��p�̃t���O�i���s�[�g�j
	Math::Matrix	m_mWorld;	//�s��

	std::shared_ptr<KdSquarePolygon> m_pTexture = nullptr;	
	std::shared_ptr<KdTexture> m_p2DTexture = nullptr;	

	Math::Vector3	m_playerPos;
	Math::Vector3	m_worldRot;
};