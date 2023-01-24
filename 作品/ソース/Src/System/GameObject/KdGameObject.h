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

//3D空間上にのみ存在する機能
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
		eMove,			//移動
		eStandBy,		//待機
		eAvoidance,		//回避
		eJustAvoidance,	//ジャスト回避
		eAttack,		//攻撃
		eBullet,		//弾
		eDamage,		//ダメージ
		eUlt,			//ウルト
		eNoAlive		//HP0
	};

	

	// 行列
	const Math::Matrix& GetMatrix() const { return m_mWorld; }	
	void SetMatrix(const Math::Matrix& mat) {					
		m_mWorld = mat;
	}

	// 座標
	void SetPos(const Math::Vector3& pos) { m_mWorld.Translation(pos); }
	Math::Vector3 GetPos() const { return m_mWorld.Translation(); }

	// 消去
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
		float m_overlapDistance = 0.0f; //オブジェクト間の距離
	};

	//オブジェクト同士の衝突判定(球VS球)
	virtual bool CheckCollisionBump(const SphereInfo& info, BumpResult& result)
	{
		return false;
	}

	//用途による当たり判定の呼び出し
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

	// 状態変更
	virtual eState GeteState()const { return eObjectState; }
	virtual void SetState(const eState state)	{
		eObjectState	= state;
		m_count			= 0;
	}

	
	
	//円判定のゲット＆セット
	virtual bool GetCircleCollisionFlg() const { return m_circleCollisionFlg; }
	virtual void SetCircleCollisionFlg(bool flg) { m_circleCollisionFlg = flg; }

	//モデルが行きているかをセット
	virtual void SetAlive(bool flg) { m_Alive = flg; }					
	virtual bool GetAlive()const { return m_Alive; }

	//モデルの初期位置
	virtual void InitPos() {};
	
	//秒数
	virtual int GetCount() const { return m_count; }
	virtual void InitCount() { m_count = 0; }

	//　オブジェクトスピード
	virtual void SetObjectSpeed(float speed) {
		m_Objectspeed = speed;
	}
	virtual float GetObjectSpeed() const { return m_Objectspeed; }

	//衝突当たり判定のゲット＆セット
	void SetCollisionFlg(bool flg) { m_collisionFlg = flg; }
	bool GetCollisionFlg() { return m_collisionFlg; }

	Math::Vector3 GetCameraPos() { return m_cameraPos; }

protected:
	eState			eObjectState = eStandBy;

	Math::Matrix	m_mWorld;
	Math::Vector3   m_cameraPos;

	bool			m_needDelete = false;			//消去フラグ
	bool			m_circleCollisionFlg = false;	//円当たり判定フラグ
	bool			m_collisionFlg = false;			//衝突当たり判定フラグ
	bool			m_Alive = true;					//生存フラグ

	int				m_count = 0;					//秒数カウント

	float			m_Objectspeed = 1.0f;			//オブジェクトスピード
};