#include "KdMaterial.h"

void KdMaterial::Load(std::string_view filePath)
{
	std::string fileDir = KdGetDirFromPath(filePath.data());

	std::string fileName = KdGetFileName(filePath.data(), true);

	SetTextures(fileDir, fileName + ".png", fileName + "_rm.png",
		fileName + "_ems.png", fileName + "_nml.png");
}

void KdMaterial::SetTextures(const std::string& fileDir, std::string_view baseTexName, std::string_view mrTexName, std::string_view emiTexName, std::string_view nmlTexName)
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// ��{�F
	if (FileExistence(fileDir + baseTexName.data()))
	{
		BaseColorTex = std::make_shared<KdTexture>();
		BaseColorTex->Load(fileDir + baseTexName.data());
	}

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// �������E�e��
	if (FileExistence(fileDir + mrTexName.data()))
	{
		MetallicRoughnessTex = std::make_shared<KdTexture>();
		MetallicRoughnessTex->Load(fileDir + mrTexName.data());

		Metallic = 1.0f;
	}

	// ���Ȕ����E�G�~�b�V�u
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	if (FileExistence(fileDir + emiTexName.data()))
	{
		EmissiveTex = std::make_shared<KdTexture>();
		EmissiveTex->Load(fileDir + emiTexName.data());

		Emissive = { 1.0f, 1.0f, 1.0f };
	}

	// �@��
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	if (FileExistence(fileDir + nmlTexName.data()))
	{
		NormalTex = std::make_shared<KdTexture>();
		NormalTex->Load(fileDir + nmlTexName.data());
	}
}