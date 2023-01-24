
//===========================================
//
// 定数バッファ
//  ゲームプログラム側から持ってくるデータ
//
//===========================================

// 定数バッファ(オブジェクト単位更新)
cbuffer cbPerObject : register(b0)
{
	// UV関係
	float2 g_UVOffset;
	float2 g_UVTiling;
	
    float g_DiscordValue;
};

// 定数バッファ(メッシュ単位更新)
cbuffer cbPerMesh : register(b1)
{
	row_major float4x4 g_mWorld; // ワールド変換行列
};

// 定数バッファ(マテリアル)
cbuffer cbMaterial : register(b2)
{
	float4 g_BaseColor;	// ベース色
	float3 g_Emissive;	// 自己発光色
};

//===========================================
// 頂点シェーダから出力するデータ
//===========================================
struct VSOutput
{
	float4 Pos : SV_Position;   // 射影座標
	float2 UV : TEXCOORD0;      // UV座標
	float4 Color : TEXCOORD1;   // 色
    float3 wPos : TEXCOORD2; // 3Dワールド座標
    float3 wN : TEXCOORD3; // 3Dワールド法線
};
