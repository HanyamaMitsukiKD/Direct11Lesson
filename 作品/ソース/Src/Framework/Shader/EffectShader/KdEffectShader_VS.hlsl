#include "../inc_KdCommon.hlsli"
#include "inc_KdEffectShader.hlsli"

//================================
// エフェクト用 頂点シェーダ
//================================
VSOutput main(float4 pos : POSITION,	// 頂点座標
			  float2 uv : TEXCOORD0,	// テクスチャUV座標
			  float3 normal : NORMAL,   // 法線 : 名前を合わせる
			  float4 color : COLOR		// 色
){
	VSOutput Out;

	// 座標変換
	Out.Pos = mul(pos, g_mWorld);		// ローカル座標系 -> ワールド座標系へ変換
    Out.wPos = Out.Pos; //ワールド座標セット
	Out.Pos = mul(Out.Pos, g_mView);	// ワールド座標系 -> ビュー座標系へ変換
	Out.Pos = mul(Out.Pos, g_mProj);	// ビュー座標系 -> 射影座標系へ変換

	// UV座標
	Out.UV = uv * g_UVTiling + g_UVOffset;

	// 色
	Out.Color = color;
	
	// ワールドに変換:(float3x3) = 3 * 3 になる（シェーダーだけ）
    Out.wN = mul(normal, (float3x3) g_mWorld);

	// 出力
	return Out;
}
