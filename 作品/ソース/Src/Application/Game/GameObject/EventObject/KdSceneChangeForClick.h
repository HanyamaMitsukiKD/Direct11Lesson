#pragma once
#include"System/GameObject/KdGameObject.h"

class KdSceneChangeForClick:public KdGameObject
{
public:
	KdSceneChangeForClick(){};
	~KdSceneChangeForClick()override {};

	void Update()override;

	//string_view‚Íˆø”‚¾‚¯
	void SetNextScene(std::string_view nextSceneName) { 
		m_nextSceneName = nextSceneName;
	}

private:
	bool m_isClicked = true;
	std::string m_nextSceneName = "";
};