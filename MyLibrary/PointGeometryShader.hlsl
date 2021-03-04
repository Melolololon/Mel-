#include"PointShaderHeader.hlsli"
static const uint vertNum = 4;

static const float4 ofset_array[vertNum] = 
{
	float4(-0.5f , - 0.5f,0,0),
	float4(-0.5f , + 0.5f,0,0),
	float4(+0.5f , - 0.5f,0,0),
	float4(+0.5f , + 0.5f,0,0)
	
	/*float4(-0.7, -0.7,0,1),
	float4(-0.7 ,+0.7,0,1),
	float4(+0.7 ,-0.7,0,1),
	float4(+0.7, +0.7,0,1)*/
};

static const float2 uvs[vertNum] =
{
	float2(0.0,0.0),
	float2(0.0,1.0),
	float2(1.0,0.0),
	float2(1.0,1.0)

};

[maxvertexcount(vertNum)]
void GSmain(
	point VSOutput input[1] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	for (uint i = 0; i < vertNum; i++)
	{

		//svpos�ɓ���Ă���X�P�[���|����
	//�������ƃr���{�[�h�O�ɃX�P�[����Z���Ă邪�A��������ƍ��W�����������Ȃ�
	//�ǂ����݂����������Ȃ�
		//���������Ȃ�̂́A���[���h���W{0,0,0}�ɓ_�����ĂȂ�����?
		//����ōs��|���ăo�O���Ă�?
		//���i���s�ړ�������Ɋ|���Ă邩��֌W�Ȃ�?
		//�J�����̈ʒu�ɉ����Ă߂��Ⴍ����ړ����Ă�
		//far��20�ɂ��Ď�������v���C���[�Ɠ������W�Ȃ̂Ƀv���C���[������������������m��
	//�s�N�Z���V�F�[�_�[��pos��Ԃ����甒���Ȃ����̂Ŋm��
		//�����ƒl�����^�[���ł��ĂȂ�?
		//�������e��߂Ĕz��̒l�ς����肵�Ē��ő����Ă��T�C�Y�ς���
		//���^�[���ł��Ă�(obj�ł��������ʂɂȂ�������)
		//mat�̏�Z�𒸓_�V�F�[�_�[�ł������ʏ�ʂ�ɕ`��ł���
		//����
		//���ĂȂ�!!
		//�ʏ�̔|���̃X�P�[����ς������Ƀr���{�[�h����Ƃ��������Ȃ�
		//��������������X�P�[����������Ƃ��������Ȃ邩��


		float4 iPos = input[0].svpos;
		float4 offset = ofset_array[i];
		offset.x *= input[0].scale.x;
		offset.y *= input[0].scale.y;
		offset = mul(billboardMat, offset);
		element.svpos = iPos + offset;
		element.svpos = mul(mat, element.svpos);
		element.uv = uvs[i];
		element.color = input[0].color;


	//	float4 iPos = input[0].svpos;
	//	float4 offset = ofset_array[i];
	///*	offset.x *= input[0].scale.x;
	//	offset.y *= input[0].scale.y;
	//	offset = mul(billboardMat, offset);*/
	//	//element.svpos = iPos + offset;
	//	//element.svpos = mul(mat, element.svpos);
	//	element.uv = uvs[i];
	//	element.color = input[0].color;

	//	element.svpos = iPos+ offset ;


		output.Append(element);
	}
}