#include "Framework/KdFramework.h"

#include "KdEffectShader.h"

// １頂点の形式
struct Vertex
{
	Math::Vector3 Pos;
	Math::Vector2 UV;
	Math::Vector4 Color;
};

void KdEffectShader::DrawLine(const Math::Vector3 & p1, const Math::Vector3 & p2, const Math::Color & color)
{
	// 定数バッファ書き込み
	m_cb0.Write();

	// 頂点レイアウトをセット
	D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);

	// 
	Vertex vertex[2] = {
		{p1, {0,0}, color},
		{p2, {1,0}, color},
	};

	// 頂点を描画
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, 2, &vertex[0], sizeof(Vertex));
}

void KdEffectShader::DrawMesh(const KdMesh* mesh, const Math::Matrix& mWorld, const std::vector<KdMaterial>& materials)
{
	if (mesh == nullptr) { return; }

	// 定数バッファ書き込み
	SetWorldMatrix(mWorld);

	m_cb1.Write();

	// メッシュ情報をセット
	mesh->SetToDevice();

	// 頂点レイアウトをセット
	D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);

	// 全サブセット
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// 面が１枚も無い場合はスキップ
		if (mesh->GetSubsets()[subi].FaceCount == 0)continue;

		// マテリアルセット
		const KdMaterial& material = materials[ mesh->GetSubsets()[subi].MaterialNo ];

		SetMaterial(material);

		//-----------------------
		// サブセット描画
		//-----------------------
		mesh->DrawSubset(subi);
	}
}


void KdEffectShader::DrawModel(const KdModelData& rModel, const Math::Matrix& mWorld)
{
	auto& dataNodes = rModel.GetOriginalNodes();

	m_cb0.Write();

	// 全メッシュノードを描画
	for (auto& nodeIdx : rModel.GetMeshNodeIndices())
	{
		auto& rDataNode = dataNodes[nodeIdx];

		// 描画
		DrawMesh(rDataNode.m_spMesh.get(), rDataNode.m_worldTransform * mWorld, rModel.GetMaterials());
	}
}

void KdEffectShader::DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld)
{
	// 有効じゃないときはスキップ
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = rModel.GetData();

	// モデルがないときはスキップ
	if (data == nullptr) { return; }

	m_cb0.Write();

	auto& workNodes = rModel.GetNodes();
	auto& dataNodes = data->GetOriginalNodes();

	// 全メッシュノードを描画
	for (auto& nodeIdx : rModel.GetData()->GetMeshNodeIndices())
	{
		auto& rDataNode = dataNodes[nodeIdx];
		auto& rWorkNode = workNodes[nodeIdx];

		// 描画
		DrawMesh(rDataNode.m_spMesh.get(), rWorkNode.m_worldTransform * mWorld, data->GetMaterials());
	}
}

void KdEffectShader::DrawPolygon(const KdPolygon& polygon, const Math::Matrix& mWorld)
{
	if (!polygon.IsEnable()) { return; }

	m_cb0.Write();

	std::vector<KdPolygon::Vertex> vertices;

	// ポリゴン描画用の頂点生成
	polygon.GenerateVertices(vertices);

	// 頂点数が3より少なければポリゴンが形成できないので描画不能
	if (vertices.size() < 3) { return; }

	SetWorldMatrix(mWorld);

	m_cb1.Write();

	if (polygon.GetMaterial())
	{
		SetMaterial(*polygon.GetMaterial());
	}
	else
	{
		SetMaterial(KdMaterial());
	}

	D3D.WorkShaderManager().ChangeSamplerState(0, D3D.WorkShaderManager().m_ss_Anisotropic_Clamp);

	// 指定した頂点配列を描画する関数
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertices.size(), &vertices[0], sizeof(KdPolygon::Vertex));

	D3D.WorkShaderManager().UndoSamplerState();
}

bool KdEffectShader::Init()
{
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
		#include "KdEffectShader_VS.inc"

		// 頂点シェーダー作成
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		{
			// １頂点の詳細な情報
			std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};

			// 頂点入力レイアウト作成
			if (FAILED(D3D.WorkDev()->CreateInputLayout(
				&layout[0],			// 入力エレメント先頭アドレス
				layout.size(),		// 入力エレメント数
				&compiledBuffer[0],				// 頂点バッファのバイナリデータ
				sizeof(compiledBuffer),			// 上記のバッファサイズ
				&m_inputLayout))					// 
				) {
				assert(0 && "CreateInputLayout失敗");
				Release();
				return false;
			}		
		}
	}

	


	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
		#include "KdEffectShader_PS.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}


	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0.Create();
	m_cb1.Create();
	m_cb2_Material.Create();

	return true;
}

void KdEffectShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_inputLayout);

	

	m_cb0.Release();
	m_cb1.Release();
	m_cb2_Material.Release();


}

void KdEffectShader::SetToDevice()
{
	// 頂点シェーダをセット
	D3D.WorkDevContext()->VSSetShader(m_VS, 0, 0);
	// 頂点レイアウトをセット
	D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);

	// ピクセルシェーダをセット
	D3D.WorkDevContext()->PSSetShader(m_PS, 0, 0);

	// 定数バッファをセット
	D3D.WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.WorkDevContext()->VSSetConstantBuffers(1, 1, m_cb1.GetAddress());
	
	D3D.WorkDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(2, 1, m_cb2_Material.GetAddress());
}

void KdEffectShader::SetMaterial(const KdMaterial& material)
{
	//-----------------------
	// マテリアル情報を定数バッファへ書き込む
	//-----------------------
	m_cb2_Material.Work().BaseColor = material.BaseColor;
	m_cb2_Material.Work().Emissive = material.Emissive;
	m_cb2_Material.Write();

	//-----------------------
	// テクスチャセット
	//-----------------------
	ID3D11ShaderResourceView* srvs[2] = {};

	// BaseColor
	srvs[0] = material.BaseColorTex->WorkSRView();
	// Emissive
	srvs[1] = material.EmissiveTex->WorkSRView();

	// セット
	D3D.WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
}