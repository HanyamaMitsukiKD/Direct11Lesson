#include "../inc_KdCommon.hlsli"
#include "inc_KdStandardShader.hlsli"

// テクスチャ
Texture2D g_baseTex : register(t0); // ベースカラーテクスチャ
Texture2D g_emissiveTex : register(t1); // エミッシブテクスチャ
Texture2D g_mrTex : register(t2); // メタリック/ラフネステクスチャ
Texture2D g_normalTex : register(t3); // 法線マップ

Texture2D g_toonTex : register(t4); //トゥーンテクスチャ

// サンプラ
SamplerState g_ss : register(s0);
SamplerState g_clampSS : register(s1);

// BlinnPhong NDF
// ・lightDir    … ライトの方向
// ・vCam        … ピクセルからカメラへの方向
// ・normal      … 法線
// ・specPower   … 反射の鋭さ
float BlinnPhong(float3 lightDir, float3 vCam, float3 normal, float specPower)
{
    float3 H = normalize(-lightDir + vCam);
	// カメラの角度差(0～1)
    float NdotH = saturate(dot(normal, H));
    float spec = pow(NdotH, specPower);

	// 正規化Blinn-Phong
    return spec * ((specPower + 2) / (2 * 3.1415926535));
}

//================================
// ピクセルシェーダ
//================================
float4 main(VSOutput In) : SV_Target0
{
    
	// カメラへの方向
    float3 vCam = g_CamPos - In.wPos;
    float camDist = length(vCam); // カメラ - ピクセル距離
    vCam = normalize(vCam);
	
	// 3種の法線から法線行列を作成
    row_major float3x3 mTBN =
    {
        normalize(In.wT),
		normalize(In.wB),
		normalize(In.wN),
    };

	// 法線マップから法線ベクトルを取得
    float3 wN = g_normalTex.Sample(g_ss, In.UV).rgb;

	// 0～1から-1～1へ変換
    wN = wN * 2.0 - 1.0;

	// 法線ベクトルをこのピクセル空間へ変換
    wN = normalize(mul(wN, mTBN));
	
	//------------------------------------------
	// 材質色
	//------------------------------------------

    float4 mr = g_mrTex.Sample(g_ss, In.UV);
	// 金属性
    float metallic = mr.b * g_Metallic;
	// 粗さ
    float roughness = mr.g * g_Roughness;
	
	// 材質の色
    float4 baseColor = g_baseTex.Sample(g_ss, In.UV) * g_BaseColor * In.Color;

	//------------------------------------------
	//
	// ライティング
	//
	//------------------------------------------
	// 最終的な色
    float3 color = 0;
	
		// 材質の拡散色　非金属ほど材質の色になり、金属ほど拡散色は無くなる
    const float3 baseDiffuse = lerp(baseColor.rgb, float3(0, 0, 0), metallic);
		// 材質の反射色　非金属ほど光の色をそのまま反射し、金属ほど材質の色が乗る
    const float3 baseSpecular = lerp(0.04, baseColor.rgb, metallic);

	//------------------
	// 平行光
	//------------------

	// Diffuse(拡散光) 正規化Lambertを使用
	{
		// 光の方向と法線の方向との角度さが光の強さになる
        float lightDiffuse = dot(-g_DL_Dir, wN);
        lightDiffuse = saturate(lightDiffuse); // マイナス値は0にする　0(暗)～1(明)になる

        if (g_ToonFlg)
        {
			// 正規化Lambert
            lightDiffuse /= 3.1415926535;

			// 光の色 * 材質の拡散色 * 透明率
            color += (g_DL_Color * lightDiffuse) * baseDiffuse * baseColor.a;
			
        }
        else
        {
			// トゥーン
			// 画像のUVのU（横座標:0～1）を持ってきている
            float3 toonColor = g_toonTex.Sample(g_clampSS, float2(lightDiffuse, 0.5)).rgb;
            toonColor /= 3.1415926535;

			// 光の色 * 材質の拡散色 * 透明率
            color += (g_DL_Color * toonColor) * baseDiffuse * baseColor.a;
        }
		
    }

	// Specular(反射色) 正規化Blinn-Phong NDFを使用
	{
		// 反射した光の強さを求める

		// ラフネスから、Blinn-Phong用のSpecularPowerを求める
        float smoothness = 1.0 - roughness; // ラフネスを逆転させ「滑らか」さにする
        float specPower = pow(2, 13 * smoothness); // 1～8192
	
		// Blinn-Phong NDF
        float spec = BlinnPhong(g_DL_Dir, vCam, wN, specPower);
	
		// 光の色 * 反射光の強さ * 材質の反射色 * 正規化係数 * 透明率
        color += (g_DL_Color * spec) * baseSpecular * baseColor.a;
    }

	//------------------
	// 環境光
	//------------------
    color += g_AmbientLight.rgb * baseColor.rgb * baseColor.a;

	//------------------
	// エミッシブ
	//------------------
    color += g_emissiveTex.Sample(g_ss, In.UV).rgb * g_Emissive;

	//------------------------------------------
	// 距離フォグ
	//------------------------------------------
    if (g_DistanceFogEnable && g_FogEnable)
    {
		// フォグ 1(近い)～0(遠い)
        float f = saturate(1.0 / exp(camDist * g_DistanceFogDensity));
		
        color.rgb = lerp(g_DistanceFogColor, color.rgb, f);
    }
	
	// ポイントライト-------------------------------------------------------
    // ポイントライトの方向ベクトル: 目的地 - 出発地
    float3 pointLightDir = In.wPos - g_PointLightPos;
    // 法線同士の向きで比べる:これは長さ
    float pointLightDist = length(In.wPos - g_PointLightPos); //ポイントライトからピクセルに向かう長さ
    
    normalize(pointLightDir);
    
	// 技範囲に使えるかも
	// 短かったら
    if (pointLightDist <= g_PointLightRadius)
    {
        // 法線情報             可読性のために-をここでつける
        float normalRate = dot(-pointLightDir, normalize(In.wN));
        // 法線の内積レート
        // 内積のベクトルが０なら映らない
        normalRate = saturate(normalRate);
        
        // 減衰
        float rate = saturate(pointLightDist / g_PointLightRadius); // 外に行くに連れて
        rate = pow(rate, 3);
        color.rgb += g_PointLightColor * rate * normalRate;
    }
    
    
	// グレイスケール
    float3 grayScale = color.r * 0.3
		+ color.g * 0.11
		+ color.b * 0.59;

    color.rgb = color.rgb +
		(grayScale - color.rgb)
		* g_GrayScaleValue;
	
	//------------------------------------------
	// 出力
	//------------------------------------------
    //return float4(wN, baseColor.a);	//法線マップ確認
	return float4(color, baseColor.a);
}