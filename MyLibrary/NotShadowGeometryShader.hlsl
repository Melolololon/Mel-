#include"NotShadowShaderHeader.hlsli"


[maxvertexcount(3)]//�ő咸�_��
void GSmain
(
	//triangle�̓v���~�e�B�u�̂���
	//[3]�́A�g���C�A���O�����X�g�A�X�g���b�v�Ƃ�������
	triangle VSOutput input[3],
	inout TriangleStream< GSOutput > output
)
{
	//�ʂ𑝂₷���@
	//svpos��2����āA���W�����炵�����̂�2�ڂɓ����?
	//output��2��s��?
	//���_�𑝂₹�Ζʂ�������
	//���_�����ԕӂ̊Ԃɒ��_���Z�b�g����
	//�ʕ����̓e�b�Z���[�^�ł�������������炵��
	GSOutput element;

	//������
	//int level = 2;

	//�ӂ̒��������߂��ɁA�d�S�����߂��ق�������?

	//(���_1X + ���_2X) / ������ = �ӂ̏�̒��_�̊ԊuX
	//(���_1Y + ���_2Y) / ������ = �ӂ̏�̒��_�̊ԊuY

	//input[0]�̒��_�ƁAinput[1]�̒��_�̊Ԃɂ��钸�_�̊Ԋu(�����level2�Ȃ̂ŁA�ӂ̒��S)
	/*float4 kankaku[3];
	kankaku[0] = float4(float3((input[0].svpos.xyz + input[1].svpos.xyz) / (float)level), 1);
	kankaku[1] = float4(float3((input[1].svpos.xyz + input[1].svpos.xyz) / (float)level), 1);
	kankaku[2] = float4(float3((input[2].svpos.xyz + input[1].svpos.xyz) / (float)level), 1);*/



	//�@�������߂Ă���
	//normal���g����悤�ɂ���
	/*float3 faceEdgeA = input[1].svpos - input[0].svpos;
	float3 faceEdgeB = input[2].svpos - input[0].svpos;
	float3 faceNormal = normalize(cross(faceEdgeA, faceEdgeB));*/


	for (uint i = 0; i < 3; i++)
	{
		//element.svpos = input[i].svpos + float4(faceNormal * ex, 1);
		element.svpos = input[i].svpos + float4(input[i].normal  * ex, 1);

		element.normal = input[i].normal;
		element.uv = input[i].uv;
		output.Append(element);
	}


	/*element.svpos = input[0].svpos;
	element.normal = input[0].normal;
	element.uv = input[0].uv;
	output.Append(element);
	element.svpos = kankaku[0]; de32
	output.Append(element);
	element.svpos = kankaku[2];
	output.Append(element);

	element.svpos = input[1].svpos;
	output.Append(element);
	element.svpos = kankaku[0];
	output.Append(element);
	element.svpos = kankaku[1];
	output.Append(element);

	element.svpos = kankaku[0];
	output.Append(element);
	element.svpos = kankaku[1];
	output.Append(element);
	element.svpos = kankaku[2];
	output.Append(element);

	element.svpos = input[2].svpos;
	output.Append(element);
	element.svpos = kankaku[1];
	output.Append(element);
	element.svpos = kankaku[2];
	output.Append(element);*/

	output.RestartStrip();

}