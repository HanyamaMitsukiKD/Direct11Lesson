#pragma once

#include"Application/Game/GameObject/ModelObject/Character/Enemy/EnemyBase.h"
#include"Application/Game/GameObject/EffectObject/EffectBase/EffectBase.h"
#include"Application/Game/GameObject/EffectObject/SwordEffect/SwordEffect.h"

class nomalBullet;

class normalEnemy :public EnemyBase
{
public:
	normalEnemy() {}
	virtual ~normalEnemy() override {}

	void Update() override;
	void DrawLighting()override;
	void Init()override;

	eState GeteState()const override { return eObjectState; }

	const bool GetDamageFlg()const { return m_damageFlg; }
	const int GetDamegeNum()const { return m_damegeNum; }
	void SetDamageFlg(bool flg)
	{
		m_damageFlg = flg;
	}

	void EnemyBulletCreate();

	// SE
	const bool GetEnemyBulletCreateFlg()const { return m_enemyBulletCreateFlg; }
	void SetEnemyBulletCreateFlg(bool flg) { m_enemyBulletCreateFlg = flg; }

	// �C�[�W���O�֐�
	void easeInCubic(float& move, float& pos);

	const float GetMoveSpd()const { return m_moveSpd; }

	void SetDamageFlgTrue() {

		m_damageFlg = true;
	}

	void CounterDamage(Math::Vector3& damagePos) {
		UpdateAttackEffect(damagePos);
		enemyHP -= 1.0f;
	}

	void UpdateEffect();
	void UpdateAttackEffect(Math::Vector3& damagePos);
	void UpdateSlashingAttackEffect();
	void UpdateSlashingMoveEffect();

	void CreateUltDamageEffect();
	void CreateAttackStartEffect();

private:
	//�Փ˔���Ƃ���ɔ������W�̍X�V
	void UpdateGroundCollision();
	void UpdateCircleCollision();
	void UpdateCollision();

	void PlayerJustCounter();



private:
	//	�_���[�W���󂯂��Ƃ��p�̕ϐ�
	bool	m_damageFlg = false;
	bool	closeRangeCheck = false;
	bool	m_noAliveFlg = false;
	int		m_damegeNum = 0;
	float	m_progress = 0.0f;

	float	playerMove = 0;				// �ړ���
	float	m_moveSpd = 0.2f;

	// ���ʉ��t���O
	bool	takkuruMoveSEFlg = false;

	// �~�T�C��
	bool	m_enemyBulletCreateFlg = false;
	int		m_bulletCreate = 180;		// 3�b�����ɒe����

	bool	m_nMoveFlg = false;

	// �_���[�W�G�t�F�N�g
	bool	m_attackEffectFlg = false;
	int		m_attackEffecCount = 0;

	// �a���G�t�F�N�g
	std::list<std::shared_ptr<EffectBase>>	m_spAttacks;
	Math::Vector3 animationPos;

	std::list<std::shared_ptr<SwordEffect>>	m_spSwordEffects;

	bool m_battleRound2Flg = false;
};