#pragma once

class KdGameObject;
class Player;
class normalEnemy;
class KdUIObject;

class KdGameSystem
{
public:
	KdGameSystem(){};
	~KdGameSystem() { Release(); }

	void Init();
	void PreUpdate();
	void Update();
	void PreDraw();	
	void Draw();
	
	void RequestChangeScene(std::string_view nextSceneName, bool flg);
	
	const float GetGravity() { return m_gravity; }

	const std::list<std::shared_ptr<KdGameObject>>& GetGameObjects() const
	{
		return m_objects;
	}

	std::list<std::shared_ptr<KdGameObject>>& WorkGameObjects() 
	{
		return m_objects;
	}

	const std::shared_ptr<Player>& GetPlayer() const
	{
		return m_spPlayer;
	}

	std::shared_ptr<Player>& WorkPlayer() 
	{
		return m_spPlayer;
	}

	const std::shared_ptr<normalEnemy>& GetEnemy() const
	{
		return m_spNormalEnemy;
	}

	void AddObject(const std::shared_ptr<KdGameObject>& obj)
	{
		m_objects.push_back(obj);
	}

	
private:
	//シーンチェンジ
	void InitTitle();
	void InitStageSlect();
	void InitGame();
	void InitResult();

	void ExcuteChangeScene();	//シーンチェンジ関数

	void Release();

private:
	std::list<std::shared_ptr<KdGameObject>> m_objects;		//ゲームオブジェ

	std::shared_ptr<KdCamera>		m_spCamera	= nullptr;
	std::shared_ptr<Player>			m_spPlayer = nullptr;
	std::shared_ptr<normalEnemy>	m_spNormalEnemy = nullptr;

	// UI
	std::shared_ptr<KdUIObject> m_UI = nullptr;

	
	//シーン変更のための機能
	bool m_requestedChangeScene = false;
	std::string m_nextSceneName = "";
	std::string m_nowSceneName = "";
	
	//重力
	float m_gravity = -0.01f;

	KdTexture m_postRTTex;		//レンダーターゲット
	KdTexture m_postDSTex;		//デプスステンシル（深度）
	D3D11_VIEWPORT m_postVP;	//ビューポート用
};