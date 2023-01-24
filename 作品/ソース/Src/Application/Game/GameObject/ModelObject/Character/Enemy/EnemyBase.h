#pragma once

#include"../CharacterBase.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"

class EnemyBase :public CharacterBase
{
public:
	EnemyBase() {}
	virtual ~EnemyBase() override {}

	void Update()override;

	eClassID GetClassID() const override { return eEnemy; }

	const int GetHP() const { return  (int)enemyHP; }
	const int GetDownDamag() const { return  m_bigDownDamage; }

	void SetPlayer(const std::shared_ptr<Player> player)
	{
		m_wpPlayer = player;	//��Q�ƁB��������������
	}

	enum eEnemyState
	{
		ebullet,			//�e�U��
		eShortlyAttack		//�ߋ����U��
	};

protected:
	eEnemyState enemyState;

	std::weak_ptr<Player>	m_wpPlayer;				//�v���C���[���

	float enemyHP = 728;
	int m_bigDownDamage = 3;						//�R�񂭂�������_���[�W
};