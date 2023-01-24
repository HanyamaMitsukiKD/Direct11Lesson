#pragma once

//==========================================================
// �}�e���A��
//==========================================================
struct KdMaterial
{
	void Load(std::string_view filePath);

	void SetTextures(const std::string& fileDir, std::string_view baseTex, std::string_view mrTex, std::string_view emiTex, std::string_view nmlTex);

	//---------------------------------------
	// �ގ��f�[�^
	//---------------------------------------

	// ���O
	std::string					Name;

	// ��{�F
	std::shared_ptr<KdTexture>	BaseColorTex;				// ��{�F�e�N�X�`��
	Math::Vector4				BaseColor = { 1,1,1,1 };	// ��{�F�̃X�P�[�����O�W��(RGBA)

	// �������A�e��
	std::shared_ptr<KdTexture>	MetallicRoughnessTex;		// B:������ G:�e��
	float						Metallic = 0.0f;			// �������̃X�P�[�����O�W��
	float						Roughness = 1.0f;			// �e���̃X�P�[�����O�W��

	// ���Ȕ���
	std::shared_ptr<KdTexture>	EmissiveTex;				// ���Ȕ����e�N�X�`��
	Math::Vector3				Emissive = { 0,0,0 };		// ���Ȕ����̃X�P�[�����O�W��(RGB)

	// �@���}�b�v
	std::shared_ptr<KdTexture>	NormalTex;

	KdMaterial()
	{
		BaseColorTex = D3D.GetWhiteTex();
		MetallicRoughnessTex = D3D.GetWhiteTex();
		EmissiveTex = D3D.GetWhiteTex();
		NormalTex = D3D.GetNormalTex();
	}
};
