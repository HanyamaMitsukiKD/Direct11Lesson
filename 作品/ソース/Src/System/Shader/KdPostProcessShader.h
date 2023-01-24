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
		m_cb0_blur.Work().m_numPixel = numPixel;	// CPU��̒l�ύX
		m_cb0_blur.Work().m_uvOffset = uvOffset;

		m_cb0_blur.Write();						// GPU�ɏ�������
	}

private:
	ID3D11VertexShader* m_pVS = nullptr;			// ���_�V�F�[�_�[
	ID3D11InputLayout* m_pInputLayOut = nullptr;	// ���_�̃f�[�^
	ID3D11PixelShader* m_pPS = nullptr;				// �s�N�Z���V�F�[�_�[
	ID3D11PixelShader* m_pPS_Blur = nullptr;		// �s�N�Z���V�F�[�_�[(�u���[)

	struct cb_blur
	{
		Math::Vector2 m_uvOffset;					// �e�N�Z���T�C�Y * ���炷����
		int m_numPixel = 1;							// ���T�C�h�ɉ��s�N�Z�����ڂ�����
		float _blank = 0;
	};

	KdConstantBuffer<cb_blur> m_cb0_blur;			// �R���X�^���g�o�b�t�@����F�������ƊJ�����Z�b�g��
};