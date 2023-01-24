#include "main.h"
#include"Application/Game/KdGameSystem.h"
#include"System/Manager/KdThrowManager/KdThrowManager.h"

//===================================================================
// メイン
//===================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// COM初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// 実行
	//===================================================================
	Application::Instance().Execute();

	// COM解放
	CoUninitialize();

	return 0;
}


// アプリケーション初期設定
bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (m_window.Create(w, h, "サシロボ", "Window") == false) {
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
	/*if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		bFullScreen = true;
	}*/

	//===================================================================
	// Direct3D初期化
	//===================================================================

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
	#ifdef _DEBUG
	deviceDebugMode = true;
	#endif

	// Direct3D初期化
	std::string errorMsg;
	if (D3D.Init(m_window.GetWndHandle(), w, h, deviceDebugMode, errorMsg) == false) {
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		return false;
	}

	// フルスクリーン設定
	if (bFullScreen) {
		D3D.WorkSwapChain()->SetFullscreenState(TRUE, 0);
	}

	//===================================================================
	// imgui
	//===================================================================
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// Setup Dear ImGui style
	// ImGui::StyleColorsDark();
	ImGui::StyleColorsClassic();
	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_window.GetWndHandle());
	ImGui_ImplDX11_Init(D3D.WorkDev(), D3D.WorkDevContext());

	#include "imgui/ja_glyph_ranges.h"
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	// 日本語対応
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);


	//===================================================================
	// オーディオ初期化
	//===================================================================

	return true;
}

// アプリケーション終了
void Application::Release()
{
	m_spGameSystem = nullptr;

	// imgui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	// Direct3D解放
	D3D.Release();

	// ウィンドウ削除
	m_window.Release();
}

// アプリケーション実行
void Application::Execute()
{
	//===================================================================
	// 初期設定(ウィンドウ作成、Direct3D初期化など)
	//===================================================================
	if (!Application::Instance().Init(1280, 720))
	{
		return;
	}

	//===================================================================
	// ゲームループ
	//===================================================================

	// 時間
	m_fpsController.Init();

	// 初期化
	m_spGameSystem = std::make_unique<KdGameSystem>();
	m_spGameSystem->Init();

	// ループ
	while (1)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			ShowCursor(true);
			if (MessageBoxA(m_window.GetWndHandle(), "本当にゲームを終了しますか？",
				"確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
			{
				break;
			}
			else
			{
				ShowCursor(false);
			}

			SetCursorPos(640, 320);
		}
		

		// 処理開始時間Get
		m_fpsController.UpdateStartTime();

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag)
		{ 
			break;
		}

		//=========================================
		//
		// ウィンドウ関係の処理
		//
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (m_window.IsCreated() == false)
		{
			break;
		}

		// ImGui開始
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//=========================================
		//
		// ゲーム処理
		//
		//=========================================
		


		// ゲーム更新
		m_spGameSystem->PreUpdate();
		m_fpsController.SetGameSpeed(KdThrowManager::GetInstance().GetSpeed());
		m_spGameSystem->Update();
	
		// ゲーム描画
		m_spGameSystem->PreDraw();
		m_spGameSystem->Draw();

	

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		
		
		// BackBuffer -> 画面表示
		D3D.WorkSwapChain()->Present(0, 0);

		//=========================================
		//
		// フレームレート制御・計測
		//
		//=========================================
		m_fpsController.Update();
	}
	

	//===================================================================
	// アプリケーション解放
	//===================================================================
	Release();
}

void FPSController::Init()
{
	m_fpsMonitorBeginTime = timeGetTime();
}

void FPSController::UpdateStartTime()
{
	m_frameStartTime = timeGetTime();
}

void FPSController::Update()
{
	Control();

	Monitoring();
}

// FPS制御
void FPSController::Control()
{
	// 処理終了時間Get
	DWORD frameProcessEndTime = timeGetTime();

	// 1フレームで経過すべき時間
	// スローモーション時(0:1=5000:1000)
	m_nowGameSpeed = (int)((kThrowSecond - (kThrowSecond * m_gameSpeed)) + kSecond);

	DWORD timePerFrame = m_nowGameSpeed /m_maxFps;
	
	if (frameProcessEndTime - m_frameStartTime < timePerFrame)
	{
		// 1秒間にMaxFPS回数以上処理が回らないように待機する
		Sleep(timePerFrame - (frameProcessEndTime - m_frameStartTime));
	}
}

// 現在のFPS計測
void FPSController::Monitoring()
{
	constexpr float kFpsRefreshFrame = 500;		// FPS計測のタイミング　0.5秒おき

	m_fpsCnt++;

	// 0.5秒おきに FPS計測
	if (m_frameStartTime - m_fpsMonitorBeginTime >= kFpsRefreshFrame)
	{
		// 現在のFPS算出
		m_nowFps = (m_fpsCnt * kSecond) / (m_frameStartTime - m_fpsMonitorBeginTime);

		m_fpsMonitorBeginTime = m_frameStartTime;
		
		m_fpsCnt = 0;
	}
}
