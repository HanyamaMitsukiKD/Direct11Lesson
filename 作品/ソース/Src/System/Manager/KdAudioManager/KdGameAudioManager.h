#pragma once

class KdGameAudioManager
{
public:
	~KdGameAudioManager() {}

	static KdGameAudioManager& GetInstance()
	{
		static KdGameAudioManager instance;
		return instance;
	}

	void Update();
	void Init();


	void SoundStop();

	// BGM
	void PlayTitleBGM();
	void PlayGameBGM();
	
	// SE
	void BomSE();			//爆発音
	void mailSE();			//
	void messageSE();		//
	void avoidanceSE();		//
	void takkuruMoveSE();	//
	void takkuruStandSE();	//
	void sowrdSlasheSE();	//
	void sceneChangeSE();	//
	void counterAttackSE();	// 
	void attackStartSE();	// 攻撃開始

	std::shared_ptr<KdSoundInstance> GetTakkuruSE() { return m_aTakkuru; }

private:
	KdGameAudioManager() {}

	KdAudioManager m_audioManager;

	// Player
	std::shared_ptr<KdSoundInstance> m_aWalk;			//Player移動音
	std::shared_ptr<KdSoundInstance> m_aCounterCount;	//カウンターカウント音
	std::shared_ptr<KdSoundInstance> m_aTakkuru;		//エネミーとプレイヤーの激突音
	bool m_counterCountSEFlg = false;
	bool m_UltSEFlg = false;
	bool m_TakkuruSEFlg = false;

	// エネミー
	std::shared_ptr<KdSoundInstance> m_aMisail;			//ミサイル音
	std::shared_ptr<KdSoundInstance> m_aMisailStanby;	//ミサイル構え音
	std::shared_ptr<KdSoundInstance> m_aSword;			//剣が激突した音
	bool m_MisailSEFlg = false;
	bool m_SwordSEFlg = false;
	int m_MisailStanbyCount = 0;

	std::shared_ptr<KdSoundInstance> m_aThrowMotion;	//スローモーション音

};