#pragma once


//============================================================
//
// エフェクトシェーダ
//
//============================================================
class KdEffectShader
{
public:

	// １頂点の形式
	struct Vertex
	{
		Math::Vector3 Pos;
		Math::Vector2 UV;
		Math::Vector3 Normal;
		UINT Color;
		Math::Vector3 Tangent;
	};


	//================================================
	// 取得・設定
	//================================================

	// ワールド行列セット
	void SetWorldMatrix(const Math::Matrix& m)
	{
		m_cb1.Work().mW = m;
	}

	// UV Offsetセット
	void SetUVOffset(const Math::Vector2& offset)
	{
		m_cb0.Work().UVOffset = offset;
	}

	// UV Tilingセット
	void SetUVTiling(const Math::Vector2& tiling)
	{
		m_cb0.Work().UVTiling = tiling;
	}

	void SetDissolveValue(const float value)
	{
		m_cb0.Work().DissolveValue = value;
		m_cb0.Write(); //このやり方はゴミ
	}

	//================================================
	// 描画関係
	//================================================

	// このシェーダをデバイスへセット
	void SetToDevice();

	// 3D線描画
	void DrawLine(const Math::Vector3& p1, const Math::Vector3& p2, const Math::Color& color);

	// メッシュ描画
	void DrawMesh(const KdMesh* mesh, const Math::Matrix& mWorld, const std::vector<KdMaterial>& materials);

	// モデル描画（静的モデル）
	void DrawModel(const KdModelData& model, const Math::Matrix& mWorld);

	// モデル描画（アニメーション可モデル）
	void DrawModel(const KdModelWork& model, const Math::Matrix& mWorld);

	// 頂点群から成るポリゴン群描画
	void DrawPolygon(const KdPolygon& poly, const Math::Matrix& mWorld);

	// 輪郭描画としてデバイスセット
	
	//================================================
	// 
	//================================================

	// 初期化
	bool Init();

	// 解放
	void Release();

	// 
	~KdEffectShader()
	{
		Release();
	}

private:

	void SetMaterial(const KdMaterial& material);

	// 3Dモデル用シェーダ
	ID3D11VertexShader*	m_VS = nullptr;					// 頂点シェーダー
	ID3D11InputLayout*	m_inputLayout = nullptr;		// 頂点入力レイアウト

	ID3D11PixelShader*	m_PS = nullptr;				// ピクセルシェーダー

	

	// 定数バッファ
	//  ※定数バッファは、パッキング規則というものを厳守しなければならない
	//  <パッキング規則> 参考：https://docs.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-packing-rules
	//  ・構造体のサイズは16バイトの倍数にする。
	//  ・各項目(変数)は、16バイト境界をまたがないようにする。
	// 
	// 
	// 定数バッファ(オブジェクト単位更新)
	struct cbObject
	{
		Math::Vector2		UVOffset = { 0, 0 };
		Math::Vector2		UVTiling = { 1, 1 };

		float				DissolveValue = 0.5;	// 黒が多いと半分くらい消える
		float				_blank[3] = { 0,0,0 };
	};
	KdConstantBuffer<cbObject>	m_cb0;

	// 定数バッファ(メッシュ単位更新)
	struct cbMesh
	{
		Math::Matrix		mW;
	};
	KdConstantBuffer<cbMesh>	m_cb1;

	// 定数バッファ(マテリアル単位更新)
	struct cbMaterial
	{
		Math::Vector4	BaseColor;
		Math::Vector3	Emissive;
		float			_blank;
	};
	KdConstantBuffer<cbMaterial>	m_cb2_Material;
};


