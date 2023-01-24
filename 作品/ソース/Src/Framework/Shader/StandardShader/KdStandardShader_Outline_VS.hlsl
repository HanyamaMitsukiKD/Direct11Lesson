#include "../inc_KdCommon.hlsli"
#include "inc_KdStandardShader.hlsli"

//================================
// 頂点シェーダ
//================================
VSOutput main(
	float4 pos : POSITION, // 頂点座標
	float2 uv : TEXCOORD0, // テクスチャUV座標
	float3 normal : NORMAL, // 法線ベクトル
	float4 color : COLOR, // 頂点色
	float3 tangent : TANGENT // 接線
)
{
    VSOutput Out;

	// 頂点座標を「法線方向」に少しずらす
    pos.xyz = pos.xyz + normal * g_OutLinePos;

	// 3D頂点座標を2Dへ変換
    Out.Pos = mul(pos, g_mWorld); // ワールド行列で変換してから、
    Out.Pos = mul(Out.Pos, g_mView); // カメラの逆行列(ビュー行列)で変換し、
    Out.Pos = mul(Out.Pos, g_mProj); // 射影行列で変換
   
	
	
    return Out;
}
