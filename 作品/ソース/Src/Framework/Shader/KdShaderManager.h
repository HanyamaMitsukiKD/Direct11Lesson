#pragma once

#include "StandardShader/KdStandardShader.h"
#include "EffectShader/KdEffectShader.h"
#include "SpriteShader/KdSpriteShader.h"
#include<System/Shader/KdPostProcessShader.h>

//==========================================================
//
// シェーダ関係をまとめたクラス
//
//==========================================================
class KdShaderManager
{
public:

	KdShaderManager() {}

	~KdShaderManager() { Release(); }

	//==============================================================
	//
	// 初期化・解放
	//
	//==============================================================

	// 初期化
	void Init();

	//==========================
	//
	// シェーダ
	//
	//==========================
	KdStandardShader	m_standardShader;	// 光の影響を受ける物質を描画するシェーダ
	KdEffectShader		m_effectShader;		// 光の影響を受けない物質を描画するシェーダ
	KdSpriteShader		m_spriteShader;		// 2Dテクスチャ描画シェーダ
	KdPostProcessShader m_postProcessShader;// ポストエフェクト用のシェーダー

	//==========================
	//
	// アクセサ
	//
	//==========================
	bool SetVertexShader(ID3D11VertexShader* pSetVS);
	bool SetPixelShader(ID3D11PixelShader* pSetPS);

	bool SetInputLayout(ID3D11InputLayout* pSetLayout);
	bool SetVSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetBuffer);
	bool SetPSConstantBuffer(int startSlot, ID3D11Buffer* const* pSetBuffer);

	void ChangeDepthStencilState(ID3D11DepthStencilState* pSetDs);
	void UndoDepthStencilState();

	void ChangeRasterizerState(ID3D11RasterizerState* pSetRs);
	void UndoRasterizerState();

	void ChangeBlendState(ID3D11BlendState* pSetBs);
	void UndoBlendState();

	void ChangeSamplerState(int slot, ID3D11SamplerState* pSetSs);
	void UndoSamplerState();

	void SetPointLight(const Math::Vector3& pos, const Math::Vector3& color, float radius);

	//==========================
	//
	// カメラ関係
	//
	//==========================

	// カメラ定数バッファ用構造体
	struct cbCamera
	{
		Math::Matrix		mView;		// ビュー行列
		Math::Matrix		mProj;		// 射影行列
		Math::Matrix		mProjInv;	// 射影行列：逆行列
		Math::Vector3		CamPos;		// カメラのワールド座標

		int					DistanceFogEnable = 0;			// 距離フォグ有効
		Math::Vector3		DistanceFogColor = { 1, 1, 1 };	// 距離フォグ色
		float				DistanceFogDensity = 0.001f;	// 距離フォグ減衰率

		bool				ToonFlg = false;
		float				GrayScaleValue = 0;// グレイスケール用
		float               _blank[2];
	};

	// カメラ定数バッファ
	KdConstantBuffer<cbCamera>	m_cb7_Camera;

	//==========================
	//
	// ライト関係
	//
	//==========================

	// ライト定数バッファ用構造体
	struct cbLight
	{
		static const int	MaxPointLightNum = 100;

		// 環境光
		//Math::Vector4		AmbientLight = {1.0f, 1.0f, 1.0f, 0.3f };
		Math::Vector4		AmbientLight = { 0.25f, 0.25f, 0.25f, 1.0f };	//ダークな雰囲気

		// 平行光
		Math::Vector3		DirLight_Dir = { -1.0f, -1.0f, 1.0f };	// 光の方向
		float				_blank1 = 0.0f;
		Math::Vector3		DirLight_Color = { 2.0f, 2.0f, 2.0f };	// 光の色
		float				_blank2 = 0.0f;

		Math::Matrix		DirLight_mVP;					// ビュー行列と正射影行列の合成行列


		Math::Vector3		PointLightColor;		// 色
		float				PointLightRadius;		// 半径
		Math::Vector3	    PointLightPos;			// 座標
		float				_blank3;
	};

	// ライト定数バッファ
	KdConstantBuffer<cbLight>	m_cb8_Light;

	//==========================
	//
	// パイプラインステート
	//
	//==========================

	//深度ステンシル
	ID3D11DepthStencilState* m_ds_ZEnable_ZWriteEnable = nullptr;		// 深度バッファの利用ON、 深度に書き込みON
	ID3D11DepthStencilState* m_ds_ZEnable_ZWriteDisable = nullptr;		// 深度バッファの利用ON、 深度に書き込みOFF
	ID3D11DepthStencilState* m_ds_ZDisable_ZWriteDisable = nullptr;		// 深度バッファの利用OFF、深度に書き込みOFF
	ID3D11DepthStencilState* m_ds_Undo = nullptr;

	// ラスタライズステート
	ID3D11RasterizerState* m_rs_CullBack = nullptr;			// 描画カリングモード：裏面の描画省略
	ID3D11RasterizerState* m_rs_CullFront = nullptr;		// 描画カリングモード：表面の描画省略
	ID3D11RasterizerState* m_rs_CullNone = nullptr;			// 描画カリングモード：描画省略なし
	ID3D11RasterizerState* m_rs_Undo = nullptr;

	// ブレンドステート
	ID3D11BlendState* m_bs_Alpha = nullptr;					// 透明度による色合成
	ID3D11BlendState* m_bs_Add = nullptr;					// 加算による色合成
	ID3D11BlendState* m_bs_Undo = nullptr;

	// サンプラーステート
	ID3D11SamplerState* m_ss_Anisotropic_Wrap = nullptr;	// 異方性補間：テクスチャ繰り返し
	ID3D11SamplerState* m_ss_Anisotropic_Clamp = nullptr;	// 異方性補間：テクスチャ範囲外は端のピクセルを延長
	ID3D11SamplerState* m_ss_Linear_Clamp = nullptr;		// 線形補間：　テクスチャ範囲外は端のピクセルを延長
	ID3D11SamplerState* m_ss_Linear_Clamp_Cmp = nullptr;	// 線形補間：　テクスチャ範囲外は端のピクセルを延長・比較機能付き
	ID3D11SamplerState* m_ss_Point_Wrap = nullptr;			// 補間なし：　テクスチャ繰り返し
	ID3D11SamplerState* m_ss_Undo = nullptr;

	std::shared_ptr<KdTexture> m_disoolveTex = nullptr;

private:

	// 解放
	void Release();
};
