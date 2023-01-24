#include "KdPostProcessShader.hlsli"

// テクスチャ
Texture2D g_Tex : register(t0);

// サンプラ
SamplerState g_ss : register(s0);

cbuffer cbBlur : register(b0)
{
    float2 g_uvOffset;
    int g_numPixel;
};

float4 main(VSOutput In) : SV_TARGET
{
    float4 outCol = 0;
    
    for (int i = -g_numPixel; i <= g_numPixel; ++i)
    {
        float2 offsetUV = g_uvOffset * i;
        
        outCol += g_Tex.Sample(g_ss, In.UV + offsetUV); //左右の色を加算

    }
    
    return (outCol / (g_numPixel * 2 + 1.0));
}