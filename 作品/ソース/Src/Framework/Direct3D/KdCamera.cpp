#include "KdCamera.h"

// 射影行列の設定
void KdCamera::SetProjectionMatrix(float fov, float maxRange, float minRange, float aspectRatio)
{
	m_mProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), aspectRatio, minRange, maxRange);

	SetProjectionMatrix(m_mProj);
}

void KdCamera::SetProjectionMatrix(const DirectX::SimpleMath::Matrix& rProj)
{
	m_mProj = rProj;
}

// カメラ行列のセット・カメラ行列からビュー行列を生成
void KdCamera::SetCameraMatrix(const DirectX::SimpleMath::Matrix& mCam)
{
	// カメラ行列セット
	m_mCam = mCam;

	// カメラ行列からビュー行列を算出
	m_mView = mCam.Invert();
}

// ビュー行列のセット・ビュー行列からカメラ行列を生成
void KdCamera::SetViewMatrix(const DirectX::SimpleMath::Matrix& mView)
{
	// ビュー行列セット
	m_mView = mView;

	// ビュー行列からカメラ行列を算出
	m_mCam = mView.Invert();
}

// カメラ情報(ビュー・射影行列など)をシェーダへセット
void KdCamera::SetToShader() const
{
	auto& cbCamera = D3D.WorkShaderManager().m_cb7_Camera.Work();

	// カメラ座標をセット
	cbCamera.CamPos = m_mCam.Translation();

	// ビュー行列をセット
	cbCamera.mView = m_mView;

	// 射影行列をセット
	cbCamera.mProj = m_mProj;

	cbCamera.mProjInv = m_mProj.Invert();

	// カメラ情報(ビュー行列、射影行列)を、シェーダの定数バッファへ書き込む
	D3D.WorkShaderManager().m_cb7_Camera.Write();
}