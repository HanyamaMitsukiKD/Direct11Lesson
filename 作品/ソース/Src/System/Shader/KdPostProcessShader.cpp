#include "KdPostProcessShader.h"

void KdPostProcessShader::Init()
{
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdPostProcessShader_VS.inc"

// 頂点シェーダー作成
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_pVS))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//座標
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV座標
		};														 //12バイトから始まる

		// 頂点インプットレイアウト作成
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],
			layout.size(),
			compiledBuffer,
			sizeof(compiledBuffer),
			&m_pInputLayOut))
			) {
			assert(0 && "CreateInputLayout失敗");
			Release();
			return;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdPostProcessShader_PS.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_pPS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdPostProcessShader_PS_Blur.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_pPS_Blur))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return;
		}
	}

	m_cb0_blur.Create();
}

void KdPostProcessShader::Release()
{
	KdSafeRelease(m_pVS);
	KdSafeRelease(m_pInputLayOut);
	KdSafeRelease(m_pPS);
	KdSafeRelease(m_pPS_Blur);

	m_cb0_blur.Release();
}

void KdPostProcessShader::SetToDevice()
{
	//　切り替えはStateで管理したほうがいいかもしれない
	//  スタンダードのスキンメッシュを参考にする
	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();

	pDevCon->VSSetShader(m_pVS, 0, 0);			//
	pDevCon->IASetInputLayout(m_pInputLayOut);	//この２つはセット
	pDevCon->PSSetShader(m_pPS, 0, 0);

	//pDevCon->PSSetConstantBuffers(0, 1, nullptr);
}

void KdPostProcessShader::BeginBlur()
{
	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();

	pDevCon->VSSetShader(m_pVS, 0, 0);
	pDevCon->IASetInputLayout(m_pInputLayOut);
	pDevCon->PSSetShader(m_pPS_Blur, 0, 0);

	// ピクセルシェーダーで使う  0番にセット
	pDevCon->PSSetConstantBuffers(0, 1, m_cb0_blur.GetAddress());

	m_cb0_blur.Write();						// GPUに書き込む

	// サンプラーステートをクランプモードに切り替える
	// 一枚絵
	D3D.WorkShaderManager().ChangeSamplerState(0, D3D.WorkShaderManager().m_ss_Linear_Clamp);
}

void KdPostProcessShader::EndBlur()
{
	// サンプラーステートを元に戻す
	D3D.WorkShaderManager().UndoSamplerState();
}

void KdPostProcessShader::Draw(const KdTexture& tex)
{
	struct Vertex
	{
		Math::Vector3 pos;
		Math::Vector2 UV;
	};

	Vertex screenVertex[4];

	// 頂点の設定：受業の場合
	screenVertex[0] = { {-1,-1,0},{0,1} };
	screenVertex[1] = { {-1,1,0},{0,0} };
	screenVertex[2] = { {1,-1,0},{1,1} };
	screenVertex[3] = { {1,1,0},{1,0} };

	D3D.WorkDevContext()->PSSetShaderResources(0, 1, tex.WorkSRViewAddress());

	// ポリゴンを表示
	D3D.DrawVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		4, &screenVertex[0], sizeof(Vertex));
}

void KdPostProcessShader::GenerateBlurTexture(KdTexture& dst, const KdTexture& src)
{
	// コスト度外視で書いている
	BeginBlur();

	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();

	if (!pDevCon) { return; }

	// ①レンダーターゲットの切り替え
	//　 現状のRTV/DSV/VPの保存
	ID3D11RenderTargetView* pSaveRTV = nullptr;
	ID3D11DepthStencilView* pSaveDSV = nullptr;
	pDevCon->OMGetRenderTargets(1, &pSaveRTV, &pSaveDSV);

	// ビューポート（サイズ、アスペクト比）
	UINT numViews = 1;
	D3D11_VIEWPORT saveVP;
	pDevCon->RSGetViewports(&numViews, &saveVP);

	// 切り替える中間RTV/VPの作成
	//「dst」は「destination（デスティネーション）」の略語です。 意味は「目的地、行先、あて先」などになります
	KdTexture tempTex;
	tempTex.CreateRenderTarget(dst.GetWidth(), dst.GetHeight());

	D3D11_VIEWPORT dstVP = { 0,0,
		(float)dst.GetWidth(),
		(float)dst.GetHeight(),
		0.0f,1.0f };

	// 中間RTV/VPへの切り替え
	pDevCon->OMSetRenderTargets(1,
		tempTex.WorkRTViewAddress(), nullptr);
	pDevCon->RSSetViewports(1, &dstVP);


	// ②横ブラー処理
	//テクセル ぼかし具合
	SetBlurInfo(Math::Vector2::UnitX * (1.0f / src.GetWidth()), 10);
	Draw(src);

	// ③レンダーターゲットをdst(最終的に出力する画像)に切り替え
	pDevCon->OMSetRenderTargets(1,
		dst.WorkRTViewAddress(), nullptr);

	// ④縦ブラー処理
	SetBlurInfo(Math::Vector2::UnitY * (1.0f / tempTex.GetHeight()), 10);
	Draw(tempTex);


	// ⑤レンダーターゲットを元に戻す
	// 関数に入る前のRTVの復元
	pDevCon->OMSetRenderTargets(1, &pSaveRTV, pSaveDSV);
	pDevCon->RSSetViewports(1, &dstVP);

	// ⑥一時変数の開放
	KdSafeRelease(pSaveRTV);
	KdSafeRelease(pSaveDSV);
	tempTex.Release();


	EndBlur();
}


void KdPostProcessShader::GenerateLightBloomTexture(KdTexture& dst, const KdTexture& src)
{
	const int timeOfBlur = 8;	//8回ぼかす
	Math::Vector2 srcTexSize((float)src.GetWidth(), (float)src.GetHeight());	//ソースのテクスチャサイズ

	ID3D11DeviceContext* DevCon = D3D.WorkDevContext();

	// 何度か書くからクラス化するといい。レンダーターゲット切り替えるためだけのクラス
	// 現在のレンダーターゲットの保存
	ID3D11RenderTargetView* pSaveRTV = nullptr;
	ID3D11DepthStencilView* pSaveDSV = nullptr;
	DevCon->OMGetRenderTargets(1, &pSaveRTV, &pSaveDSV);

	UINT numViews = 1;
	D3D11_VIEWPORT saveVP;
	DevCon->RSGetViewports(&numViews, &saveVP);



	// ①光源テクスチャリストの作成
	KdTexture tempLightBloomRTTex[timeOfBlur];
	D3D11_VIEWPORT tempLightBloomVP[timeOfBlur];

	for (int i = 0; i < timeOfBlur; ++i)
	{
		tempLightBloomRTTex[i].CreateRenderTarget((int)srcTexSize.x, (int)srcTexSize.y);
		//															//深度の最小と最大
		tempLightBloomVP[i] = { 0.0f,0.0f,srcTexSize.x ,srcTexSize.y,0.0f,1.0f };

		srcTexSize *= 0.5f;
	}

	// ②光源テクスチャの作成
	GenerateBlurTexture(tempLightBloomRTTex[0], src);
	// 1番は作成済み
	for (int i = 1; i < timeOfBlur; ++i)
	{
		// 参考にする画像は一つ前の画像
		GenerateBlurTexture(tempLightBloomRTTex[i], tempLightBloomRTTex[i - 1]);
	}

	// 出力テクスチャにレンダーターゲットとビューポートをセット
	DevCon->OMSetRenderTargets(1, dst.WorkRTViewAddress(), nullptr);
	D3D11_VIEWPORT dstVP = { 0,0,(float)dst.GetWidth(),(float)dst.GetHeight(),0.0f,1.0f };
	DevCon->RSSetViewports(1, &dstVP);
	SetToDevice();
	// ③光源テクスチャの合成:これで重なって光って見える画像の出来上がり
	D3D.WorkShaderManager().ChangeBlendState(D3D.WorkShaderManager().m_bs_Add);
	for (int i = 0; i < timeOfBlur; ++i)
	{
		Draw(tempLightBloomRTTex[i]);
	}
	D3D.WorkShaderManager().UndoBlendState();

	// レンダーターゲットの復元
	DevCon->OMSetRenderTargets(1, &pSaveRTV, pSaveDSV);
	DevCon->RSSetViewports(1, &saveVP);

	KdSafeRelease(pSaveRTV);
	KdSafeRelease(pSaveDSV);
}