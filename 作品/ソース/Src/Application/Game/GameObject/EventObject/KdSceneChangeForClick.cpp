#include "KdSceneChangeForClick.h"
#include"Application/main.h"
#include"Application/Game/KdGameSystem.h"

void KdSceneChangeForClick::Update()
{
	//���N���b�N�̌��o
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (!m_isClicked)
		{
			//�V�[���̐؂�ւ����N�G�X�g
			Application::Instance().WorkGameSystem()->RequestChangeScene(m_nextSceneName,true);
		}
		m_isClicked = true;
	}
	else
	{
		m_isClicked = false;
	}
}
