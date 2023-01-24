#pragma once

class KdGameSystem;

struct FPSController
{
	// FPS制御
	int		m_nowFps = 0;		// 現在のFPS値
	int		m_maxFps = 60;		// 最大FPS

	int m_nowGameSpeed = 1000;

	void Init();

	void UpdateStartTime();

	void Update();

	void SetGameSpeed(const float& speed)
	{
		m_gameSpeed = speed;
	}


private:

	void Control();

	void Monitoring();

	DWORD		m_frameStartTime = 0;		// フレームの開始時間

	int			m_fpsCnt = 0;				// FPS計測用カウント
	DWORD		m_fpsMonitorBeginTime = 0;	// FPS計測開始時間

	const int	kSecond = 1000;				// １秒のミリ秒
	const int	kThrowSecond = 2000;		// スローモーション時の１秒のミリ秒
	
	float		m_gameSpeed = 1.0f;			//ゲームのスピード
};

//============================================================
// アプリケーションクラス
//	APP.～ でどこからでもアクセス可能
//============================================================
class Application
{
// メンバ
public:

	// アプリケーション実行
	void Execute();

	// アプリケーション終了
	void End() { m_endFlag = true; }

	const KdWindow& GetWindow()const { return m_window; }

	int GetNowFPS() const { return m_fpsController.m_nowFps; }
	int GetMaxFPS() const { return m_fpsController.m_maxFps; }

	std::unique_ptr<KdGameSystem>& WorkGameSystem() { return  m_spGameSystem; }
	const std::unique_ptr<KdGameSystem>& GetGameSystem() { return  m_spGameSystem; }
	FPSController GetFPSController() { return  m_fpsController; }



private:
	std::unique_ptr<KdGameSystem> m_spGameSystem = nullptr;

	KdWindow		m_window;			// アプリケーションウィンドウ
	FPSController	m_fpsController;	// FPS制御


	// アプリケーション初期化
	bool Init(int w, int h);

	// アプリケーション解放
	void Release();

	bool		m_endFlag = false;		// ゲーム終了フラグ trueで終了

//=====================================================
// シングルトンパターン
//=====================================================
private:
	// 
	Application() {}

public:
	static Application &Instance()
	{
		static Application Instance;
		return Instance;
	}
};