#pragma once
#include"System/GameObject/KdGameObject.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/normalEnemy/normalEnemy.h"

class KdUIObject :public KdGameObject
{
public:
	KdUIObject() {};
	virtual ~KdUIObject() override {};

	virtual void Init()override;
	virtual void Update()override;
	virtual void DrawSprite() override;

	void DrawPlayerHP();
	void DrawPlayerUltGauge();
	void DrawPlayerQTE();

	void DrawEnemyHP();

	void SetAsset(std::shared_ptr<KdTexture>& sp_Tex, std::string_view fileName);

	void SetPlayer(const std::shared_ptr<Player> player)
	{
		m_wpPlayer = player;
	}

	void SetEnemy(const std::shared_ptr<normalEnemy> enemy)
	{
		m_wpEnemy = enemy;
	}

	void PlayerHPMove();
	void EnemyHPMove();
	void UIEaseOutCubic(float& speed, float& animetion);

	void SetNowSceneName(std::string_view fileName);
	void RequestChangeScene(bool sceneChangeflg);

	// �E���g�Q�[�W�p�̃C�[�W���O�̐i���x��������
	void UltGugesProgressExit();

	const bool GetReadyChangeSceneFlg() const { return m_readyChangeGameSceneFlg; }

private:
	std::string m_SceneName = "";

	// �V�[���`�F���W
	std::shared_ptr<KdTexture>	m_spSceneChengeTex = nullptr;	// �V�[���`�F���W�摜
	std::shared_ptr<KdTexture>	m_spNowLoadingTex = nullptr;	// NowLoading�摜

	Math::Vector2 m_sceneChengePos;								// �V�[���`�F���W�摜�̍��W
	Math::Vector2 m_pNowLoadingPos;								// NowLoading�摜�̍��W
	int m_nowLoadingRec = 0;

	float m_sceneChengePosSpd;									// �ړ��X�s�[�h
	int  m_nowLoadingCount;										// ���[�h����
	bool m_requestChangeSceneFlg = false;						// �V�[���`�F���W�����Ăق������̃t���O
	bool m_readyChangeGameSceneFlg = false;						// �Q�[���V�[�����ς���Ă������̃t���O



	// PlayerUI----------------------------------------------------------------------------
	std::shared_ptr<KdTexture>	m_spPlayerHPTex = nullptr;			// �̗�
	std::shared_ptr<KdTexture>	m_spPlayerHPGaugeTex = nullptr;		// �̗̓Q�[�W
	std::shared_ptr<KdTexture>	m_spPlayerUltGaugeTex = nullptr;	// �E���g�Q�[�W
	std::shared_ptr<KdTexture>	m_spULTTex = nullptr;				// ult�\��
	std::shared_ptr<KdTexture>	m_spQTETex = nullptr;				// QTE
	std::shared_ptr<KdTexture>	m_spQTEPointTex = nullptr;			// QTE�J�[�\��
	Math::Matrix m_qtePointMat;										// QTE�摜�p�̍s��
	bool m_qtePointPushFlg;											// QTE�������ꂽ���t���O
	float m_qteAlpha = 1.0f;										// QTE�摜�̓����x

	//PlyaerHP�摜���W
	Math::Vector2 playerHPTexPos = { -431 , -280 };
	int playerHPGaugeTexX = 232;
	Math::Vector2 playerHPGauge = { 232,50 };

	float m_ultGugesProgress = 0;		//�i���x
	float m_ultGugeSpeed = 0;
	// ----------------------------------------------------------------------------------



	// EnemyUI ------------------------------------------------------------------------------
	std::shared_ptr<KdTexture>	m_spEnemyHPTex = nullptr;			//�̗�UI
	std::shared_ptr<KdTexture>	m_spEnemyHPGaugeTex = nullptr;		//HP�Q�[�W
	//------------------------------------------------------------------------------



	//�G�l�~�[HP�Q�[�W
	//���W
	Math::Vector2 m_enemyHpTexPos = { -5,300 };
	//�e�N�X�`���T�C�Y
	int enemyHpGaugeTexInitSizeX = 728;
	int enemyHpGaugeTexSizeX = 728;
	int enemyHpGaugeTexSizeY = 28;

	float energySpeed = 1.0f;
	float animationEnergySpeed = 0.0f;

	float hpGaugeSpeed = 1.0f;
	float animationHpSpeed = 0.0f;

	std::weak_ptr<Player>		m_wpPlayer;				//�v���C���[���
	std::weak_ptr<normalEnemy>	m_wpEnemy;				//�G�l�~�[���

	// ------------------------------------------------------------------------------------------
};