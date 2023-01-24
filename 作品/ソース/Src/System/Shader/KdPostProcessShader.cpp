#include "KdPostProcessShader.h"

void KdPostProcessShader::Init()
{
	//-------------------------------------
	// ���_�V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
#include "KdPostProcessShader_VS.inc"

// ���_�V�F�[�_�[�쐬
		if (FAILED(D3D.WorkDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_pVS))) {
			assert(0 && "���_�V�F�[�_�[�쐬���s");
			Release();
			return;
		}

		// �P���_�̏ڍׂȏ��
		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//���W
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },	//UV���W
		};														 //12�o�C�g����n�܂�

		// ���_�C���v�b�g���C�A�E�g�쐬
		if (FAILED(D3D.WorkDev()->CreateInputLayout(
			&layout[0],
			layout.size(),
			compiledBuffer,
			sizeof(compiledBuffer),
			&m_pInputLayOut))
			) {
			assert(0 && "CreateInputLayout���s");
			Release();
			return;
		}
	}

	//-------------------------------------
	// �s�N�Z���V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
#include "KdPostProcessShader_PS.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_pPS))) {
			assert(0 && "�s�N�Z���V�F�[�_�[�쐬���s");
			Release();
			return;
		}
	}

	//-------------------------------------
	// �s�N�Z���V�F�[�_
	//-------------------------------------
	{
		// �R���p�C���ς݂̃V�F�[�_�[�w�b�_�[�t�@�C�����C���N���[�h
#include "KdPostProcessShader_PS_Blur.inc"

		if (FAILED(D3D.WorkDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_pPS_Blur))) {
			assert(0 && "�s�N�Z���V�F�[�_�[�쐬���s");
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
	//�@�؂�ւ���State�ŊǗ������ق���������������Ȃ�
	//  �X�^���_�[�h�̃X�L�����b�V�����Q�l�ɂ���
	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();

	pDevCon->VSSetShader(m_pVS, 0, 0);			//
	pDevCon->IASetInputLayout(m_pInputLayOut);	//���̂Q�̓Z�b�g
	pDevCon->PSSetShader(m_pPS, 0, 0);

	//pDevCon->PSSetConstantBuffers(0, 1, nullptr);
}

void KdPostProcessShader::BeginBlur()
{
	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();

	pDevCon->VSSetShader(m_pVS, 0, 0);
	pDevCon->IASetInputLayout(m_pInputLayOut);
	pDevCon->PSSetShader(m_pPS_Blur, 0, 0);

	// �s�N�Z���V�F�[�_�[�Ŏg��  0�ԂɃZ�b�g
	pDevCon->PSSetConstantBuffers(0, 1, m_cb0_blur.GetAddress());

	m_cb0_blur.Write();						// GPU�ɏ�������

	// �T���v���[�X�e�[�g���N�����v���[�h�ɐ؂�ւ���
	// �ꖇ�G
	D3D.WorkShaderManager().ChangeSamplerState(0, D3D.WorkShaderManager().m_ss_Linear_Clamp);
}

void KdPostProcessShader::EndBlur()
{
	// �T���v���[�X�e�[�g�����ɖ߂�
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

	// ���_�̐ݒ�F��Ƃ̏ꍇ
	screenVertex[0] = { {-1,-1,0},{0,1} };
	screenVertex[1] = { {-1,1,0},{0,0} };
	screenVertex[2] = { {1,-1,0},{1,1} };
	screenVertex[3] = { {1,1,0},{1,0} };

	D3D.WorkDevContext()->PSSetShaderResources(0, 1, tex.WorkSRViewAddress());

	// �|���S����\��
	D3D.DrawVertices(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		4, &screenVertex[0], sizeof(Vertex));
}

void KdPostProcessShader::GenerateBlurTexture(KdTexture& dst, const KdTexture& src)
{
	// �R�X�g�x�O���ŏ����Ă���
	BeginBlur();

	ID3D11DeviceContext* pDevCon = D3D.WorkDevContext();

	if (!pDevCon) { return; }

	// �@�����_�[�^�[�Q�b�g�̐؂�ւ�
	//�@ �����RTV/DSV/VP�̕ۑ�
	ID3D11RenderTargetView* pSaveRTV = nullptr;
	ID3D11DepthStencilView* pSaveDSV = nullptr;
	pDevCon->OMGetRenderTargets(1, &pSaveRTV, &pSaveDSV);

	// �r���[�|�[�g�i�T�C�Y�A�A�X�y�N�g��j
	UINT numViews = 1;
	D3D11_VIEWPORT saveVP;
	pDevCon->RSGetViewports(&numViews, &saveVP);

	// �؂�ւ��钆��RTV/VP�̍쐬
	//�udst�v�́udestination�i�f�X�e�B�l�[�V�����j�v�̗���ł��B �Ӗ��́u�ړI�n�A�s��A���Đ�v�ȂǂɂȂ�܂�
	KdTexture tempTex;
	tempTex.CreateRenderTarget(dst.GetWidth(), dst.GetHeight());

	D3D11_VIEWPORT dstVP = { 0,0,
		(float)dst.GetWidth(),
		(float)dst.GetHeight(),
		0.0f,1.0f };

	// ����RTV/VP�ւ̐؂�ւ�
	pDevCon->OMSetRenderTargets(1,
		tempTex.WorkRTViewAddress(), nullptr);
	pDevCon->RSSetViewports(1, &dstVP);


	// �A���u���[����
	//�e�N�Z�� �ڂ����
	SetBlurInfo(Math::Vector2::UnitX * (1.0f / src.GetWidth()), 10);
	Draw(src);

	// �B�����_�[�^�[�Q�b�g��dst(�ŏI�I�ɏo�͂���摜)�ɐ؂�ւ�
	pDevCon->OMSetRenderTargets(1,
		dst.WorkRTViewAddress(), nullptr);

	// �C�c�u���[����
	SetBlurInfo(Math::Vector2::UnitY * (1.0f / tempTex.GetHeight()), 10);
	Draw(tempTex);


	// �D�����_�[�^�[�Q�b�g�����ɖ߂�
	// �֐��ɓ���O��RTV�̕���
	pDevCon->OMSetRenderTargets(1, &pSaveRTV, pSaveDSV);
	pDevCon->RSSetViewports(1, &dstVP);

	// �E�ꎞ�ϐ��̊J��
	KdSafeRelease(pSaveRTV);
	KdSafeRelease(pSaveDSV);
	tempTex.Release();


	EndBlur();
}


void KdPostProcessShader::GenerateLightBloomTexture(KdTexture& dst, const KdTexture& src)
{
	const int timeOfBlur = 8;	//8��ڂ���
	Math::Vector2 srcTexSize((float)src.GetWidth(), (float)src.GetHeight());	//�\�[�X�̃e�N�X�`���T�C�Y

	ID3D11DeviceContext* DevCon = D3D.WorkDevContext();

	// ���x����������N���X������Ƃ����B�����_�[�^�[�Q�b�g�؂�ւ��邽�߂����̃N���X
	// ���݂̃����_�[�^�[�Q�b�g�̕ۑ�
	ID3D11RenderTargetView* pSaveRTV = nullptr;
	ID3D11DepthStencilView* pSaveDSV = nullptr;
	DevCon->OMGetRenderTargets(1, &pSaveRTV, &pSaveDSV);

	UINT numViews = 1;
	D3D11_VIEWPORT saveVP;
	DevCon->RSGetViewports(&numViews, &saveVP);



	// �@�����e�N�X�`�����X�g�̍쐬
	KdTexture tempLightBloomRTTex[timeOfBlur];
	D3D11_VIEWPORT tempLightBloomVP[timeOfBlur];

	for (int i = 0; i < timeOfBlur; ++i)
	{
		tempLightBloomRTTex[i].CreateRenderTarget((int)srcTexSize.x, (int)srcTexSize.y);
		//															//�[�x�̍ŏ��ƍő�
		tempLightBloomVP[i] = { 0.0f,0.0f,srcTexSize.x ,srcTexSize.y,0.0f,1.0f };

		srcTexSize *= 0.5f;
	}

	// �A�����e�N�X�`���̍쐬
	GenerateBlurTexture(tempLightBloomRTTex[0], src);
	// 1�Ԃ͍쐬�ς�
	for (int i = 1; i < timeOfBlur; ++i)
	{
		// �Q�l�ɂ���摜�͈�O�̉摜
		GenerateBlurTexture(tempLightBloomRTTex[i], tempLightBloomRTTex[i - 1]);
	}

	// �o�̓e�N�X�`���Ƀ����_�[�^�[�Q�b�g�ƃr���[�|�[�g���Z�b�g
	DevCon->OMSetRenderTargets(1, dst.WorkRTViewAddress(), nullptr);
	D3D11_VIEWPORT dstVP = { 0,0,(float)dst.GetWidth(),(float)dst.GetHeight(),0.0f,1.0f };
	DevCon->RSSetViewports(1, &dstVP);
	SetToDevice();
	// �B�����e�N�X�`���̍���:����ŏd�Ȃ��Č����Č�����摜�̏o���オ��
	D3D.WorkShaderManager().ChangeBlendState(D3D.WorkShaderManager().m_bs_Add);
	for (int i = 0; i < timeOfBlur; ++i)
	{
		Draw(tempLightBloomRTTex[i]);
	}
	D3D.WorkShaderManager().UndoBlendState();

	// �����_�[�^�[�Q�b�g�̕���
	DevCon->OMSetRenderTargets(1, &pSaveRTV, pSaveDSV);
	DevCon->RSSetViewports(1, &saveVP);

	KdSafeRelease(pSaveRTV);
	KdSafeRelease(pSaveDSV);
}