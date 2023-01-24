#include "Framework/KdFramework.h"

#include "KdStandardShader.h"


void KdStandardShader::SetToDevice()
{
	D3D.WorkDevContext()->PSSetShader(m_PS, 0, 0);

	// 定数バッファをセット
	D3D.WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());

	D3D.WorkDevContext()->VSSetConstantBuffers(1, 1, m_cb1_Mesh.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(2, 1, m_cb2_Material.GetAddress());
	D3D.WorkDevContext()->VSSetConstantBuffers(3, 1, m_cb3_Bones.GetAddress());
}

void KdStandardShader::DrawMesh(const KdMesh* mesh, const Math::Matrix& mWorld, const std::vector<KdMaterial>& materials)
{
	if (mesh == nullptr) { return; }

	// 行列セット
	SetWorldMatrix(mWorld);

	m_cb1_Mesh.Write();

	// StaticMeshとSkinMeshによって頂点シェーダーと入力レイアウトを切り替える
	if (mesh->IsSkinMesh())
	{
		// SkinMesh用の頂点シェーダーをセット
		if (D3D.WorkShaderManager().SetVertexShader(m_VS_Skin))
		{
			// 頂点レイアウトをセット
			D3D.WorkDevContext()->IASetInputLayout(m_inputLayout_Skin);
		}
	}
	else
	{
		// StaticMesh用の頂点シェーダーをセット
		if (D3D.WorkShaderManager().SetVertexShader(m_VS))
		{
			// 頂点レイアウトをセット
			D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);
		}
	}

	// メッシュ情報をセット
	mesh->SetToDevice();

	// 全サブセット
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// 面が１枚も無い場合はスキップ
		if (mesh->GetSubsets()[subi].FaceCount == 0) { continue; }

		// マテリアルセット
		const KdMaterial& material = materials[mesh->GetSubsets()[subi].MaterialNo];

		SetMaterial(material);

		m_cb2_Material.Write();

		//Normal Map
		D3D.WorkDevContext()->PSSetShaderResources(3, 1, material.NormalTex->WorkSRViewAddress());


		//-----------------------
		// サブセット描画
		//-----------------------
		mesh->DrawSubset(subi);
	}
}

void KdStandardShader::DrawModel(const KdModelWork& rModel, const Math::Matrix& mWorld)
{
	// 有効じゃないときはスキップ
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = rModel.GetData();

	// モデルがないときはスキップ
	if (data == nullptr) { return; }

	m_cb0.Write();

	auto& workNodes = rModel.GetNodes();
	auto& dataNodes = data->GetOriginalNodes();

	// ピクセルシェーダーをセット
	D3D.WorkDevContext()->PSSetShader(m_PS, 0, 0);

	// スキンメッシュモデルの場合：ボーン情報を書き込み
	if (data->IsSkinMesh())
	{
		// ノード内からボーン情報を取得
		for (auto&& nodeIdx : data->GetBoneNodeIndices())
		{
			if (nodeIdx >= KdStandardShader::maxBoneBufferSize) { assert(0 && "転送できるボーンの上限数を超えました"); return; }

			auto& dataNode = dataNodes[nodeIdx];
			auto& workNode = workNodes[nodeIdx];

			// ボーン情報からGPUに渡す行列の計算
			BoneCB().Work().mBones[dataNode.m_boneIndex] = dataNode.m_boneInverseWorldMatrix * workNode.m_worldTransform;

			BoneCB().Write();
		}
	}

	// 全メッシュノードを描画
	for (auto& nodeIdx : data->GetMeshNodeIndices())
	{
		auto& rWorkNode = workNodes[nodeIdx];

		const std::shared_ptr<KdMesh>& spMesh = rModel.GetMesh(nodeIdx);

		// 描画
		DrawMesh(spMesh.get(), rWorkNode.m_worldTransform * mWorld, data->GetMaterials());
	}
}

void KdStandardShader::DrawPolygon(const KdPolygon& poly, const Math::Matrix& mWorld)
{
	// 有効じゃないときはスキップ
	if (!poly.IsEnable()) { return; }

	m_cb0.Write();

	std::vector<KdPolygon::Vertex> vertices;

	// ポリゴン描画用の頂点生成
	poly.GenerateVertices(vertices);

	// 頂点数が3より少なければポリゴンが形成できないので描画不能
	if (vertices.size() < 3) { return; }

	// StaticMesh用の頂点シェーダーをセット
	if (D3D.WorkShaderManager().SetVertexShader(m_VS))
	{
		// 頂点レイアウトをセット
		D3D.WorkDevContext()->IASetInputLayout(m_inputLayout);
	}

	SetWorldMatrix(mWorld);

	m_cb1_Mesh.Write();

	if (poly.GetMaterial())
	{
		SetMaterial(*poly.GetMaterial());
	}
	else
	{
		SetMaterial(KdMaterial());
	}

	m_cb2_Material.Write();

	D3D.WorkShaderManager().ChangeSamplerState(0, D3D.WorkShaderManager().m_ss_Anisotropic_Clamp);

	// 指定した頂点配列を描画する関数
	D3D.DrawVertices(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP, vertices.size(), &vertices[0], sizeof(KdPolygon::Vertex));

	D3D.WorkShaderManager().UndoSamplerState();
}

void KdStandardShader::DrawModelOutline(const KdModelWork& rModel, const Math::Matrix& mWorld,const float outLinePos)
{
	// 有効じゃないときはスキップ
	if (!rModel.IsEnable()) { return; }

	const std::shared_ptr<KdModelData>& data = rModel.GetData();

	// モデルがないときはスキップ
	if (data == nullptr) { return; }

	m_cb1_Mesh.Work().m_OutLinePos = outLinePos;
	m_cb1_Mesh.Write();

	
	auto& workNodes = rModel.GetNodes();
	auto& dataNodes = data->GetOriginalNodes();

	// スキンメッシュモデルの場合：ボーン情報を書き込み
	if (data->IsSkinMesh())
	{
		// ノード内からボーン情報を取得
		for (auto&& nodeIdx : data->GetBoneNodeIndices())
		{
			if (nodeIdx >= KdStandardShader::maxBoneBufferSize) { assert(0 && "転送できるボーンの上限数を超えました"); return; }

			auto& dataNode = dataNodes[nodeIdx];
			auto& workNode = workNodes[nodeIdx];

			// ボーン情報からGPUに渡す行列の計算
			BoneCB().Work().mBones[dataNode.m_boneIndex] = dataNode.m_boneInverseWorldMatrix * workNode.m_worldTransform;

			BoneCB().Write();
		}
	}

	// 全メッシュノードを描画
	for (auto& nodeIdx : data->GetMeshNodeIndices())
	{
		auto& rWorkNode = workNodes[nodeIdx];

		const std::shared_ptr<KdMesh>& spMesh = rModel.GetMesh(nodeIdx);

		// 描画
		DrawMesh(spMesh.get(), rWorkNode.m_worldTransform * mWorld, data->GetMaterials());
	}

	// 輪郭描画
	// 表面をカリング(非表示)にするラスタライザステートを設置
	D3D.WorkDevContext()->RSSetState(D3D.WorkShaderManager().m_rs_CullFront);

	// 輪郭シェーダの情報を設定
	SetToDevice_Outline();

	// 全メッシュノードを描画
	for (auto& nodeIdx : data->GetMeshNodeIndices())
	{
		auto& rWorkNode = workNodes[nodeIdx];

		const std::shared_ptr<KdMesh>& spMesh = rModel.GetMesh(nodeIdx);
		if (spMesh == nullptr) { continue; }

		// 描画
		DrawMesh_Outline(spMesh.get(), rWorkNode.m_worldTransform * mWorld);
	}

	// 裏面をカリング(非表示にするラスタライザステートに戻す)
	D3D.WorkDevContext()->RSSetState(D3D.WorkShaderManager().m_rs_CullBack);
}

void KdStandardShader::SetToDevice_Outline()
{
	// 頂点シェーダーをセット
	D3D.WorkDevContext()->VSSetShader(m_OutlineVS, 0, 0);

	// 頂点レイアウトをセット:、DirectX10からはシェーダの入力が非常に柔軟になったため、
	// その整合性を合わせる「頂点レイアウト（入力レイアウト）」という仲介インターフェイスに双方の情報を集めるようになりました。
	// その頂点レイアウトを描画デバイスに登録すれば、描画時に頂点バッファとシェーダが結びつく事になります。
	D3D.WorkDevContext()->IASetInputLayout(m_inputLayout_Outline);

	// ピクセルシェーダーをセット
	D3D.WorkDevContext()->PSSetShader(m_OutlinePS, 0, 0);

	// 定数バッファをセット:DX11ではシェーダ実行時に自由に使うことができる値として、定数バッファ(英訳:ConstantBuffer)というものが用意されています。
	D3D.WorkDevContext()->VSSetConstantBuffers(0, 1, m_cb0.GetAddress());
	D3D.WorkDevContext()->PSSetConstantBuffers(0, 1, m_cb0.GetAddress());
}

void KdStandardShader::DrawMesh_Outline(const KdMesh* mesh, const Math::Matrix& m_World)
{
	if (mesh == nullptr) { return; }

	// 行列をセット
	SetWorldMatrix(m_World);

	m_cb1_Mesh.Write();

	// StaticMeshとSkinMeshによって頂点シェーダーと入力レイアウトを切り替える
	if (mesh->IsSkinMesh())
	{
		// SkinMesh用の頂点シェーダーをセット
		if (D3D.WorkShaderManager().SetVertexShader(m_VS_Skin))
		{
			// 頂点レイアウトをセット
			D3D.WorkDevContext()->IASetInputLayout(m_inputLayout_Skin);
		}
	}

	mesh->SetToDevice();

	// 全サブセット
	for (UINT subi = 0; subi < mesh->GetSubsets().size(); subi++)
	{
		// 面が１枚もない場合はスキップ
		if (mesh->GetSubsets()[subi].FaceCount == 0) { continue; }

		mesh->DrawSubset(subi);
	}
}

bool KdStandardShader::Init()
{
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_VS.inc"

// 頂点シェーダー作成
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

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
			&layout[0],				// 入力エレメント先頭アドレス
			layout.size(),			// 入力エレメント数
			&compiledBuffer[0],		// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),	// 上記のバッファサイズ
			&m_inputLayout))
			) {
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}
	//-------------------------------------
	// 頂点シェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_VS_Skin.inc"

// 頂点シェーダー作成
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS_Skin))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		// １頂点の詳細な情報
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 20,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,		0, 32,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 36,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKININDEX",	0, DXGI_FORMAT_R16G16B16A16_UINT,	0, 48,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SKINWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 56,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// 頂点入力レイアウト作成
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],				// 入力エレメント先頭アドレス
			layout.size(),			// 入力エレメント数
			&compiledBuffer[0],		// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),	// 上記のバッファサイズ
			&m_inputLayout_Skin))
			) {
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}


	//-------------------------------------
	// 頂点シェーダ(輪郭用)
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_Outline_VS.inc"	//先にビルドを通すとincが生まれる

// 頂点シェーダー作成
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_OutlineVS))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

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
			&layout[0],				// 入力エレメント先頭アドレス
			layout.size(),			// 入力エレメント数
			&compiledBuffer[0],		// 頂点バッファのバイナリデータ
			sizeof(compiledBuffer),	// 上記のバッファサイズ
			&m_inputLayout_Outline))
			) {
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
	}


	//-------------------------------------
	// ピクセルシェーダ
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_PS.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_PS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// ピクセルシェーダ(輪郭用)
	//-------------------------------------
	{
		// コンパイル済みのシェーダーヘッダーファイルをインクルード
#include "KdStandardShader_Outline_PS.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_OutlinePS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}
	}

	//-------------------------------------
	// 定数バッファ作成
	//-------------------------------------
	m_cb0.Create();
	m_cb1_Mesh.Create();
	m_cb2_Material.Create();
	m_cb3_Bones.Create();


	//-------------------------------------
	// テクスチャ読込み
	//-------------------------------------
	m_texToon = std::make_shared<KdTexture>();
	m_texToon->Load("Asset/Texture/Shader/toon3.png");
	D3D.WorkDevContext()->
		PSSetShaderResources
		(4, 1, m_texToon->WorkSRViewAddress());



	return true;
}

void KdStandardShader::Release()
{
	KdSafeRelease(m_VS);
	KdSafeRelease(m_VS_Skin);
	KdSafeRelease(m_inputLayout);
	KdSafeRelease(m_inputLayout_Outline);
	KdSafeRelease(m_inputLayout_Skin);
	KdSafeRelease(m_inputLayout_SkinOutline);
	KdSafeRelease(m_PS);
	KdSafeRelease(m_OutlinePS);
	KdSafeRelease(m_OutlineVS);

	m_cb0.Release();
	m_cb1_Mesh.Release();
	m_cb2_Material.Release();
	m_cb3_Bones.Release();
}

void KdStandardShader::SetMaterial(const KdMaterial& material)
{
	//-----------------------
	// マテリアル情報を定数バッファへ書き込む
	//-----------------------
	m_cb2_Material.Work().BaseColor = material.BaseColor;
	m_cb2_Material.Work().Emissive = material.Emissive;
	m_cb2_Material.Work().Metallic = material.Metallic;
	m_cb2_Material.Work().Roughness = material.Roughness;

	//-----------------------
	// テクスチャセット
	//-----------------------
	ID3D11ShaderResourceView* srvs[3] = {};

	// BaseColor
	srvs[0] = material.BaseColorTex->WorkSRView();
	// Emissive
	srvs[1] = material.EmissiveTex->WorkSRView();
	// Metallic Roughness
	srvs[2] = material.MetallicRoughnessTex->WorkSRView();

	// セット
	D3D.WorkDevContext()->PSSetShaderResources(0, _countof(srvs), srvs);
}