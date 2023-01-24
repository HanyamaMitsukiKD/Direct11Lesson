#include "PlayerAttack.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/PlayerState/Move/PlayerMove.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"

void PlayerAttack::Exit(Player& owner)
{
	m_count = 0;
	owner.SetAttackAnimationState(Player::eAnimationAttack1);
	owner.SetState(KdGameObject::eState::eAttack);
	m_attackAnimotionContinueFlg = false;
	endAnimationFlg = false;
	m_AttackKeyFlg = false;
	m_attackFlg = false;

	m_attackState = SwordAttack1;
}

void PlayerAttack::Update(Player& owner)
{
	Player::ePlayerAttackAnimation attackState = owner.GetAttackAnimationState();

	// �U���A�j���[�V�����I��
	if (endAnimationFlg)
	{
		if (owner.GetAnimationEndFlg())
		{
			ChangeMove(owner);
		}

		return;
	}

	if (!m_attackFlg)
	{
		if (!GetAsyncKeyState(VK_LBUTTON))
		{
			m_attackFlg = true;
		}
	}

	//���U��
	if (m_attackFlg)
	{
		if (GetAsyncKeyState(VK_LBUTTON))
		{
			if (!m_AttackKeyFlg)
			{
				// ���U���𑱂��邩
				if (!m_attackAnimotionContinueFlg)
				{
					// ���̃A�j���[�V�����Z�b�g
					switch (attackState)
					{
					case Player::eNoAnimationAttack:
						attackState = Player::eAnimationAttack1;

					case Player::eAnimationAttack1:
						attackState = Player::eAnimationAttack2;

						break;
					case Player::eAnimationAttack2:
						attackState = Player::eAnimationAttack3;

						break;

					case Player::eAnimationAttack3:
						attackState = Player::eNoAnimationAttack;

					default:
						break;
					}

					owner.SetAttackAnimationState(attackState);
					m_attackAnimotionContinueFlg = true;
				}


				m_AttackKeyFlg = true;
			}
		}
		else
		{
			m_AttackKeyFlg = false;
		}
	}

	// �A�j���[�V�������I�������
	if (owner.GetAnimationEndFlg())
	{
		//�U���A�j���[�V�����𑱂��邩�t���O
		if (m_attackAnimotionContinueFlg)
		{
			// �U���𑱂���
			switch (owner.GetAttackAnimationState())
			{
			case Player::eAnimationAttack2:	//�Q��ڂ̍U�����I�������
				owner.SetPlayerAttackAnimation("Attack2", false);
				m_attackState = SwordAttack2;
				break;

			case Player::eAnimationAttack3:	//�Q��ڂ̍U�����I�������
				owner.SetPlayerAttackAnimation("Attack3", false);
				m_attackState = SwordAttack3;
				break;
			case Player::eNoAnimationAttack:	//�Ō�̍U�����I�������
				owner.SetPlayerAttackAnimation("StandBY", true);
				owner.SetAttackAnimationState(Player::eNoAnimationAttack);
				ChangeMove(owner);
				
				break;
			default:
				break;
			}

			m_attackAnimotionContinueFlg = false;
		}
		else
		{
			//�U���A�j���[�V�����𑱂��Ȃ��Ȃ�
			switch (owner.GetAttackAnimationState())
			{
			case Player::eAnimationAttack1:
				owner.SetPlayerAttackAnimation("Attack1StandBy", false);
				endAnimationFlg = true;
				
				break;
			case Player::eAnimationAttack2:	//�Q��ڂ̍U�����I�������
				owner.SetPlayerAttackAnimation("Attack2StandBy", false);
				endAnimationFlg = true;
				
				break;
			case Player::eAnimationAttack3:	//�Ō�̍U�����I�������
				owner.SetPlayerAttackAnimation("StandBY", true);

				ChangeMove(owner);
				break;

			case Player::eNoAnimationAttack:
				owner.SetPlayerAttackAnimation("StandBY", true);


				ChangeMove(owner);
				break;
			default:
				break;

				m_attackAnimotionContinueFlg = false;
				owner.SetAttackAnimationState(Player::eNoAnimationAttack);
			}
		}
		m_count = 0;
	}

	// ���ʉ�
	PlaySwordSe(owner);
}

void PlayerAttack::SetAnimation(Player& owner)
{
	owner.SetNextAnimation("Attack1", false);
}

void PlayerAttack::ChangeMove(Player& player)
{
	player.ChangeActionState(std::make_shared<PlayerMove>());
}


void PlayerAttack::PlaySwordSe(Player& owner)
{
	m_count++;
	switch (m_attackState)
	{
	case SwordAttack1:
		if (m_count == 20)
		{
			KdGameAudioManager::GetInstance().sowrdSlasheSE();
		}

		break;
	case SwordAttack2:
		if (m_count == 28)
		{
			KdGameAudioManager::GetInstance().sowrdSlasheSE();
		}

		break;
	case SwordAttack3:
		if (m_count == 10)
		{
			KdGameAudioManager::GetInstance().sowrdSlasheSE();
		}

		if (m_count == 40)
		{
			KdGameAudioManager::GetInstance().sowrdSlasheSE();
		}

		break;
	default:
		break;
	}
}
