#include "KdSceneChangeForClick.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"

void KdSceneChangeForClick::Update()
{
	//左クリックの検出
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (!m_isClicked)
		{
			//シーンの切り替えリクエスト
			Application::Instance().WorkGameSystem()->RequestChangeScene(m_nextSceneName,true);
		}
		m_isClicked = true;
	}
	else
	{
		m_isClicked = false;
	}
}
