#pragma once

//==========================================================
// マテリアル
//==========================================================
struct KdMaterial
{
	void Load(std::string_view filePath);

	void SetTextures(const std::string& fileDir, std::string_view baseTex, std::string_view mrTex, std::string_view emiTex, std::string_view nmlTex);

	//---------------------------------------
	// 材質データ
	//---------------------------------------

	// 名前
	std::string					Name;

	// 基本色
	std::shared_ptr<KdTexture>	BaseColorTex;				// 基本色テクスチャ
	Math::Vector4				BaseColor = { 1,1,1,1 };	// 基本色のスケーリング係数(RGBA)

	// 金属性、粗さ
	std::shared_ptr<KdTexture>	MetallicRoughnessTex;		// B:金属製 G:粗さ
	float						Metallic = 0.0f;			// 金属性のスケーリング係数
	float						Roughness = 1.0f;			// 粗さのスケーリング係数

	// 自己発光
	std::shared_ptr<KdTexture>	EmissiveTex;				// 自己発光テクスチャ
	Math::Vector3				Emissive = { 0,0,0 };		// 自己発光のスケーリング係数(RGB)

	// 法線マップ
	std::shared_ptr<KdTexture>	NormalTex;

	KdMaterial()
	{
		BaseColorTex = D3D.GetWhiteTex();
		MetallicRoughnessTex = D3D.GetWhiteTex();
		EmissiveTex = D3D.GetWhiteTex();
		NormalTex = D3D.GetNormalTex();
	}
};
