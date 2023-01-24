#pragma once


#include"../CharacterBase.h"
#include"Application/Game/GameObject/CameraObject/TPSCamera.h"


class StateBase;
class CameraBase;
class TPSCamera;
class Sword;

class Player :public CharacterBase
{
public:
	Player() {}
	virtual ~Player() override {}

	void DrawLighting()override;
	void DrawSprite()override;
	void Update() override;
	void Init()override;

	eClassID GetClassID() const override { return ePlayer; }

	void Set2DAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName);
	
	void GetCamera(const std::shared_ptr<TPSCamera>& sp){m_wpCamera = sp;}
	
	//HP�֌W
	void SetPlayerDamageFlg(bool flg);
	const int	GetPlayerHP()	const { return m_playerHP; }
	const int	GetHPDecreaseValue()		const { return m_playerHPDecreaseValue; }
	const bool	GetDamageFlg()	const { return m_playerDamageFlg; }
	
	//�W���X�g����AQTE�֌W
	void QTECheck();
	const bool	GetQtePointFlg()const { return m_qtePointFlg; }
	

	// �J�E���^�[�֌W
	const int GetCounterCount()const { return m_CounterCount; }
	void ResetCounterCount() { m_CounterCount = 0; }
	const bool GetCounterCountFlg() const { return m_CounterCountFlg; }
	void SetCounterCountFlg(bool flg) { m_CounterCountFlg = flg; }

	// Player�̃X�e�[�^�X
	const bool GetMoveFlg() const { return m_moveFlg; }
	void SetTrueMoveFlg()  {  m_moveFlg=true; }
	const bool GetAttackMotionFlg() const { return m_attackMotionFlg; }
	
	const float GetPlayerMoveSpd()const { return playerMove; }

	//�v���C���[�̕���֌W
	const std::shared_ptr<const Sword> GetSword() const
	{
		return m_spSword;
	}

	void UpdateUltSecondOption();
	void UpdateCounterEffect();
	void UpdateUltAttackEffect();
	void UltInit();

	
	void JustRightCheck();

	enum ePlayerAttackAnimation
	{
		eNoAnimationAttack,
		eAnimationAttack1,
		eAnimationAttack2,
		eAnimationAttack3
	};

	void SetPlayerAttackAnimation(std::string_view fileName,const bool loopFlg);

	ePlayerAttackAnimation GetAttackAnimationState() { return m_paAtaackState; }
	void SetAttackAnimationState(const ePlayerAttackAnimation state) {  m_paAtaackState= state; }
	const bool GetAnimotionContinueFlg() const { return m_attackAnimotionContinueFlg; }

	const float GetOutLineValue()const { return m_outLineValue; }

	// �A�j���[�V�������Z�b�g
	void SetNextAnimation(std::string_view fileName, const bool flg);
	const bool GetAnimationEndFlg()const;

	//��������
	void UpdateMove(Math::Vector3& dstMove, Math::Vector3& nowPos);
	void UpdateDamageMove(Math::Vector3& dstMove, Math::Vector3& nowPos);
	void UpdateMatrix(Math::Vector3& nowPos);

	// �������Z�b�g
	void SetAvoidanceDir(Math::Vector3& dir);
	void ChangeActionState(std::shared_ptr<StateBase>);
	void QTEExit();

	//�W���X�g��𔻒�
	bool JustAvoidanceFlg();
	bool JustCounterFlg();


	//�W���X�g����A�������
	void UpdateAvoidance(Math::Vector3& dstMove, Math::Vector3& nowPos);
	void UpdateJustAvoidance(Math::Vector3& dstMove, Math::Vector3& nowPos);

	//�����蔻�蔻��Ƃ���ɔ������W�̍X�V
	void UpdateGroundCollision();
	void UpdateEnemyFutureCircleCollision(Math::Vector3& dstMove, Math::Vector3& nowPos,Math::Vector3& moveVec);
	void UpdateCircleCollision();
	void UpdateCollision();

private:
	std::weak_ptr<CameraBase>		m_wpCamera;
	std::shared_ptr<Sword>			m_spSword;
	
	// �J�E���^�[�J�E���g
	int m_CounterCount = 0;
	bool m_CounterCountFlg = false;
	
	// �G�t�F�N�g�֌W
	int m_ultCount = 0;
	int m_ultZpos = 4;
	
	bool ultOption1Flg = false;
	bool ultOption2Flg = false;
	bool ultOption3Flg = false;

	//�����
	float m_outLineValue = 0;	

	// �L�����N�^�[�̈ړ����x
	float	m_playerMoveSpd = 0.2f;
	bool	playerSpdFlg = true;
	float	playerMove = 0;						//�ړ���

	// �G�l���M�[�֌W�ϐ�
	int		m_playerHP = 232;				//HP
	int		m_playerHPDecreaseValue = 0;	//�󂯂��_���[�W��
	bool	m_playerDamageFlg = false;		//�G�l���M�[����t���O

	// �W���X�g�J�E���^�[
	bool justCounterFlg = false;

	// �_���[�W�֌W
	Math::Vector3	m_damageDir;			//�Ԃ���΂�������
	
	Math::Vector3	m_AvoidanceDir;			//������
	
	// enum�ŊǗ������ق���������������Ȃ��ϐ�
	bool JustAvoidanceUpdateFlg = false;	//�W���X�g����t���O

	// �L�[
	bool spaseFlg = false;					//�X�y�[�X�L�[�t���O
	bool m_qtePointFlg = false;				//QTE�L�[�t���O
	bool m_attackClickFlg = false;			//�A�^�b�N�L�[�t���O

	// �A�j���[�V����
	bool m_attackMotionFlg = false;
	bool m_moveFlg = false;
	bool m_attackAnimotionContinueFlg = false;
	float m_MoveTransitionSpd = 0.0f;

	// UI
	std::shared_ptr<KdTexture>	m_spUlTTex = nullptr;
	Math::Vector2 UIpos;
	int m_ultRec = 1;
	int m_uilTexPos = 0;

	//�X�|�b�g���C�g
	float m_spotRightRadius = 0.0f;;//�X�|�b�g���C�g�̔��a


	ePlayerAttackAnimation m_paAtaackState = eNoAnimationAttack;

	
	std::shared_ptr<StateBase> m_nowActionState;
};