#include "DirectX11On12.h"

DirectX11On12* DirectX11On12::GetInstance()
{
	static DirectX11On12 instance;
	return &instance;
}

void DirectX11On12::Initialize(ID3D12Device* dev12, ID3D12CommandQueue* queue12)
{
	UINT flag =
		D3D11_CREATE_DEVICE_DEBUG
		|| D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	
	auto result = D3D11On12CreateDevice
	(
		dev12,
		flag,
		nullptr,//nullptr�ɂ����D3D12�̕��Ɠ������x���ɂȂ�
		0,//��̈����̔z��(����͂����Ŏw�肵�Ȃ�����0)
		reinterpret_cast<IUnknown**>(&queue12),//D3D12�̃L���[�̃|�C���^���L���X�g�������̂ł���?
		1,//�L���[�̐�
		0,//D3D12�f�o�C�X�̃m�[�h
		&device11,
		&context,
		nullptr//�I�΂ꂽ���x�����擾����ꍇ�|�C���^��n��
	);

	//���꒲�ׂ�
	device11.As(&device11On12);

}
