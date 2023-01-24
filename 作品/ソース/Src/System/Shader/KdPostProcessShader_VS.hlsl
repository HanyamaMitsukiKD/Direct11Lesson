#include "KdPostProcessShader.hlsli"

VSOutput main(
    float4 pos : POSITION,
    float2 uv : TEXCOORD0)
{
    VSOutput Out;
    
    Out.Pos = pos;
    Out.UV = uv;
    
    return Out;
}