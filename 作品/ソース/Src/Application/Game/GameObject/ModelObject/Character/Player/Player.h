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
	
	//HP関係
	void SetPlayerDamageFlg(bool flg);
	const int	GetPlayerHP()	const { return m_playerHP; }
	const int	GetHPDecreaseValue()		const { return m_playerHPDecreaseValue; }
	const bool	GetDamageFlg()	const { return m_playerDamageFlg; }
	
	//ジャスト回避、QTE関係
	void QTECheck();
	const bool	GetQtePointFlg()const { return m_qtePointFlg; }
	

	// カウンター関係
	const int GetCounterCount()const { return m_CounterCount; }
	void ResetCounterCount() { m_CounterCount = 0; }
	const bool GetCounterCountFlg() const { return m_CounterCountFlg; }
	void SetCounterCountFlg(bool flg) { m_CounterCountFlg = flg; }

	// Playerのステータス
	const bool GetMoveFlg() const { return m_moveFlg; }
	void SetTrueMoveFlg()  {  m_moveFlg=true; }
	const bool GetAttackMotionFlg() const { return m_attackMotionFlg; }
	
	const float GetPlayerMoveSpd()const { return playerMove; }

	//プレイヤーの武器関係
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

	// アニメーションをセット
	void SetNextAnimation(std::string_view fileName, const bool flg);
	const bool GetAnimationEndFlg()const;

	//動き処理
	void UpdateMove(Math::Vector3& dstMove, Math::Vector3& nowPos);
	void UpdateDamageMove(Math::Vector3& dstMove, Math::Vector3& nowPos);
	void UpdateMatrix(Math::Vector3& nowPos);

	// 回避方向セット
	void SetAvoidanceDir(Math::Vector3& dir);
	void ChangeActionState(std::shared_ptr<StateBase>);
	void QTEExit();

	//ジャスト回避判定
	bool JustAvoidanceFlg();
	bool JustCounterFlg();


	//ジャスト回避、回避処理
	void UpdateAvoidance(Math::Vector3& dstMove, Math::Vector3& nowPos);
	void UpdateJustAvoidance(Math::Vector3& dstMove, Math::Vector3& nowPos);

	//当たり判定判定とそれに伴う座標の更新
	void UpdateGroundCollision();
	void UpdateEnemyFutureCircleCollision(Math::Vector3& dstMove, Math::Vector3& nowPos,Math::Vector3& moveVec);
	void UpdateCircleCollision();
	void UpdateCollision();

private:
	std::weak_ptr<CameraBase>		m_wpCamera;
	std::shared_ptr<Sword>			m_spSword;
	
	// カウンターカウント
	int m_CounterCount = 0;
	bool m_CounterCountFlg = false;
	
	// エフェクト関係
	int m_ultCount = 0;
	int m_ultZpos = 4;
	
	bool ultOption1Flg = false;
	bool ultOption2Flg = false;
	bool ultOption3Flg = false;

	//縁取り
	float m_outLineValue = 0;	

	// キャラクターの移動速度
	float	m_playerMoveSpd = 0.2f;
	bool	playerSpdFlg = true;
	float	playerMove = 0;						//移動量

	// エネルギー関係変数
	int		m_playerHP = 232;				//HP
	int		m_playerHPDecreaseValue = 0;	//受けたダメージ量
	bool	m_playerDamageFlg = false;		//エネルギー消費フラグ

	// ジャストカウンター
	bool justCounterFlg = false;

	// ダメージ関係
	Math::Vector3	m_damageDir;			//ぶっ飛ばされる方向
	
	Math::Vector3	m_AvoidanceDir;			//回避方向
	
	// enumで管理したほうがいいかもしれない変数
	bool JustAvoidanceUpdateFlg = false;	//ジャスト回避フラグ

	// キー
	bool spaseFlg = false;					//スペースキーフラグ
	bool m_qtePointFlg = false;				//QTEキーフラグ
	bool m_attackClickFlg = false;			//アタックキーフラグ

	// アニメーション
	bool m_attackMotionFlg = false;
	bool m_moveFlg = false;
	bool m_attackAnimotionContinueFlg = false;
	float m_MoveTransitionSpd = 0.0f;

	// UI
	std::shared_ptr<KdTexture>	m_spUlTTex = nullptr;
	Math::Vector2 UIpos;
	int m_ultRec = 1;
	int m_uilTexPos = 0;

	//スポットライト
	float m_spotRightRadius = 0.0f;;//スポットライトの半径


	ePlayerAttackAnimation m_paAtaackState = eNoAnimationAttack;

	
	std::shared_ptr<StateBase> m_nowActionState;
};