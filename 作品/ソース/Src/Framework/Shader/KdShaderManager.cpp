#include "Framework/KdFramework.h"

#include "KdShaderManager.h"

void KdShaderManager::Init()
{
	//============================================
	// シェーダ
	//============================================
	m_standardShader.Init();
	m_effectShader.Init();
	m_spriteShader.Init();

	//============================================
	// 定数バッファ
	//============================================

	// カメラ
	m_cb7_Camera.Create();
	D3D.WorkDevContext()->VSSetConstantBuffers(7, 1, m_cb7_Camera.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(7, 1, m_cb7_Camera.GetAddress());

	// ライト
	m_cb8_Light.Create();
	m_cb8_Light.Work().DirLight_Dir.Normalize();
	m_cb8_Light.Write();
	D3D.WorkDevContext()->VSSetConstantBuffers(8, 1, m_cb8_Light.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(8, 1, m_cb8_Light.GetAddress());

	//============================================
	// パイプラインステート関係
	//============================================

	//深度ステンシルステート作成
	m_ds_ZEnable_ZWriteEnable = D3D.CreateDepthStencilState(true, true);
	m_ds_ZEnable_ZWriteDisable = D3D.CreateDepthStencilState(true, false);
	m_ds_ZDisable_ZWriteDisable = D3D.CreateDepthStencilState(false, false);

	D3D.WorkDevContext()->OMSetDepthStencilState(m_ds_ZEnable_ZWriteEnable, 0);

	// ラスタライザステート作成
	m_rs_CullBack = D3D.CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, true, false);
	m_rs_CullNone = D3D.CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, true, false);

	D3D.WorkDevContext()->RSSetState(m_rs_CullBack);

	// ブレンドステート作成
	m_bs_Alpha = D3D.CreateBlendState(KdBlendMode::Alpha);
	m_bs_Add = D3D.CreateBlendState(KdBlendMode::Add);

	D3D.WorkDevContext()->OMSetBlendState(m_bs_Alpha, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	// サンプラーステート作成
	m_ss_Anisotropic_Wrap = D3D.CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Wrap, false);
	m_ss_Anisotropic_Clamp = D3D.CreateSamplerState(KdSamplerFilterMode::Anisotropic, 4, KdSamplerAddressingMode::Clamp, false);
	m_ss_Linear_Clamp = D3D.CreateSamplerState(KdSamplerFilterMode::Linear, 0, KdSamplerAddressingMode::Clamp, false);
	m_ss_Linear_Clamp_Cmp = D3D.CreateSamplerState(KdSamplerFilterMode::Linear, 0, KdSamplerAddressingMode::Clamp, true);
	m_ss_Point_Wrap = D3D.CreateSamplerState(KdSamplerFilterMode::Point, 0, KdSamplerAddressingMode::Wrap, false);
	
	D3D.WorkDevContext()->PSSetSamplers(0, 1, &m_ss_Anisotropic_Wrap);
	
	// ディゾルブ処理
	m_disoolveTex = std::make_shared<KdTexture>();
	m_disoolveTex->Load("Asset/Texture/Shader/mask.png");
	// 10番にセットしたから 1つだけ送る WorkSRViewAddressは複数で渡せるから複数形
	D3D.WorkDevContext()->PSSetShaderResources(10, 1, m_disoolveTex->WorkSRViewAddress());
}

// 頂点シェーダのセット（既にセットされているシェーダーの場合はキャンセル）
bool KdShaderManager::SetVertexShader(ID3D11VertexShader* pSetVS)
{
	if (!pSetVS) { return false; }

	ID3D11VertexShader* pNowVS = nullptr;
	D3D.WorkDevContext()->VSGetShader(&pNowVS, nullptr, nullptr);

	bool needChange = pNowVS != pSetVS;

	if (pNowVS)
	{
		pNowVS->Release();
	}

	if (needChange)
	{
		D3D.WorkDevContext()->VSSetShader(pSetVS, nullptr, 0);
	}

	return needChange;
}

// ピクセルシェーダのセット（既にセットされているシェーダーの場合はキャンセル）
bool KdShaderManager::SetPixelShader(ID3D11PixelShader* pSetPS)
{
	if (!pSetPS) { return false; }

	ID3D11PixelShader* pNowPS = nullptr;
	D3D.WorkDevContext()->PSGetShader(&pNowPS, nullptr, nullptr);

	bool needChange = pNowPS != pSetPS;

	if (pNowPS)
	{
		pNowPS->Release();
	}

	if (needChange)
	{
		D3D.WorkDevContext()->PSSetShader(pSetPS, nullptr, 0);
	}

	return needChange;
}

bool KdShaderManager::SetInputLayout(ID3D11InputLayout* pSetLayout)
{
	if (!pSetLayout) { return false; }

	ID3D11InputLayout* pNowLayout = nullptr;

	D3D.WorkDevContext()->IAGetInputLayout(&pNowLayout);

	bool needChange = pNowLayout != pSetLayout;

	if (pNowLayout)
	{
		pNowLayout->Release();
	}

	if (needChange)
	{
		D3D.WorkDevContext()->IASetInputLayout(pSetLayout);
	}

	return needChange;
}

bool KdShaderManager::SetVSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetVSBuffer)
{
	if (!pSetVSBuffer) { return false; }

	ID3D11Buffer* pNowVSBuffer = nullptr;

	D3D.WorkDevContext()->VSGetConstantBuffers(startSlot, 1, &pNowVSBuffer);

	bool needChange = pNowVSBuffer != *pSetVSBuffer;

	if (pNowVSBuffer)
	{
		pNowVSBuffer->Release();
	}

	if (needChange)
	{
		D3D.WorkDevContext()->VSSetConstantBuffers(startSlot, 1, pSetVSBuffer);
	}

	return needChange;
}

bool KdShaderManager::SetPSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetPSBuffer)
{
	if (!pSetPSBuffer) { return false; }

	ID3D11Buffer* pNowPSBuffer = nullptr;

	D3D.WorkDevContext()->PSGetConstantBuffers(startSlot, 1, &pNowPSBuffer);

	bool needChange = pNowPSBuffer != *pSetPSBuffer;

	if (pNowPSBuffer)
	{
		pNowPSBuffer->Release();
	}

	if (needChange)
	{
		D3D.WorkDevContext()->PSSetConstantBuffers(startSlot, 1, pSetPSBuffer);
	}

	return needChange;
}

void KdShaderManager::ChangeDepthStencilState(ID3D11DepthStencilState* pSetDs)
{
	if (!pSetDs) { return; }

	ID3D11DepthStencilState* pNowDs = nullptr;
	D3D.WorkDevContext()->OMGetDepthStencilState(&pNowDs, 0);

	if (pNowDs != pSetDs)
	{
		m_ds_Undo = pNowDs;

		D3D.WorkDevContext()->OMSetDepthStencilState(pSetDs, 0);
	}

	if (pNowDs)
	{
		pNowDs->Release();
	}
}

void KdShaderManager::UndoDepthStencilState()
{
	if (!m_ds_Undo) { return; }

	D3D.WorkDevContext()->OMSetDepthStencilState(m_ds_Undo, 0);

	m_ds_Undo = nullptr;
}

void KdShaderManager::ChangeRasterizerState(ID3D11RasterizerState* pSetRs)
{
	if (!pSetRs) { return; }

	ID3D11RasterizerState* pNowRs = nullptr;
	D3D.WorkDevContext()->RSGetState(&pNowRs);

	if (pNowRs != pSetRs)
	{
		m_rs_Undo = pNowRs;

		D3D.WorkDevContext()->RSSetState(pSetRs);
	}

	if (pNowRs)
	{
		pNowRs->Release();
	}
}

void KdShaderManager::UndoRasterizerState()
{
	if (!m_rs_Undo) { return; }

	D3D.WorkDevContext()->RSSetState(m_rs_Undo);

	m_rs_Undo = nullptr;
}

void KdShaderManager::ChangeBlendState(ID3D11BlendState* pSetBs)
{
	if (!pSetBs) { return; }

	ID3D11BlendState* pNowBs = nullptr;
	D3D.WorkDevContext()->OMGetBlendState(&pNowBs, nullptr, nullptr);

	if (pNowBs != pSetBs)
	{
		m_bs_Undo = pNowBs;

		D3D.WorkDevContext()->OMSetBlendState(pSetBs, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);
	}

	if (pNowBs)
	{
		pNowBs->Release();
	}
}

void KdShaderManager::UndoBlendState()
{
	if (!m_bs_Undo) { return; }

	D3D.WorkDevContext()->OMSetBlendState(m_bs_Undo, Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	m_bs_Undo = nullptr;
}

void KdShaderManager::ChangeSamplerState(int slot, ID3D11SamplerState* pSetSs)
{
	if (!pSetSs) { return; }

	ID3D11SamplerState* pNowSs = nullptr;
	D3D.WorkDevContext()->PSGetSamplers(slot, 1, &pNowSs);

	if (pNowSs == pSetSs)
	{
		m_ss_Undo = pNowSs;

		D3D.WorkDevContext()->PSSetSamplers(slot, 1, &pSetSs);
	}

	if (pNowSs)
	{
		pNowSs->Release();
	}
}

void KdShaderManager::UndoSamplerState()
{
	if (!m_ss_Undo) { return; }

	D3D.WorkDevContext()->PSSetSamplers(0, 1, &m_ss_Undo);

	m_ss_Undo = nullptr;
}

void KdShaderManager::SetPointLight(const Math::Vector3& pos, const Math::Vector3& color, float radius)
{
	m_cb8_Light.Work().PointLightColor = color;
	m_cb8_Light.Work().PointLightRadius = radius;
	m_cb8_Light.Work().PointLightPos = pos;
}

void KdShaderManager::Release()
{
	m_standardShader.Release();
	m_effectShader.Release();
	m_spriteShader.Release();

	m_cb7_Camera.Release();
	m_cb8_Light.Release();

	//深度ステンシルステート開放
	KdSafeRelease(m_ds_ZEnable_ZWriteEnable);
	KdSafeRelease(m_ds_ZEnable_ZWriteDisable);
	KdSafeRelease(m_ds_ZDisable_ZWriteDisable);
	KdSafeRelease(m_ds_Undo);

	// ラスタライザステート解放
	KdSafeRelease(m_rs_CullBack);
	KdSafeRelease(m_rs_CullFront);
	KdSafeRelease(m_rs_CullNone);
	KdSafeRelease(m_rs_Undo);

	// ブレンドステート解放
	KdSafeRelease(m_bs_Alpha);
	KdSafeRelease(m_bs_Add);
	KdSafeRelease(m_bs_Undo);

	// サンプラーステート解放
	KdSafeRelease(m_ss_Anisotropic_Wrap);
	KdSafeRelease(m_ss_Anisotropic_Clamp);
	KdSafeRelease(m_ss_Linear_Clamp);
	KdSafeRelease(m_ss_Linear_Clamp_Cmp);
	KdSafeRelease(m_ss_Point_Wrap);
	KdSafeRelease(m_ss_Undo);
}