#include "KdGameSystem.h"
#include"System/GameObject/KdGameObject.h"
#include"Application/Game/GameObject/ModelObject/KdModelObject.h"
#include"Application/Game/GameObject/UIObject/KdUIObject.h"
#include"Application/Game/GameObject/EventObject/KdSceneChangeForClick.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/Player.h"
#include"Application/Game/GameObject/ModelObject/Character/Enemy/normalEnemy/normalEnemy.h"
#include"Application/Game/GameObject/TitleObject/Title.h"
#include"Application/Game/GameObject/StageSelectObject/StageSelect.h"
#include"Application/Game/GameObject/ResultObject/Result.h"
#include"Application/Game/GameObject/CameraObject/TPSCamera.h"
#include"Application/Game/GameObject/ModelObject/Stage/Stage1/Stage1.h"
#include"System/Manager/KdAssetManager/KdAssetManager.h"
#include"System/Manager/KdDebugManager/KdDebugManager.h"
#include"System/Manager/KdAudioManager/KdGameAudioManager.h"
#include"System/Manager/KdThrowManager/KdThrowManager.h"
#include"Application/Game/GameObject/ModelObject/Character/Player/CounterState/CounterState.h"


void KdGameSystem::Init()
{
	KdAssetManager::GetInstance().Init();
	KdGameAudioManager::GetInstance().Init();
	InitTitle();//タイトルから始まる

	//UI
	{
		m_UI = std::make_shared<KdUIObject>();
		m_UI->Init();
	}
	

	// レンダー
	m_postRTTex.CreateRenderTarget(
		D3D.GetBackBuffer()->GetWidth(),
		D3D.GetBackBuffer()->GetHeight());

	m_postDSTex.CreateDepthStencil(
		D3D.GetBackBuffer()->GetWidth(),
		D3D.GetBackBuffer()->GetHeight());

	m_postVP = {
		0.0f,0.0f,
		(float)m_postRTTex.GetWidth(),
		(float)m_postRTTex.GetHeight(),
		0.0f,1.0f };

	// デバック
	KdDebug::GetInstance().Init();

	// スローモーション
	KdThrowManager::GetInstance().Init();
}


void KdGameSystem::PreUpdate()
{
	////スローモーションフラグ
	if (!m_spPlayer) { return; }

	if (m_spPlayer->GetCounterCount() == 3)
	{
		CounterState::GetInstance().CounterFlgOn();
	}
	else
	{
		CounterState::GetInstance().CounterFlgOFF();
	}


	if (CounterState::GetInstance().GetCounterFlg())
	{
		CounterState::GetInstance().Update();
	}
	else
	{
		KdThrowManager::GetInstance().Update();
	}
}



void KdGameSystem::Update()
{
	//デバック
	KdDebug::GetInstance().Update();

	if (KdDebug::GetInstance().GetDebugActiveFlg()) { return; }

	for (std::shared_ptr<KdGameObject>& spObj : m_objects)
	{
		spObj->Update();
	}


	if (m_requestedChangeScene)
	{
		if (m_UI->GetReadyChangeSceneFlg())
		{
			ExcuteChangeScene();
		}
	}

	for (auto iter = m_objects.begin(); iter != m_objects.end();)
	{
		//イテレータの中身をみる
		if ((*iter)->NeedDelete())
		{
			iter = m_objects.erase(iter);

			continue;
		}
		++iter;
	}

	// 今のシーン判定
	m_UI->SetNowSceneName(m_nowSceneName);
	m_UI->Update();

	// 音関係
	KdGameAudioManager::GetInstance().Update();
}


void KdGameSystem::PreDraw()
{
	//バックバッファの初期化
	float col[4]{ 1.0f,1.0f,1.0f,1.0f };
	if (!KdThrowManager::GetInstance().GetThrowFlg())
	{
		D3D.WorkDevContext()->ClearRenderTargetView(
			D3D.WorkBackBuffer()->WorkRTView(), col);
	}

	//Zバッファの初期化
	D3D.WorkDevContext()->ClearDepthStencilView(
		D3D.WorkZBuffer()->WorkDSView(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	if (KdThrowManager::GetInstance().GetThrowFlg())
	{
		// レンダー関係初期化
		D3D.WorkDevContext()->ClearRenderTargetView(
			m_postRTTex.WorkRTView(), col);

		D3D.WorkDevContext()->ClearDepthStencilView(
			m_postDSTex.WorkDSView(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	if (m_spCamera)
	{
		m_spCamera->SetToShader();
	}
}

void KdGameSystem::Draw()
{
	// レンダー--------------------------------
	ID3D11DeviceContext* DevCon = D3D.WorkDevContext();

	// 現在のレンダーターゲットの保存
	ID3D11RenderTargetView* pSaveRTV = nullptr;
	ID3D11DepthStencilView* pSaveDSV = nullptr;
	UINT numViews = 1;
	D3D11_VIEWPORT saveVP;
	if (KdThrowManager::GetInstance().GetThrowFlg())
	{
		DevCon->OMGetRenderTargets(1, &pSaveRTV, &pSaveDSV);
		DevCon->RSGetViewports(&numViews, &saveVP);	

		DevCon->OMSetRenderTargets
		(1, m_postRTTex.WorkRTViewAddress(),
			m_postDSTex.WorkDSView());

		DevCon->RSSetViewports(1, &m_postVP);
	}
	//-------------------------------------------


	D3D.WorkShaderManager().m_standardShader.SetToDevice();

	for (std::shared_ptr<KdGameObject>& spObj : m_objects)
	{
		spObj->DrawLighting();
	}


	//エフェクト
	D3D.WorkShaderManager().m_effectShader.SetToDevice();
	for (std::shared_ptr<KdGameObject>& spObj : m_objects)
	{
		spObj->DrawNoLighting();
	}

	// レンダーターゲットの復元
	if (KdThrowManager::GetInstance().GetThrowFlg())
	{
		DevCon->OMSetRenderTargets(1, &pSaveRTV, pSaveDSV);
		DevCon->RSSetViewports(1, &saveVP);
	}

	//2D
	D3D.WorkShaderManager().m_spriteShader.Begin();

	// 残像:バックバッファーのクリアをコメント化
	if (KdThrowManager::GetInstance().GetThrowFlg())
	{
		D3D.WorkShaderManager().m_spriteShader.DrawTex(&m_postRTTex, 0, 0, nullptr, &Math::Color(1.0f, 1.0f, 1.0, 0.25f));
	}

	for (std::shared_ptr<KdGameObject>& spObj : m_objects)
	{
		spObj->DrawSprite();
	}

	m_UI->DrawSprite();

	D3D.WorkShaderManager().m_spriteShader.End();

	if (KdThrowManager::GetInstance().GetThrowFlg())
	{
		KdSafeRelease(pSaveRTV);
		KdSafeRelease(pSaveDSV);
	}

	//デバック
	//KdDebug::GetInstance().Draw();
}


void KdGameSystem::InitTitle()
{
	KdGameAudioManager::GetInstance().PlayTitleBGM();

	{
		std::shared_ptr<Title> m_obj = std::make_shared<Title>();
		m_obj->Init();
		m_objects.push_back(m_obj);
	}

	{
		std::shared_ptr<KdSceneChangeForClick> m_obj = std::make_shared<KdSceneChangeForClick>();
		m_obj->SetNextScene("Game");
		m_objects.push_back(m_obj);
	}

	m_nowSceneName = "Title";
}

void KdGameSystem::InitStageSlect()
{
	{
		std::shared_ptr<StageSelect> m_obj = std::make_shared<StageSelect>();
		m_obj->SetAsset("Asset/Texture/stageselect.png");
		m_objects.push_back(m_obj);
	}

	{
		std::shared_ptr<KdSceneChangeForClick> m_obj = std::make_shared<KdSceneChangeForClick>();
		m_obj->SetNextScene("Game");
		m_objects.push_back(m_obj);
	}
}

void KdGameSystem::InitGame()
{
	//閉じにくくなる
	ShowCursor(false);

	//空(というより地面以外)
	std::shared_ptr<KdModelObject> m_spSky = std::make_shared<KdModelObject>();
	m_spSky->SetModelDate(KdAssetManager::GetInstance().m_ModelAsset.GetModel_Stage());
	m_objects.push_back(m_spSky);

	Math::Vector3 playerInitPos = { 0,5,-50 };			//プレイヤー初期座標
	Math::Vector3 enemyInitPos = { 0,10,0 };			//エネミー初期座標

	//ステージ
	std::shared_ptr<Stage1>	m_spTerrain = std::make_shared<Stage1>();
	m_spTerrain->SetModelDate(KdAssetManager::GetInstance().m_ModelAsset.GetModel_StageGround());
	m_objects.push_back(m_spTerrain);

	//プレイヤー関係--------------------------------
	//プレイヤー
	std::shared_ptr<Player>	m_spRobot = std::make_shared<Player>();
	//m_spRobot->SetAsset("Asset/Model/Player/robot.gltf");
	m_spRobot->SetModelDate(KdAssetManager::GetInstance().m_ModelAsset.GetModel_Player());
	m_spRobot->SetPos(playerInitPos);
	m_objects.push_back(m_spRobot);
	m_spRobot->Init();
	m_spPlayer = m_spRobot;

	//敵1
	m_spNormalEnemy = std::make_shared<normalEnemy>();
	m_spNormalEnemy->SetModelDate(KdAssetManager::GetInstance().m_ModelAsset.GetModel_Enemy());
	m_spNormalEnemy->SetPos(enemyInitPos);
	m_spNormalEnemy->Init();
	m_spNormalEnemy->SetPlayer(m_spRobot);
	m_objects.push_back(m_spNormalEnemy);
	

	{
		m_UI->SetPlayer(m_spPlayer);
		m_UI->SetEnemy(m_spNormalEnemy);
		m_nowSceneName = "Game";
	}

	{
		m_spCamera = std::make_shared<KdCamera>();
		std::shared_ptr<TPSCamera> sp_TPS_Camera = std::make_shared<TPSCamera>();
		sp_TPS_Camera->Init();
		sp_TPS_Camera->SetTarget(m_spRobot);
		m_spCamera = sp_TPS_Camera->WorkCamera();
		sp_TPS_Camera->WorkCamera()->SetProjectionMatrix(60.0f);
		m_spCamera->SetToShader();
		m_spRobot->GetCamera(sp_TPS_Camera);
		m_objects.push_back(sp_TPS_Camera);
	}

	KdGameAudioManager::GetInstance().PlayGameBGM();
}

void KdGameSystem::InitResult()
{
	{
		std::shared_ptr<Result> m_obj = std::make_shared<Result>();
		m_obj->Init();
		m_objects.push_back(m_obj);
	}

	{
		std::shared_ptr<KdSceneChangeForClick> m_obj = std::make_shared<KdSceneChangeForClick>();
		m_obj->SetNextScene("Title");
		m_objects.push_back(m_obj);
	}
}



void KdGameSystem::RequestChangeScene(std::string_view nextSceneName, bool flg)
{
	m_requestedChangeScene = true;
	m_nextSceneName = nextSceneName;
	m_UI->RequestChangeScene(flg);
	KdGameAudioManager::GetInstance().sceneChangeSE();
}

void KdGameSystem::ExcuteChangeScene()
{
	if (m_nextSceneName == "Game")
	{
		KdGameAudioManager::GetInstance().SoundStop();
		m_objects.clear();
		InitGame();
	}
	else if (m_nextSceneName == "Title")
	{
		KdGameAudioManager::GetInstance().SoundStop();
		m_objects.clear();
		InitTitle();
	}
	else if (m_nextSceneName == "Result")
	{
		InitResult();
	}

	m_requestedChangeScene = false;
}

void KdGameSystem::Release()
{
	m_objects.clear();

	m_spCamera = nullptr;
	m_spNormalEnemy = nullptr;
	m_spPlayer = nullptr;
	m_UI = nullptr;
}