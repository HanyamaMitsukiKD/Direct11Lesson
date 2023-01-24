#pragma once

#include"Application/Game/GameObject/ModelObject/Bullet/BulletBase.h"

class nomalBullet :public BulletBase
{
public:
	nomalBullet() {}
	virtual ~nomalBullet() override {}

	void Update() override;
	void Init() override;
	void DrawLighting()override;
	

	void UpdateRotate();

private:
	//弾の状態別更新
	void NormalUpdate();

	//イージング関数処理
	//弾の挙動
	void UpdateMove();
	

	//衝突判定
	void UpdateGroundCollision();

	void SmokeUpdate();

	void EffectFire(Math::Vector3& pos);

private:
	Math::Vector3	m_moveDir;				//方向
	Math::Vector3	m_targetPos;			//ターゲット座標

	int				loopAttckCount = 0;		//何回攻撃するか（弾の残弾をエネミーに依存させるまで）
	float			bulletYPosSpeed = 0;	//弾がY座標を登っていく移動量

	//イージング関数用
	float			m_moveSpeed = 0.0f;		//スピード
};