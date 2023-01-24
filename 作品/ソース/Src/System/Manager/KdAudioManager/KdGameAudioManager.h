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
	void BomSE();			//������
	void mailSE();			//
	void messageSE();		//
	void avoidanceSE();		//
	void takkuruMoveSE();	//
	void takkuruStandSE();	//
	void sowrdSlasheSE();	//
	void sceneChangeSE();	//
	void counterAttackSE();	// 
	void attackStartSE();	// �U���J�n

	std::shared_ptr<KdSoundInstance> GetTakkuruSE() { return m_aTakkuru; }

private:
	KdGameAudioManager() {}

	KdAudioManager m_audioManager;

	// Player
	std::shared_ptr<KdSoundInstance> m_aWalk;			//Player�ړ���
	std::shared_ptr<KdSoundInstance> m_aCounterCount;	//�J�E���^�[�J�E���g��
	std::shared_ptr<KdSoundInstance> m_aTakkuru;		//�G�l�~�[�ƃv���C���[�̌��ˉ�
	bool m_counterCountSEFlg = false;
	bool m_UltSEFlg = false;
	bool m_TakkuruSEFlg = false;

	// �G�l�~�[
	std::shared_ptr<KdSoundInstance> m_aMisail;			//�~�T�C����
	std::shared_ptr<KdSoundInstance> m_aMisailStanby;	//�~�T�C���\����
	std::shared_ptr<KdSoundInstance> m_aSword;			//�������˂�����
	bool m_MisailSEFlg = false;
	bool m_SwordSEFlg = false;
	int m_MisailStanbyCount = 0;

	std::shared_ptr<KdSoundInstance> m_aThrowMotion;	//�X���[���[�V������

};