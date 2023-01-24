#pragma once

class KdPostProcessShader
{
public:
	KdPostProcessShader() {}
	~KdPostProcessShader() {}

	void Init();
	void Release();

	void SetToDevice();

	void BeginBlur();
	void EndBlur();

	void Draw(const KdTexture& tex);

	// 
	void GenerateLightBloomTexture(KdTexture& dst, const KdTexture& src);
	void GenerateBlurTexture(KdTexture& dst, const KdTexture& src);

	void SetBlurInfo(const Math::Vector2& uvOffset, int numPixel)
	{
		m_cb0_blur.Work().m_numPixel = numPixel;	// CPU上の値変更
		m_cb0_blur.Work().m_uvOffset = uvOffset;

		m_cb0_blur.Write();						// GPUに書き込む
	}

private:
	ID3D11VertexShader* m_pVS = nullptr;			// 頂点シェーダー
	ID3D11InputLayout* m_pInputLayOut = nullptr;	// 頂点のデータ
	ID3D11PixelShader* m_pPS = nullptr;				// ピクセルシェーダー
	ID3D11PixelShader* m_pPS_Blur = nullptr;		// ピクセルシェーダー(ブラー)

	struct cb_blur
	{
		Math::Vector2 m_uvOffset;					// テクセルサイズ * ずらす方向
		int m_numPixel = 1;							// 両サイドに何ピクセルずつぼかすか
		float _blank = 0;
	};

	KdConstantBuffer<cb_blur> m_cb0_blur;			// コンスタントバッファ制作：初期化と開放もセットで
};