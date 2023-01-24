//=====================================================
//
// �S�V�F�[�_����
//
//=====================================================

//------------------------------
// �萔�o�b�t�@(�J����)
//------------------------------
cbuffer cbCamera : register(b7)
{
	// �J�������
	row_major float4x4  g_mView;	// �r���[�ϊ��s��
	row_major float4x4  g_mProj;	// �ˉe�ϊ��s��
	row_major float4x4  g_mProjInv;	// �ˉe�ϊ��s��F�t�s��
	float3              g_CamPos;	// �J�������W

	// �t�H�O
	int     g_DistanceFogEnable;	// �t�H�O�L��/�����t���O
	float3  g_DistanceFogColor;		// �t�H�O�F
	float   g_DistanceFogDensity;	// �t�H�O������
	
    bool g_ToonFlg; // �g�D�[���V�F�[�_�[
    float g_GrayScaleValue; // �O���C�X�P�[��
};

//------------------------------
// �萔�o�b�t�@(���C�g)
//------------------------------
cbuffer cbLight : register(b8)
{
	// ����
	float4  g_AmbientLight;

	// ���s��
	float3  g_DL_Dir;    // ���̕���
	float3  g_DL_Color;  // ���̐F
	row_major float4x4  g_DL_mLightVP;  // �r���[�s�� x �ˉe�s��
	
	//--------------
	// �_��
	//--------------
    float3 g_PointLightColor; // �F
    float g_PointLightRadius; // ���a
    float3 g_PointLightPos; // ���W
};