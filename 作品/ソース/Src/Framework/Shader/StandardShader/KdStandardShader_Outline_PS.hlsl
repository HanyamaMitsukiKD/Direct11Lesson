#include "../inc_KdCommon.hlsli"
#include "inc_KdStandardShader.hlsli"

Texture2D g_DisoolveTex : register(t10); // ディゾルブテクスチャ 10番にセット

// サンプラ
SamplerState g_ss : register(s0);


float4 main(VSOutput In) : SV_TARGET
{
    float4 color = { 1.0f, 1.0f, 0.0f, 0.2f };
    //float4 color = { 0.2f, 0.2f, 0.2f, 1.0f };
   
    return color;
}