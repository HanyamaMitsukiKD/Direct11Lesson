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

	// ウルトゲージ用のイージングの進捗度を初期化
	void UltGugesProgressExit();

	const bool GetReadyChangeSceneFlg() const { return m_readyChangeGameSceneFlg; }

private:
	std::string m_SceneName = "";

	// シーンチェンジ
	std::shared_ptr<KdTexture>	m_spSceneChengeTex = nullptr;	// シーンチェンジ画像
	std::shared_ptr<KdTexture>	m_spNowLoadingTex = nullptr;	// NowLoading画像

	Math::Vector2 m_sceneChengePos;								// シーンチェンジ画像の座標
	Math::Vector2 m_pNowLoadingPos;								// NowLoading画像の座標
	int m_nowLoadingRec = 0;

	float m_sceneChengePosSpd;									// 移動スピード
	int  m_nowLoadingCount;										// ロード時間
	bool m_requestChangeSceneFlg = false;						// シーンチェンジをしてほしいかのフラグ
	bool m_readyChangeGameSceneFlg = false;						// ゲームシーンが変わっていいかのフラグ



	// PlayerUI----------------------------------------------------------------------------
	std::shared_ptr<KdTexture>	m_spPlayerHPTex = nullptr;			// 体力
	std::shared_ptr<KdTexture>	m_spPlayerHPGaugeTex = nullptr;		// 体力ゲージ
	std::shared_ptr<KdTexture>	m_spPlayerUltGaugeTex = nullptr;	// ウルトゲージ
	std::shared_ptr<KdTexture>	m_spULTTex = nullptr;				// ult表現
	std::shared_ptr<KdTexture>	m_spQTETex = nullptr;				// QTE
	std::shared_ptr<KdTexture>	m_spQTEPointTex = nullptr;			// QTEカーソル
	Math::Matrix m_qtePointMat;										// QTE画像用の行列
	bool m_qtePointPushFlg;											// QTEが押されたかフラグ
	float m_qteAlpha = 1.0f;										// QTE画像の透明度

	//PlyaerHP画像座標
	Math::Vector2 playerHPTexPos = { -431 , -280 };
	int playerHPGaugeTexX = 232;
	Math::Vector2 playerHPGauge = { 232,50 };

	float m_ultGugesProgress = 0;		//進捗度
	float m_ultGugeSpeed = 0;
	// ----------------------------------------------------------------------------------



	// EnemyUI ------------------------------------------------------------------------------
	std::shared_ptr<KdTexture>	m_spEnemyHPTex = nullptr;			//体力UI
	std::shared_ptr<KdTexture>	m_spEnemyHPGaugeTex = nullptr;		//HPゲージ
	//------------------------------------------------------------------------------



	//エネミーHPゲージ
	//座標
	Math::Vector2 m_enemyHpTexPos = { -5,300 };
	//テクスチャサイズ
	int enemyHpGaugeTexInitSizeX = 728;
	int enemyHpGaugeTexSizeX = 728;
	int enemyHpGaugeTexSizeY = 28;

	float energySpeed = 1.0f;
	float animationEnergySpeed = 0.0f;

	float hpGaugeSpeed = 1.0f;
	float animationHpSpeed = 0.0f;

	std::weak_ptr<Player>		m_wpPlayer;				//プレイヤー情報
	std::weak_ptr<normalEnemy>	m_wpEnemy;				//エネミー情報

	// ------------------------------------------------------------------------------------------
};