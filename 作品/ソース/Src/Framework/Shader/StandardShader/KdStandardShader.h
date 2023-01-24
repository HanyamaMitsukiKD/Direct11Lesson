#pragma once

//============================================================
//
// 基本シェーダ
//
//============================================================
class KdStandardShader
{
	struct cbObject;
	struct cbMaterial;
	struct cbBone;
public:


	//================================================
	// 設定・取得
	//================================================

	// ワールド行列セット
	void SetWorldMatrix(const Math::Matrix& m)
	{
		m_cb1_Mesh.Work().mW = m;
	}

	// UVタイリング設定
	void SetUVTiling(const Math::Vector2& tiling)
	{
		m_cb0.Work().UVTiling = tiling;
	}
	// UVオフセット設定
	void SetUVOffset(const Math::Vector2& offset)
	{
		m_cb0.Work().UVOffset = offset;
	}

	// フォグ有効/無効
	void SetFogEnable(bool enable)
	{
		m_cb0.Work().FogEnable = enable;
	}

	// Object用定数バッファをそのまま返す
	KdConstantBuffer<cbObject>& ObjectCB()
	{
		return m_cb0;
	}

	// material用定数バッファをそのまま返す
	KdConstantBuffer<cbMaterial>& MaterialCB()
	{
		return m_cb2_Material;
	}

	// bone用定数バッファをそのまま返す
	KdConstantBuffer<cbBone>& BoneCB()
	{
		return m_cb3_Bones;
	}

	

	//================================================
	// 描画
	//================================================

	// このシェーダをデバイスへセット
	void SetToDevice();

	// メッシュ描画
	void DrawMesh(const KdMesh* mesh, const Math::Matrix& mWorld, const std::vector<KdMaterial>& materials);

	// モデル描画（静的モデル）
	//void DrawModel(const KdModelData& model, const Math::Matrix& mWorld);

	// モデル描画（アニメーション可モデル）
	void DrawModel(const KdModelWork& model, const Math::Matrix& mWorld);

	// 頂点群から成るポリゴン群描画
	void DrawPolygon(const KdPolygon& poly, const Math::Matrix& mWorld);

	//================================================
	// 輪郭描画
	//================================================
	// 輪郭描画としてデバイスセット
	// モデル描画（アニメーション可モデル）
	void DrawModelOutline(const KdModelWork& model, const Math::Matrix& mWorld, const float outLinePos);
	void SetToDevice_Outline();
	void DrawMesh_Outline(const KdMesh* mesh, const Math::Matrix& m_World);
	
	//================================================
	// 初期化・解放
	//================================================

	// 初期化
	bool Init();
	// 解放
	void Release();
	// 
	~KdStandardShader()
	{
		Release();
	}

private:

	void SetMaterial(const KdMaterial& material);

	// 3Dモデル用シェーダ
	ID3D11VertexShader*	m_VS = nullptr;				// 頂点シェーダー
	ID3D11VertexShader* m_VS_Skin = nullptr;		// 頂点シェーダー(スキンメッシュ

	ID3D11InputLayout*	m_inputLayout = nullptr;	// 頂点入力レイアウト
	ID3D11InputLayout* m_inputLayout_Skin = nullptr;// 頂点入力レイアウト(スキンメッシュ

	ID3D11PixelShader*	m_PS = nullptr;				// ピクセルシェーダー

	// 輪郭描画用シェーダー
	ID3D11VertexShader* m_OutlineVS = nullptr;				// 頂点シェーダー
	ID3D11PixelShader* m_OutlinePS = nullptr;				// ピクセルシェーダー
	ID3D11InputLayout* m_inputLayout_Outline = nullptr;		// 頂点入力レイアウト(アウトライン用)
	ID3D11InputLayout* m_inputLayout_SkinOutline = nullptr;		// 頂点入力レイアウト(アウトライン用)

	//トゥーン用のテクスチャ
	std::shared_ptr<KdTexture> m_texToon;			

	// 定数バッファ
	//  ※定数バッファは、パッキング規則というものを厳守しなければならない
	//  <パッキング規則> 参考：https://docs.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
	//  ・構造体のサイズは16バイトの倍数にする。
	//  ・各項目(変数)は、16バイト境界をまたぐような場合、次のベクトルに配置される。

	// 定数バッファ(オブジェクト単位更新)
	struct cbObject
	{
		// UV操作
		Math::Vector2		UVOffset = { 0,0 };
		Math::Vector2		UVTiling = { 1,1 };

		// フォグ有効
		int					FogEnable = 1;
		float			_blank[3] = { 0.0f, 0.0f,0.0f };
		
		
	};
	KdConstantBuffer<cbObject>	m_cb0;

	// 定数バッファ(メッシュ単位更新)
	struct cbMesh
	{
		Math::Matrix		mW;		// ワールド行列　行列は16バイトx4バイトの64バイトなのでピッタリ。

		float				m_OutLinePos = 0.03f;
		float				_blank[3] = { 0.0f, 0.0f,0.0f };
	};
	KdConstantBuffer<cbMesh>	m_cb1_Mesh;

	// 定数バッファ(マテリアル単位更新)
	struct cbMaterial
	{
		Math::Vector4	BaseColor;
		Math::Vector3	Emissive;
		float			Metallic = 0.0f;
		float			Roughness = 1.0f;

		
		float			_blank[3] = { 0.0f, 0.0f,0.0f };
	};
	KdConstantBuffer<cbMaterial>	m_cb2_Material;

public:
	static const int maxBoneBufferSize = 300;
private:
	// 定数バッファ(ボーン単位更新)
	struct cbBone {
		Math::Matrix	mBones[300];
	};
	KdConstantBuffer<cbBone>	m_cb3_Bones;

};


