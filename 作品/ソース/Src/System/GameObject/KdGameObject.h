#pragma once

struct SphereInfo
{
	SphereInfo() {}

	SphereInfo(const Math::Vector3& pos, float radius) :
		m_pos(pos),
		m_radius(radius)
	{}

	Math::Vector3 m_pos = Math::Vector3::Zero;
	float m_radius = 1.0f;
};

struct BumpResult
{
	bool m_isHit = false;
	Math::Vector3 m_pushVec = Math::Vector3::Zero;
};

//3D��ԏ�ɂ̂ݑ��݂���@�\
class KdGameObject : public std::enable_shared_from_this<KdGameObject>
{
public:
	KdGameObject() {}
	virtual ~KdGameObject() {}

	virtual void Init() {}
	virtual void Update() = 0;									
	virtual void DrawLighting() {}
	virtual void DrawNoLighting() {}
	virtual void DrawBright() {}
	virtual void DrawSprite() {}
	virtual void SetAsset(std::string_view fileName) {};


	virtual const std::shared_ptr<KdModelWork> GetModelWork() const { return nullptr; }

	enum eClassID
	{
		eBase,
		eStage,
		ePlayer,
		eEnemy,
		eEnemyBullet,
		ePlayerSowd
	};

	enum eState {
		eMove,			//�ړ�
		eStandBy,		//�ҋ@
		eAvoidance,		//���
		eJustAvoidance,	//�W���X�g���
		eAttack,		//�U��
		eBullet,		//�e
		eDamage,		//�_���[�W
		eUlt,			//�E���g
		eNoAlive		//HP0
	};

	

	// �s��
	const Math::Matrix& GetMatrix() const { return m_mWorld; }	
	void SetMatrix(const Math::Matrix& mat) {					
		m_mWorld = mat;
	}

	// ���W
	void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	// ����
	bool NeedDelete() { return m_needDelete; }
	virtual void NeedDeleteTrue()
	{
		m_needDelete = true;
	}


	struct  CollisionResult
	{
		bool m_hit = false;
		Math::Vector3 m_hitPos;
		Math::Vector3 m_hitDir;
		float m_overlapDistance = 0.0f; //�I�u�W�F�N�g�Ԃ̋���
	};

	//�I�u�W�F�N�g���m�̏Փ˔���(��VS��)
	virtual bool CheckCollisionBump(const SphereInfo& info, BumpResult& result)
	{
		return false;
	}

	//�p�r�ɂ�铖���蔻��̌Ăяo��
	virtual bool CollisionGround(
		const Math::Vector3& rayPos,
		const Math::Vector3& rayDir,
		float rayRange,
		CollisionMeshResult& result) const { return false; }


	virtual bool CollisionBump(
		DirectX::BoundingSphere& sphere,
		CollisionResult& result) const {
		return false;
	}

	virtual eClassID GetClassID() const { return eBase; }

	// ��ԕύX
	virtual eState GeteState()const { return eObjectState; }
	virtual void SetState(const eState state)	{
		eObjectState	= state;
		m_count			= 0;
	}

	
	
	//�~����̃Q�b�g���Z�b�g
	virtual bool GetCircleCollisionFlg() const { return m_circleCollisionFlg; }
	virtual void SetCircleCollisionFlg(bool flg) { m_circleCollisionFlg = flg; }

	//���f�����s���Ă��邩���Z�b�g
	virtual void SetAlive(bool flg) { m_Alive = flg; }					
	virtual bool GetAlive()const { return m_Alive; }

	//���f���̏����ʒu
	virtual void InitPos() {};
	
	//�b��
	virtual int GetCount() const { return m_count; }
	virtual void InitCount() { m_count = 0; }

	//�@�I�u�W�F�N�g�X�s�[�h
	virtual void SetObjectSpeed(float speed) {
		m_Objectspeed = speed;
	}
	virtual float GetObjectSpeed() const { return m_Objectspeed; }

	//�Փ˓����蔻��̃Q�b�g���Z�b�g
	void SetCollisionFlg(bool flg) { m_collisionFlg = flg; }
	bool GetCollisionFlg() { return m_collisionFlg; }

	Math::Vector3 GetCameraPos() { return m_cameraPos; }

protected:
	eState			eObjectState = eStandBy;

	Math::Matrix	m_mWorld;
	Math::Vector3   m_cameraPos;

	bool			m_needDelete = false;			//�����t���O
	bool			m_circleCollisionFlg = false;	//�~�����蔻��t���O
	bool			m_collisionFlg = false;			//�Փ˓����蔻��t���O
	bool			m_Alive = true;					//�����t���O

	int				m_count = 0;					//�b���J�E���g

	float			m_Objectspeed = 1.0f;			//�I�u�W�F�N�g�X�s�[�h
};