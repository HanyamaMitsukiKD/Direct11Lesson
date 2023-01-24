#include "KdPostProcessShader.hlsli"

// テクスチャ
Texture2D g_Tex : register(t0);

// サンプラ
SamplerState g_ss : register(s0);

float4 main(VSOutput In) : SV_TARGET
{
    return g_Tex.Sample(g_ss, In.UV);
}