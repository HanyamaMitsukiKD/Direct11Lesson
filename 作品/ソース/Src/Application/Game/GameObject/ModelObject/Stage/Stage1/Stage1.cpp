#include "Stage1.h"

const Math::Vector3 Stage1::GetPlayerInitPos()
{
	const KdModelWork::Node* pNode = m_spModel->FindNode("StartPoint_Player");

	// �w��m�[�h���݂�������

	Math::Matrix mat;
	Math::Matrix init;

	mat = pNode->m_worldTransform * init;


	return mat.Translation();
}
