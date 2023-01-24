#include "../inc_KdCommon.hlsli"
#include "inc_KdStandardShader.hlsli"

//================================
// ���_�V�F�[�_
//================================
VSOutput main(
	float4 pos : POSITION, // ���_���W
	float2 uv : TEXCOORD0, // �e�N�X�`��UV���W
	float3 normal : NORMAL, // �@���x�N�g��
	float4 color : COLOR, // ���_�F
	float3 tangent : TANGENT // �ڐ�
)
{
    VSOutput Out;

	// ���_���W���u�@�������v�ɏ������炷
    pos.xyz = pos.xyz + normal * g_OutLinePos;

	// 3D���_���W��2D�֕ϊ�
    Out.Pos = mul(pos, g_mWorld); // ���[���h�s��ŕϊ����Ă���A
    Out.Pos = mul(Out.Pos, g_mView); // �J�����̋t�s��(�r���[�s��)�ŕϊ����A
    Out.Pos = mul(Out.Pos, g_mProj); // �ˉe�s��ŕϊ�
   
	
	
    return Out;
}
