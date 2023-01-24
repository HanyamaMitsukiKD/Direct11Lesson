#include "../inc_KdCommon.hlsli"
#include "inc_KdEffectShader.hlsli"

// テクスチャ
Texture2D g_Tex : register(t0);
Texture2D g_EmissiveTex : register(t1); // エミッシブテクスチャ
Texture2D g_DisoolveTex : register(t10); // ディゾルブテクスチャ 10番にセット


// サンプラ
SamplerState g_ss : register(s0);

//================================
// エフェクト用ピクセルシェーダ
//================================
float4 main(VSOutput In) : SV_Target0
{
    // すべての処理の前にdiscord:テクスチャから色を取ってくるのをサンプラーという
    float discordValue =
	g_DisoolveTex.Sample(g_ss, In.UV).r; // 赤だったらとかの色条件でrgbが使える
    // テクスチャの色と比べる
    if (discordValue < g_DiscordValue)
    {
        discard;
    }
	
    
    float4 color = g_Tex.Sample(g_ss, In.UV)
	* In.Color
	* g_BaseColor;
	
    color.rgb += g_EmissiveTex.Sample(g_ss, In.UV).rgb * g_Emissive;


    return color;
}