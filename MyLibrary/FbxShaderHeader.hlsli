

#define NUM_CONTROL_POINTS 3
#define DOMEIN "tri"


//�n���V�F�[�_�[�L�q

//�R���g���[���|�C���g = ���_?

//���̓R���g���[���|�C���g
struct VS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;


	//float4 worldPos:POSITION;
	//float4 svpos : SV_POSITION;
	float4 pos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

//Output����R���g���[���|�C���g
struct HS_CONTROL_POINT_OUTPUT
{
	//float3 vPosition : WORLDPOS;

	/*float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;*/
	float4 pos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};


//�o�̓p�b�`�萔�f�[�^
struct HS_CONSTANT_DATA_OUTPUT
{
	//�p�b�`�̊e���_�̃e�b�Z���[�V������(�ǂ̂��炢�������邩?)(�O���̕ӂ��ǂ̂��炢�������邩?)
	//[]���͒��_��?
	//�����ڂ����ׂ����������ꍇ�́A���������l��傫������΂���?
	float EdgeTessFactor[3] : SV_TessFactor;
	
	//�p�b�`�T�[�t�F�X���̃e�b�Z���[�V�����̗�(�|���S���̓����̕�����?)
	//�X�v���C�g���΂�΂�ɂ���Ƃ���g�����Ƃ��Ȃǂ̕��ׂ���������(�����B��)���ׂ����������Ƃ��͂���̒l��傫������?
	float InsideTessFactor : SV_InsideTessFactor;

};





//�h���C���V�F�[�_�[�L�q
//�h���C���V�F�[�_�[�̃A�E�g�v�b�g
struct DS_OUTPUT
{
	//float4 vPosition  : SV_POSITION;

	float4 worldPos :POSITION;
	float4 svpos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv: TEXCOORD;
};







struct VSOutput
{
	/*float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;*/

	float4 pos : SV_POSITION;
	float3 normal:NORMAL;
	float2 uv : TEXCOORD;
};

struct GSOutput
{
	float4 worldPos:POSITION;
	float4 svpos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};


//��{�I�ȏ��
static const int DIR_LIGTH_MAX = 20;
cbuffer cbuff0 : register(b0)
{
	float4 lightColor[DIR_LIGTH_MAX];
	float4 light[DIR_LIGTH_MAX];
	float4 cameraPos;
	matrix mat;
	matrix normalMat;
	matrix worldMat;
	float4 mulColor;
	float4 addColor;
	float4 subColor;
	float ex;
};

//�}�e���A��
cbuffer cbuff2 : register(b2)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);


};

//PBR�}�e���A��
//cbuffer cbuff4 : register(b4)
//{
//	float3 baseColor;
//	float metalness;
//	float specular;
//	float roughness;
//}

//�F
cbuffer cbuff4 : register(b4)
{
	float4 baseColor;
}

//�X�L�j���O
cbuffer cbuff3 : register(b3)
{
	matrix matSkinning[64];
}

