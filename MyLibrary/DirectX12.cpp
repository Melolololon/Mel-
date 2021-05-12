#include "DirectX12.h"
#include"LibMath.h"
#include"FbxLoader.h"
#include"ObjModel.h"
#include"PrimitiveModel.h"


DirectX12::DirectX12()
{
}


DirectX12::~DirectX12()
{


	//#ifdef _DEBUG
	//
	//	ComPtr<ID3D12DebugDevice> debugDevice;
	//	//デバッグデバイスの作成
	//	dev->QueryInterface(IID_PPV_ARGS(&debugDevice));
	//	//出力
	//	debugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY);
	//#endif // _DEBUG


}

DirectX12* DirectX12::GetInstance()
{
	static DirectX12 dx12;
	return &dx12;
}


void DirectX12::Initialize(HWND hwnd, int windouWidth, int windowHeight)
{

#pragma region 変数初期化

	cameraMat = DirectX::XMMatrixIdentity();
#pragma region Windows

	this->hwnd = hwnd;
	winWidth = windouWidth;
	winHeight = windowHeight;

#pragma endregion

#pragma region 基本的なオブジェクト

	//dev = nullptr;
	//dxgiFactory = nullptr;
	//swapchain = nullptr;
	//cmdAllocator = nullptr;
	//cmdList = nullptr;
	//cmdQueue = nullptr;
	//rtvHeaps = nullptr;
	//depthHeap = nullptr;


	//pipelineState = nullptr;
	//rootSignature = nullptr;
#pragma endregion

	heapDesc = {};
	barrierDesc = {};


	loadTextureCounter = 0;
	createSpriteCounter = 0;
	startPipelineCreateNum = 0;


#pragma region カメラ初期化

	mainCameraData.eye = { 0,0,-5 };
	mainCameraData.target = { 0,0,0 };
	mainCameraData.up = { 0,1,0 };
	mainCameraData.nowEye = { 0,0,-5 };
	mainCameraData.nowTarget = { 0,0,0 };
	mainCameraData.nowUp = { 0,1,0 };
	mainCameraData.eyeAngle = { 0,0,0 };
	mainCameraData.targetAngle = { 0,0,0 };
	mainCameraData.upAngle = { 0,0,0 };
	mainCameraData.fovY = 60.0f;
	mainCameraData.nearNumber = 0.1f;
	mainCameraData.farNumber = 40.0f;

	eyeVelocity = { 0,0,0 };
	targetVelocity = { 0,0,0 };

#pragma endregion

#pragma region ライト初期化

	lightVector = { 0,0,1 };
	lightColor = { 1,1,1 };
#pragma endregion

#pragma region 見た目
	isPlane = false;

	isBillBoardX = false;
	isBillBoardY = false;
	isBillBoardZ = false;

	smoothingFlag = false;
#pragma endregion

	createPolygon = CreatePolygon::GetInstance();

	mainCamera = new CreateCamera(winWidth, winHeight);
	DirectInput::SetMatrixAndNearFar
	(
		mainCamera->GetViewMatrix(mainCameraData),
		mainCamera->GetProjectionMatrix(mainCameraData),
		mainCameraData.nearNumber,
		mainCameraData.farNumber
	);

	gpipeline = {};
	spriteGpipeline = {};
	blenddesc = {};

	//通常パイプラインが3番なので、3
	pipelineNum = 3;
	spritePipelineNum = 0;
	sprite3DPipelineNum = 0;
	currentPostEffectPipeline = 0;
	spriteFontDrawCounter = 0;

	constData3D = nullptr;
	userConstData3D = nullptr;
	userConstDataSize3D = 0;

	materialData = {};

	postEffectCametaFlag = false;
#pragma endregion


#pragma region デバッグレイヤー
#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugController;

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // _DEBUG

#pragma endregion

#pragma region DirectX処理

	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));

#pragma region アダプタの列挙

	std::vector<IDXGIAdapter*> adapters;
	IDXGIAdapter* tmpAdapter;
	for (int i = 0; dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);
	}


	for (int i = 0; i < (int)adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"Microsoft") == std::wstring::npos/*&& strDesc.find(L"Inter") == std::wstring::npos*/)
		{
			tmpAdapter = adapters[i];
			break;
		}
	}



	for (int i = 0; i < (int)adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"NVIDIA") != std::wstring::npos)
		{
			tmpAdapter = adapters[i];
			break;
		}
	}

	for (int i = 0; i < (int)adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);
		std::wstring strDesc = adesc.Description;
		if (strDesc.find(L"AMD") != std::wstring::npos)
		{
			tmpAdapter = adapters[i];
			break;
		}
	}

#pragma endregion

#pragma region デバイスの生成

	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	D3D_FEATURE_LEVEL featureLevel;

	for (int i = 0; i < _countof(levels); i++)
	{
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			featureLevel = levels[i];
			break;
		}
	}


	if (dev != nullptr)
	{
		CreateBuffer::GetInstance()->Initialize(dev.Get(), windouWidth, windowHeight);
		createBuffer = CreateBuffer::GetInstance();
		CreatePipelineState = new CreatePipeline(dev.Get());
	}

	tmpAdapter->Release();




#pragma endregion

#pragma region アロケーター リスト生成
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator));

	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
#pragma endregion

#pragma region コマンドキュー作成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));


#pragma endregion

#pragma region スワップチェーンの生成
	ComPtr<IDXGISwapChain1>swapchain1;
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = winWidth;
	swapchainDesc.Height = winHeight;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Count = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapchainDesc.BufferCount = 2;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1
	);
	swapchain1.As(&swapchain);

#pragma endregion

#pragma region レンダーターゲットビュー

	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 2;
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

	for (int i = 0; i < 2; i++)
	{
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffer[i]));
		D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		dev->CreateRenderTargetView
		(
			backBuffer[i].Get(),
			nullptr,
			handle
		);
	}

#pragma endregion

#pragma region 深度バッファ
	//深度用ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	result = dev->CreateDescriptorHeap
	(
		&dsvHeapDesc,
		IID_PPV_ARGS(&depthHeap)
	);

	D3D12_CPU_DESCRIPTOR_HANDLE depthHeapHandle = depthHeap->GetCPUDescriptorHandleForHeapStart();


	D3D12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_D32_FLOAT,
		winWidth,
		winHeight,
		1, 0, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	createBuffer->CreateDepthBufferSet(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), depthResDesc, CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0), depthHeapHandle, depthBufferSet);


#pragma endregion

#pragma region フェンス作成
	fence = nullptr;
	fenceVal = 0;
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion


#pragma region ルートシグネチャ_ディスクリプタレンジ_ルートパラメーター


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	D3D12_DESCRIPTOR_RANGE descRangeCSV{};
	descRangeCSV.NumDescriptors = 1;
	descRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	descRangeCSV.BaseShaderRegister = 0;
	descRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE userDescRangeCSV{};
	userDescRangeCSV.NumDescriptors = 1;
	userDescRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	userDescRangeCSV.BaseShaderRegister = 1;
	userDescRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE materialDescRangeCSV{};
	materialDescRangeCSV.NumDescriptors = 1;
	materialDescRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	materialDescRangeCSV.BaseShaderRegister = 2;
	materialDescRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE commonRangeCSV{};
	commonRangeCSV.NumDescriptors = 1;
	commonRangeCSV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	commonRangeCSV.BaseShaderRegister = 3;
	commonRangeCSV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//D3D12_DESCRIPTOR_RANGE boneRangeCRV{};
	//boneRangeCRV.NumDescriptors = 1;
	//boneRangeCRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	//boneRangeCRV.BaseShaderRegister = 5;
	//boneRangeCRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	D3D12_DESCRIPTOR_RANGE descRangeSRV{};
	descRangeSRV.NumDescriptors = 1;
	descRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descRangeSRV.BaseShaderRegister = 0;
	descRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



	D3D12_ROOT_PARAMETER rootparam[5]{};

	//テクスチャ
	rootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[0].DescriptorTable.pDescriptorRanges = &descRangeSRV;
	rootparam[0].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//定数
	rootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[1].DescriptorTable.pDescriptorRanges = &descRangeCSV;
	rootparam[1].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//ユーザー定数
	rootparam[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[2].DescriptorTable.pDescriptorRanges = &userDescRangeCSV;
	rootparam[2].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//マテリアル
	rootparam[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[3].DescriptorTable.pDescriptorRanges = &materialDescRangeCSV;
	rootparam[3].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	//共通
	rootparam[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[4].DescriptorTable.pDescriptorRanges = &commonRangeCSV;
	rootparam[4].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	//ボーンデータ
	/*rootparam[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootparam[5].DescriptorTable.pDescriptorRanges = &boneRangeCRV;
	rootparam[5].DescriptorTable.NumDescriptorRanges = 1;
	rootparam[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;*/

	rootSignatureDesc.pParameters = rootparam;
	rootSignatureDesc.NumParameters = _countof(rootparam);


	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	CreatePipelineState->CreateRootSigneture(rootSignatureDesc, &rootSignature);



#pragma endregion

#pragma region グラフィックパイプライン


	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;//カリング設定

	//ポリゴン内を塗りつぶすかどうか(ワイヤーフレームかそうじゃないか設定)
	//プリミティブトポロジとかジオメトリのラインストリップと違って、インデックスに関係なく線が引かれる
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	gpipeline.RasterizerState.DepthClipEnable = true;

	//深度
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//ブレンド
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;//どの色をブレンドできるようにするかの設定?ALLだと全ての色とαがブレンド対象?
	blenddesc.BlendEnable = true;//ブレンドを有効にするかのフラグ
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算(不透明度が100%以外の時自分と後ろの色を足す)
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//gpipeline.BlendState.AlphaToCoverageEnable = true;//透過がおかしくならないようにする処理(これtrueにすると水しぶき消えちゃうからパイプライン分ける?)
	//深度が100%じゃないと無視される?テクスチャで深度設定してたら問題ない?それとも描画順で変わる?

	gpipeline.BlendState.AlphaToCoverageEnable = false;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	gpipeline.SampleDesc.Count = 1;

	gpipeline.pRootSignature = rootSignature.Get();
#pragma endregion

#pragma region パイプライン生成
	Create3DObjectPipeline();
	
	std::vector<InputLayoutData>ilData;
#pragma endregion

#pragma region スプライト用設定生成
	spriteGpipeline = gpipeline;

	//スプライト用
	//ラスタライザ、カリング
	spriteGpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	spriteGpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	//深度
	spriteGpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	spriteGpipeline.DepthStencilState.DepthEnable = false;
	spriteGpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;



	//αの書き込み
	spriteGpipeline.BlendState.AlphaToCoverageEnable = false;

	CD3DX12_DESCRIPTOR_RANGE spriteDescRangeSRV;
	spriteDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER spriteRootparam[2]={};
	spriteRootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	spriteRootparam[1].InitAsDescriptorTable(1, &spriteDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL);


	rootSignatureDesc.pParameters = spriteRootparam;
	rootSignatureDesc.NumParameters = _countof(spriteRootparam);

	CreatePipelineState->CreateRootSigneture(rootSignatureDesc, &spriteRootsignature);

	spriteGpipeline.pRootSignature = spriteRootsignature.Get();
	spritePipelineStates.resize(1);
	CreatePipelineState->CreatePipelineState(0, spriteGpipeline, &spritePipelineStates[0]);


	//3Dスプライト
	sprite3DGpipeline = spriteGpipeline;
	sprite3DGpipeline.DepthStencilState.DepthEnable = true;
	sprite3DGpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;

	sprite3DPipelineStates.resize(1);
	CreatePipelineState->CreatePipelineState(0, sprite3DGpipeline, &sprite3DPipelineStates[0]);
#pragma endregion

#pragma region ポイント用設定生成
	pointGpipeline = gpipeline;

	CD3DX12_DESCRIPTOR_RANGE pointDescRangeSRV;
	pointDescRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER pointRootparam[2];
	pointRootparam[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	pointRootparam[1].InitAsDescriptorTable(1, &pointDescRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	rootSignatureDesc.pParameters = pointRootparam;
	rootSignatureDesc.NumParameters = _countof(pointRootparam);

	CreatePipelineState->CreateRootSigneture(rootSignatureDesc, &pointRootsignature);

	pointGpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	pointGpipeline.pRootSignature = pointRootsignature.Get();
	pointPipelineStates.resize(1);


	ilData.resize(3);
	ilData[0] = { "POSITION", 3 ,FORMAT_TYPE_FLOAT };
	ilData[1] = { "TEXCOORD", 2,FORMAT_TYPE_FLOAT };
	ilData[2] = { "COLOR", 4 ,FORMAT_TYPE_FLOAT };
	CreatePipelineState->SetInputLayout(ilData);
	ilData.clear();


	pointGpipeline.BlendState.AlphaToCoverageEnable = true;
	pointGpipeline.DepthStencilState.DepthEnable = true;
	CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/PointVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/PointGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"../MyLibrary/PointPixelShader.hlsl", "PSmain", "ps_5_0" },
		pointGpipeline,
		&pointPipelineStates[0],
		true
	);
	CreatePipelineState->DeleteInputLayout();
#pragma endregion



#pragma region スプライト用ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC spriteHeapDesc = {};

	spriteHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	spriteHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	spriteHeapDesc.NumDescriptors = 256 * 5;
	dev->CreateDescriptorHeap(&spriteHeapDesc, IID_PPV_ARGS(&spriteHeap));



	//べた塗テクスチャ用意
	loadTextureCounter++;

	//ここDrawBox書いてないときに処理すると下のやつでエラー出る
	//6/1 なぜかテクスチャが出なくなった
	//uvの設定を頂点バッファ生成後にしていた
	TextureBufferSet textureSet;
	spriteTextureBufferSet.push_back(textureSet);
	spriteTextureBufferSet[loadTextureCounter - 1].textureBuffer.resize(1);

	D3D12_CPU_DESCRIPTOR_HANDLE spriteHandle = spriteHeap->GetCPUDescriptorHandleForHeapStart();

	createBuffer->CreateTextureBufferSet2
	(
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			spriteHandle,
			loadTextureCounter - 1,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		{ 0,0,0,0 },
		spriteTextureBufferSet[loadTextureCounter - 1],
		0
	);

	spriteTextureData.resize(spriteTextureData.size() + 1);
#pragma endregion

#pragma region スプライトフォント準備
	//フォント作成
	for (int i = 0; i < SPRITEFONT_MAX; i++)
	{
		CreateSprite(nullptr, true);
	}

	spriteHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	spriteHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	spriteHeapDesc.NumDescriptors = 256;//読み込めるフォント数
	dev->CreateDescriptorHeap(&spriteHeapDesc, IID_PPV_ARGS(&spriteFontHeap));
#pragma endregion


#pragma endregion

#pragma region ポストエフェクト



#pragma region 描画先板ポリのリソース作成
	postEfectConstBuffers.resize(1);
	postEfectConstBuffers.reserve(20);

	//D3D12_CLEAR_VALUE リソースをレンダーターゲットとして使う場合にどう初期化するかをまとめたもの
	D3D12_CLEAR_VALUE peClesrValue;


	/*peClesrValue.Color[0] = clearColor[0];
	peClesrValue.Color[1] = clearColor[1];
	peClesrValue.Color[2] = clearColor[2];
	peClesrValue.Color[3] = clearColor[3];
	peClesrValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	D3D12_DEPTH_STENCIL_VALUE peSepthStencilValue;
	peSepthStencilValue.Depth = 1.0f;
	peSepthStencilValue.Stencil = 0.0f;
	peClesrValue.DepthStencil = peSepthStencilValue;*/

	peClesrValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
	//リソース作成
	postEffectResources.resize(1);

	dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&backBuffer[0].Get()->GetDesc(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&peClesrValue,
		IID_PPV_ARGS(&postEffectResources[0])
	);

#pragma region ヒープとビュー作成
	//テクスチャ

	//ヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC peHeapDesc{};
	peHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	peHeapDesc.NumDescriptors = 2;
	peHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	peHeapDesc.NodeMask = 0;
	dev->CreateDescriptorHeap(&peHeapDesc, IID_PPV_ARGS(&postEffectHeap));

	//ビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};

	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView
	(
		postEffectResources[0].Get(),
		&srvDesc,
		postEffectHeap.Get()->GetCPUDescriptorHandleForHeapStart()
	);

	//定数バッファ作成
	postEffectWorldMatData.resize(1);
	postEffectWorldMatData[0].scale = { 1,1,1 };

	postEfectConstBuffers[0].constBuffer.resize(1);
	createBuffer->CreateConstBufferSet(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer((sizeof(PostEffectConstData) + 0xff)&~0xff),
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			postEffectHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
			1,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		(void**)&postEffectConstDataP,
		postEfectConstBuffers[0],
		0);
	postEffectConstDataP->worldMat = DirectX::XMMatrixIdentity();
	//postEfectConstBuffers[0].constBuffer[0].Get()->Unmap(0, nullptr);

	//レンダーターゲット

	//ヒープ作成
	peHeapDesc = {};
	peHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	peHeapDesc.NumDescriptors = 10;
	dev->CreateDescriptorHeap(&peHeapDesc, IID_PPV_ARGS(&postEffectRTVHeap));

	//ビュー作成
	D3D12_RENDER_TARGET_VIEW_DESC peRTVDesc = {};
	peRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	peRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	dev.Get()->CreateRenderTargetView
	(
		postEffectResources[0].Get(),
		&peRTVDesc,
		postEffectRTVHeap.Get()->GetCPUDescriptorHandleForHeapStart()
	);
#pragma endregion

#pragma endregion

#pragma region 描画先板ポリの頂点情報作成

	postEffectVertex.resize(4);

	postEffectVertex[0] = Vertex({ {-1,-1,0},{0,1},{0,0,0} });
	postEffectVertex[1] = Vertex({ {-1,1,0},{0,0},{0,0,0} });
	postEffectVertex[2] = Vertex({ {1,-1,0},{1,1},{0,0,0} });
	postEffectVertex[3] = Vertex({ {1,1,0},{1,0},{0,0,0} });

	createBuffer->CreateVertexBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * 4),
		postEffectVertex,
		postEffectVertexBufferSet
	);
#pragma endregion

#pragma region ルートシグネチャとパイプラインを生成
	postEffectPipeline.resize(1);
	postEffectPipeline.reserve(20);

	//レンジ
	D3D12_DESCRIPTOR_RANGE peRangeSRV{};
	peRangeSRV.NumDescriptors = 1;
	peRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	peRangeSRV.BaseShaderRegister = 0;
	peRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE peRangeCBV{};
	peRangeCBV.NumDescriptors = 1;
	peRangeCBV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
	peRangeCBV.BaseShaderRegister = 0;
	peRangeCBV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	//ルートパラメータ
	D3D12_ROOT_PARAMETER peRootparam[2]{};

	//テクスチャ
	peRootparam[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	peRootparam[0].DescriptorTable.pDescriptorRanges = &peRangeSRV;
	peRootparam[0].DescriptorTable.NumDescriptorRanges = 1;
	peRootparam[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	peRootparam[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	peRootparam[1].DescriptorTable.pDescriptorRanges = &peRangeCBV;
	peRootparam[1].DescriptorTable.NumDescriptorRanges = 1;
	peRootparam[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	rootSignatureDesc.pParameters = peRootparam;
	rootSignatureDesc.NumParameters = _countof(peRootparam);

	/*D3D12_ROOT_SIGNATURE_DESC d = {};
	d.NumParameters = 0;
	d.NumStaticSamplers = 0;
	d.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;*/

	//ルートシグネチャ
	CreatePipelineState->CreateRootSigneture(rootSignatureDesc, &postEffectRootSigneture);

	////パイプライン
	
	ilData.resize(2);
	ilData[0] = { "POSITION", 3 ,FORMAT_TYPE_FLOAT };
	ilData[1] = { "TEXCOORD", 2 ,FORMAT_TYPE_FLOAT };
	CreatePipelineState->SetInputLayout(ilData);
	ilData.clear();

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pePLDesc = spriteGpipeline;
	pePLDesc.pRootSignature = postEffectRootSigneture.Get();


	//パイプラインの設定
	pePLDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	pePLDesc.BlendState.AlphaToCoverageEnable = false;


	//バックバッファの色と加算したくないので、false
	blenddesc.BlendEnable = false;//ブレンドを有効にするかのフラグ
	pePLDesc.BlendState.RenderTarget[0] = blenddesc;

	CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/PostEffectVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		{ L"../MyLibrary/PostEffectPixelShader.hlsl","PSmain","ps_5_0" },
		pePLDesc,
		&postEffectPipeline[0],
		true
	);

	CreatePipelineState->DeleteInputLayout();

#pragma endregion


#pragma endregion

#pragma region トゥーンレンダリング用レンダーターゲット(仮)
	//アプリケーション側でレンダーターゲットの上に描画して、
	//その上に線を入れようとして作ったやつ
	

//	toonShaderPipeline.resize(1);
//	toonShaderPipeline.reserve(20);
//
//	D3D12_CLEAR_VALUE tsClesrValue;
//
//
//	tsClesrValue = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);
//	//リソース作成
//	toonShaderResources.resize(1);
//
//	dev->CreateCommittedResource
//	(
//		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
//		D3D12_HEAP_FLAG_NONE,
//		&backBuffer[0].Get()->GetDesc(),
//		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
//		&peClesrValue,
//		IID_PPV_ARGS(&toonShaderResources[0])
//	);
//
//
//#pragma region ヒープとビュー作成
//	//テクスチャ
//
//	//ヒープ作成
//	D3D12_DESCRIPTOR_HEAP_DESC tsHeapDesc{};
//	tsHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	tsHeapDesc.NumDescriptors = 10;
//	tsHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	tsHeapDesc.NodeMask = 0;
//	dev->CreateDescriptorHeap(&tsHeapDesc, IID_PPV_ARGS(&toonShaderHeap));
//
//	//ビュー作成
//	D3D12_SHADER_RESOURCE_VIEW_DESC tsSrvDesc{};
//
//	tsSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	tsSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	tsSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	tsSrvDesc.Texture2D.MipLevels = 1;
//
//	dev->CreateShaderResourceView
//	(
//		toonShaderResources[0].Get(),
//		&tsSrvDesc,
//		toonShaderHeap.Get()->GetCPUDescriptorHandleForHeapStart()
//	);
//
//
//	//レンダーターゲット
//
//	//ヒープ作成
//	tsHeapDesc = {};
//	tsHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
//	tsHeapDesc.NumDescriptors = 10;
//	dev->CreateDescriptorHeap(&tsHeapDesc, IID_PPV_ARGS(&toonShaderRTVHeap));
//
//	//ビュー作成
//	D3D12_RENDER_TARGET_VIEW_DESC tsRTVDesc = {};
//	tsRTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
//	tsRTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//
//	dev.Get()->CreateRenderTargetView
//	(
//		toonShaderResources[0].Get(),
//		&tsRTVDesc,
//		toonShaderRTVHeap.Get()->GetCPUDescriptorHandleForHeapStart()
//	);
//#pragma endregion
//
//	////パイプライン
//	createPipeline->setInputLayout("POSITION", 3);
//	createPipeline->setInputLayout("TEXCOORD", 2);
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC tsPLDesc = spriteGpipeline;
//	tsPLDesc.pRootSignature = postEffectRootSigneture.Get();
//
//	//ここ変更する
//	createPipeline->createUserPipeline
//	(
//		1,
//		{ L"../MyLibrary/PostEffectVertexShader.hlsl","VSmain","vs_5_0" },
//		{ L"NULL","","" },
//		{ L"../MyLibrary/PostEffectPixelShader.hlsl","PSmain","ps_5_0" },
//		tsPLDesc,
//		&toonShaderPipeline[0],
//		true
//	);
//
//	createPipeline->deleteInputLayout();

#pragma endregion

#pragma region FbxManager初期化(仮記述)
	FbxLoader::GetInstance()->Initialize(dev.Get());
#pragma endregion


	std::vector<ID3D12GraphicsCommandList*>cmdLists(1);
	cmdLists[0] = cmdList.Get();

	PipelineState::Initialize(dev.Get());
	Model::Initialize
	(
		dev.Get(),
		cmdLists
	);
	Model::CreateCommonBuffer();

	
	ObjModel::Initialize();
	FbxModel::Initialize();
	PrimitiveModel::Initialize();

	Sprite::Initialize(dev.Get(), cmdList.Get());
	Sprite2D::Initialize(windouWidth, windowHeight);
	Sprite3D::Initialize();




}

void DirectX12::LoopStartProcess()
{
#pragma region DInput
#pragma endregion

#pragma region バリア生成_PSResourceからRTVへ

	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	/*barrierDesc.Transition.pResource = backBuffer[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;*/

	barrierDesc.Transition.pResource = postEffectResources[0].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 画面クリア

	//D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	//rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = postEffectRTVHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = depthHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//画面のクリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);



#pragma region ビューポート_シザー矩形

	D3D12_VIEWPORT viewport{};
	viewport.Width = (float)winWidth;
	viewport.Height = (float)winHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	cmdList->RSSetViewports(1, &viewport);


	D3D12_RECT scissorrect{};
	scissorrect.left = 0;
	scissorrect.right = scissorrect.left + winWidth;
	scissorrect.top = 0;
	scissorrect.bottom = scissorrect.top + winHeight;
	cmdList->RSSetScissorRects(1, &scissorrect);

#pragma endregion

#pragma endregion

	Model::SetViewAndProjectionMat(mainCamera->Get3DCameraMatrix(mainCameraData));
	Sprite3D::SetViewAndProjectionMatrix(mainCamera->Get3DCameraMatrix(mainCameraData));
	Sprite3D::SetCameraPosTargetUpVector(mainCameraData.nowEye, mainCameraData.nowTarget, mainCameraData.nowUp);
}

void DirectX12::LoopEndProcess()
{

	CommonConstData commonData;
	commonData.cameraPos = { mainCameraData.nowEye.x,mainCameraData.nowEye.y,mainCameraData.nowEye.z,1 };
	commonData.light = { lightVector.x,lightVector.y,lightVector.z,1 };
	commonData.lightColor = { lightColor.x,lightColor.y,lightColor.z,1 };
	commonData.lightMat = cameraMat;
	//共通バッファのMap
	for (auto& cBuf : commonBuffers)
	{
		cBuf.second->Map(0, nullptr, (void**)&commonConstData3D);
		commonConstData3D->lightColor = commonData.lightColor;
		commonConstData3D->light = commonData.light;
		commonConstData3D->lightMat = commonData.lightMat;
		commonConstData3D->cameraPos = commonData.cameraPos;

		cBuf.second->Unmap(0, nullptr);
	}
	Model::MapCommonConstData(commonData);

	//ポストエフェクトレンダーターゲットのMap
	DirectX::XMMATRIX peWorldMat = DirectX::XMMatrixIdentity();
	peWorldMat *= DirectX::XMMatrixScaling
	(
		postEffectWorldMatData[0].scale.x,
		postEffectWorldMatData[0].scale.y,
		postEffectWorldMatData[0].scale.z
	);
	peWorldMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(postEffectWorldMatData[0].angle.z));
	peWorldMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(postEffectWorldMatData[0].angle.x));
	peWorldMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(postEffectWorldMatData[0].angle.y));

	peWorldMat *= DirectX::XMMatrixTranslation
	(
		postEffectWorldMatData[0].pos.x,
		postEffectWorldMatData[0].pos.y,
		postEffectWorldMatData[0].pos.z
	);
	if (postEffectCametaFlag)peWorldMat *= mainCamera->Get3DCameraMatrix(mainCameraData);

	postEffectConstDataP->worldMat = peWorldMat;


	//Mapここまで
	
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	cmdList->ResourceBarrier(1, &barrierDesc);

	//板ポリをバックバッファーに描画する準備
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	barrierDesc.Transition.pResource = backBuffer[bbIndex].Get();
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	cmdList->ResourceBarrier(1, &barrierDesc);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvH = postEffectResourcesRTVHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = depthHeap.Get()->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//画面のクリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma region ポストエフェクト用板ポリの描画コマンドセット

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	std::vector<ID3D12DescriptorHeap*> ppHeaps;

	////ハンドルvectorやめてfor内に毎回宣言したほうがいい?そうすればずらしたやつを勝手に直してくれる
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	////cmdList->SetGraphicsRootSignature(postEffectRootSigneture.Get());
	cmdList->SetGraphicsRootSignature(postEffectRootSigneture.Get());
	cmdList->SetPipelineState(postEffectPipeline[currentPostEffectPipeline].Get());
	//cmdList->SetPipelineState(spritePipelineStates[0].Get());

	cmdList->IASetVertexBuffers(0, 1, &postEffectVertexBufferSet.vertexBufferView);
	//cmdList->IASetIndexBuffer(&spriteIndexBufferSet[0].indexBufferView);

	ppHeaps.push_back(postEffectHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

	gpuDescHandle = postEffectHeap->GetGPUDescriptorHandleForHeapStart();
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		gpuDescHandle, 
		0, 
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	//////テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

	//定数
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
	(
		gpuDescHandle, 
		1,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	cmdList->DrawInstanced(4, 1, 0, 0);
	//cmdList->DrawIndexedInstanced(spriteIndices[0].size(), 1, 0, 0, 0);


#pragma endregion


#pragma region RTVからPRESENTへ
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	cmdList->ResourceBarrier(1, &barrierDesc);

	//barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//cmdList->ResourceBarrier(1, &barrierDesc);
#pragma endregion

#pragma region 実行
	cmdList->Close();
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };
	cmdQueue->ExecuteCommandLists(1, cmdLists);

	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();
	cmdList->Reset(cmdAllocator.Get(), nullptr);

	swapchain->Present(1, 0);//VSYNC
#pragma endregion

	//スプライトフォントの表示回数をリセット
	spriteFontDrawCounter = 0;

}

void DirectX12::Finalize()
{
	delete CreatePipelineState;
	delete mainCamera;

	DirectInput::Release();
	FbxLoader::GetInstance()->Finalize();
}


void DirectX12::SetScreenColor(Color screenColor)
{
	clearColor[0] = (float)screenColor.r / 255;
	clearColor[1] = (float)screenColor.g / 255;
	clearColor[2] = (float)screenColor.b / 255;
	clearColor[3] = (float)screenColor.a / 255;
}

DirectX::XMFLOAT2 DirectX12::GetTextureSize(int textureHandle)
{
	DirectX::XMFLOAT2 size = { static_cast<float>(spriteTextureData[textureHandle].width), static_cast<float>(spriteTextureData[textureHandle].height) };
	return size;
}

#pragma region パイプラインの設定
void DirectX12::SetDespTestFlag(bool flag)
{
	gpipeline.DepthStencilState.DepthEnable = flag;
}

#pragma endregion

#pragma region パイプライン作成

void DirectX12::Create3DObjectPipeline()
{

	pipelineStates.reserve(99);
	ComPtr<ID3D12PipelineState> pState;


	//深度テスト無し
	gpipeline.DepthStencilState.DepthEnable = false;
	CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/ObjVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		gpipeline,
		&pState,
		false
	);
	pipelineStates.push_back(pState);

	//背面カリングしない
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//カリング設定
	CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/ObjVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		gpipeline,
		&pState,
		false
	);
	pipelineStates.push_back(pState);

	//透明部分の深度値を書き込まない
	gpipeline.DepthStencilState.DepthEnable = true;
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	gpipeline.BlendState.AlphaToCoverageEnable = true;
	CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/ObjVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		gpipeline,
		&pState,
		false
	);
	pipelineStates.push_back(pState);

	//gpipeline.BlendState.AlphaToCoverageEnable = true;
	//透過がおかしくならないようにする処理(これtrueにすると水しぶき消えちゃうからパイプライン分ける?)
	//深度が100%じゃないと無視される?テクスチャで深度設定してたら問題ない?それとも描画順で変わる?
	gpipeline.BlendState.AlphaToCoverageEnable = false;

	//マテリアル読み込み可能版
	CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/ObjVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		gpipeline,
		&pState,
		false
	);
	pipelineStates.push_back(pState);

	//ボーン読み込み可能
	std::vector<InputLayoutData>ilData(4);
	ilData[0] = { "POSITION", 3 ,FORMAT_TYPE_FLOAT };
	ilData[1] = { "TEXCOORD", 2 ,FORMAT_TYPE_FLOAT };
	ilData[2] = { "NORMAL", 3,FORMAT_TYPE_FLOAT };
	ilData[3] = { "BONENUM", 1,FORMAT_TYPE_UNSIGNED_INT };
	SetInputLayout(ilData);
	CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/ObjAnimationVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"../MyLibrary/ObjGeometryShader.hlsl","GSmain","gs_5_0" },
		{ L"../MyLibrary/ObjPixelShader.hlsl","PSmain","ps_5_0" },
		gpipeline,
		&pState,
		true
	);
	pipelineStates.push_back(pState);
	CreatePipelineState->DeleteInputLayout();

	startPipelineCreateNum = static_cast<int>(pipelineStates.size());
}

int DirectX12::getStartPipelineCreateNum()
{
	return startPipelineCreateNum;
}

bool DirectX12::CreateUserPipelineState
(
	PipelineData pipelineData,
	ShaderData vShaderData,
	ShaderData gSyaderData,
	ShaderData pShaderData,
	bool useUserInputLayout
)
{

	switch (pipelineData.drawMode)
	{
	case DRAW_SOLID:
		gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		break;

	case DRAW_WIREFRAME:
		gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		break;
	}

	switch (pipelineData.cullMode)
	{
	case CULL_NONE:
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case CULL_FRONT:
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case CULL_BACK:
		gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	}

	switch (pipelineData.blendMode)
	{
	case BLEND_NONE:
		blenddesc.BlendEnable = false;
		break;

	case BLEND_ADD:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
		break;

	case BLEND_SUB:
		blenddesc.BlendEnable = true;
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_SUBTRACT;
		break;
	}
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	switch (pipelineData.depthMode)
	{
	case DEPTH_NONE:
		gpipeline.DepthStencilState.DepthEnable = false;
		break;

	case DEPTH_TRUE:
		gpipeline.DepthStencilState.DepthEnable = true;
		break;
	}

	switch (pipelineData.alphaWriteMode)
	{
	case ALPHA_WRITE_NONE:
		gpipeline.BlendState.AlphaToCoverageEnable = true;
		break;

	case ALPHA_WRITE_TRUE:
		gpipeline.BlendState.AlphaToCoverageEnable = false;

		break;
	}

	pipelineStates.push_back(nullptr);

	return CreatePipelineState->CreateUserPipeline
	(
		1,
		vShaderData,
		gSyaderData,
		pShaderData,
		gpipeline,
		&pipelineStates[pipelineStates.size() - 1],
		useUserInputLayout
	);
}

void DirectX12::setPipelineNumber(int num)
{
	pipelineNum = num;

}
//
//void DirectX12::setMapConstData(void** constData, int heapNum, int number)
//{
//	//constBufferSet[heapNum][number].constBuffer.Get()->Unmap(0, nullptr);
//
//	//constBufferSetにconstData戻す?
//	auto result = constBufferSet[heapNum][number].constBuffer.Get()->Map(0, nullptr, constData);
//}

void DirectX12::setConstMapData(void** dataP, unsigned int dataSize)
{
	userConstData3D = dataP;
	userConstDataSize3D = dataSize;
}

void DirectX12::GetMatrix(float matrix[4][4], const std::string& key, int number)
{
	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	matWorld *= DirectX::XMMatrixScaling
	(
		modelConstData[key][number].scale.x,
		modelConstData[key][number].scale.y,
		modelConstData[key][number].scale.z
	);
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstData[key][number].angle.z));
	matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstData[key][number].angle.x));
	matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstData[key][number].angle.y));
	
	matWorld *= DirectX::XMMatrixTranslation
	(
		modelConstData[key][number].position.x,
		modelConstData[key][number].position.y,
		modelConstData[key][number].position.z
	);

	DirectX::XMMATRIX sumMatrix = matWorld * mainCamera->Get3DCameraMatrix(mainCameraData);

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			matrix[y][x] = sumMatrix.r[y].m128_f32[x];
		}
	}
}

void DirectX12::GetCameraMatrix(float matrix[4][4])
{
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			matrix[y][x] = mainCamera->Get3DCameraMatrix(mainCameraData).r[y].m128_f32[x];
		}
	}
}

void DirectX12::SetInputLayout(const std::vector<InputLayoutData>& inputLayoutData)
{
	CreatePipelineState->SetInputLayout(inputLayoutData);
}

void DirectX12::DeleteInputLayout()
{
	CreatePipelineState->DeleteInputLayout();
}


#pragma region ポストエフェクト
void DirectX12::SetPostEffectPipeline(const int& num)
{
	currentPostEffectPipeline = num;
}

bool DirectX12::CreateUserPostEffectPipelineState(const ShaderData& pShaderData)
{
	postEffectPipeline.resize(postEffectPipeline.size() + 1);

	////パイプライン
	std::vector<InputLayoutData>ilData(2);
	ilData[0] = { "POSITION", 3 ,FORMAT_TYPE_FLOAT};
	ilData[1] = { "TEXCOORD", 2 ,FORMAT_TYPE_FLOAT};
	CreatePipelineState->SetInputLayout(ilData);

	D3D12_GRAPHICS_PIPELINE_STATE_DESC pePLDesc = spriteGpipeline;
	pePLDesc.pRootSignature = postEffectRootSigneture.Get();

	bool result = CreatePipelineState->CreateUserPipeline
	(
		1,
		{ L"../MyLibrary/PostEffectVertexShader.hlsl","VSmain","vs_5_0" },
		{ L"NULL","","" },
		pShaderData,
		pePLDesc,
		&postEffectPipeline[postEffectPipeline.size() - 1],
		true
	);

	CreatePipelineState->DeleteInputLayout();

	postEfectConstBuffers.resize(postEfectConstBuffers.size() + 1);

	return result;
}
#pragma endregion

#pragma endregion

#pragma region アニメーション

//void DirectX12::setAnimation(int polyNum, int maxWidth, int maxHeight, int animationNumX, int animationNumY)
//{
//	//1つの四角形のサイズ
//	float oneWidth = 1.0f / (float)maxWidth;
//	float oneHeight = 1.0f / (float)maxHeight;
//
//
//	float width = 1.0f / (float)maxWidth *  (float)animationNumX;
//	float height = 1.0f / (float)maxHeight *  (float)animationNumY;
//
//	//範囲指定
//	vertices[polyNum][0][0].uv = { width - oneWidth,height };
//	vertices[polyNum][0][1].uv = { width - oneWidth,height - oneHeight };
//	vertices[polyNum][0][2].uv = { width,height };
//	vertices[polyNum][0][3].uv = { width , height - oneHeight };
//
//
//	//マップ
//	//なぜかマップできてない
//	//単色だとマップしても変わらない
//	//マップしてもポリゴンのサイズは変わりません
//	for (int i = 0; i < 4; i++)
//	{
//		vertexBufferSet[polyNum][0].vertexMap[i].uv = vertices[polyNum][0][i].uv;
//	}
//
//}
//
//void DirectX12::setAnimation2(int vertexNum, int dataNum, int startAreaX, int startAreaY, int endAreaX, int endAreaY)
//{
//	float textureWidth = (float)textureData[dataNum].width;
//	float textureHeight = (float)textureData[dataNum].height;
//
//	vertices[vertexNum][0][0].uv = { startAreaX / textureWidth ,endAreaY / textureHeight };
//	vertices[vertexNum][0][1].uv = { startAreaX / textureWidth ,startAreaX / textureHeight };
//	vertices[vertexNum][0][2].uv = { endAreaX / textureWidth ,endAreaY / textureHeight };
//	vertices[vertexNum][0][3].uv = { endAreaX / textureWidth ,startAreaY / textureHeight };
//
//
//	for (int i = 0; i < 4; i++)
//	{
//		vertexBufferSet[vertexNum][0].vertexMap[i].uv = vertices[vertexNum][0][i].uv;
//	}
//}

void DirectX12::SetSpriteAnimationVertex
(
	const int& spriteNum,
	const int& textureNum,
	const DirectX::XMFLOAT2& maxSize,
	const DirectX::XMFLOAT2& currentNumber
)
{
	if (textureNum != -1)
	{
		float texX = static_cast<float>(spriteTextureData[textureNum].width);
		float texY = static_cast<float>(spriteTextureData[textureNum].height);
		float vertWidth = texX / maxSize.x;
		float vertHeight = texY / maxSize.y;
		spriteVertices[spriteNum] = createPolygon->SetBoardPolygonVertex
		(
			{ -vertWidth / 2.0f,  vertHeight / 2.0f ,0.0f },
			{ -vertWidth / 2.0f , -vertHeight / 2.0f,0.0f },
			{ vertWidth / 2.0f,   vertHeight / 2.0f ,0.0f },
			{ vertWidth / 2.0f ,  -vertHeight / 2.0f,0.0f }
		);
	}

	//1つの四角形のサイズ(uv用)
	float oneWidth = 1.0f / maxSize.x;
	float oneHeight = 1.0f / maxSize.y;


	float width = 1.0f / maxSize.x * currentNumber.x;
	float height = 1.0f / maxSize.y *  currentNumber.y;

	//範囲指定
	spriteVertices[spriteNum][1].uv = { width - oneWidth,height - oneHeight };
	spriteVertices[spriteNum][0].uv = { width - oneWidth,height };
	spriteVertices[spriteNum][3].uv = { width , height - oneHeight };
	spriteVertices[spriteNum][2].uv = { width,height };


	//マップ
	for (int i = 0; i < 4; i++)
		spriteVertexBuffSet[spriteNum].vertexMap[i] = spriteVertices[spriteNum][i];
	
}

void DirectX12::SetSpriteAnimationVertex2
(
	int spriteNum, 
	int textureNum, 
	int posX, 
	int posY, 
	int areaWidth, 
	int areaHeight, 
	int startAreaX, 
	int startAreaY, 
	int endAreaX, 
	int endAreaY
)
{
	SpriteDataMap({ (float)posX,(float)posY }, { (float)areaWidth,(float)areaHeight }, spriteNum, 0);


	std::vector<Vertex>vertex;

	vertex = createPolygon->SetBoardPolygonVertex
	(
		{ (float)-areaWidth / 2,  (float)areaHeight / 2 ,0 },
		{ (float)-areaWidth / 2 ,(float)-areaHeight / 2,0 },
		{ (float)areaWidth / 2,  (float)areaHeight / 2 ,0 },
		{ (float)areaWidth / 2 ,(float)-areaHeight / 2,0 }
	);


	float texX = (float)spriteTextureData[textureNum].width;
	float texY = (float)spriteTextureData[textureNum].height;
	//endXYはendAreaの代わりにテクスチャのサイズを掛ける?
	float startX = 1.0f / (float)texX * (float)startAreaX;
	float startY = 1.0f / (float)texY * (float)startAreaY;
	float endX = 1.0f / (float)texX*(float)endAreaX;
	float endY = 1.0f / (float)texY*(float)endAreaY;


	vertex[0].uv = { startX ,endY };
	vertex[1].uv = { startX ,startY };
	vertex[2].uv = { endX ,endY };
	vertex[3].uv = { endX ,startY };
	spriteVertices[spriteNum] = vertex;

	auto vertexSize = vertex.size();
	for (int i = 0; i < vertexSize; i++)
	{
		spriteVertexBuffSet[spriteNum].vertexMap[i] = vertex[i];
	}
	
}

void DirectX12::SetSprite3DAnimation
(
    DirectX::XMFLOAT2 leftUpPosition,
	DirectX::XMFLOAT2 rightDownPosition,
	const int& spriteNumber,
	const int& textureNumber
)
{
	DirectX::XMFLOAT2 textureSize =
	{
		static_cast<float>(spriteTextureData[textureNumber].width),
		static_cast<float>(spriteTextureData[textureNumber].height) 
	};

	leftUpPosition.x = 1.0f / textureSize.x * leftUpPosition.x;
	leftUpPosition.y = 1.0f / textureSize.y * leftUpPosition.y;
	rightDownPosition.x = 1.0f / textureSize.x * rightDownPosition.x;
	rightDownPosition.y = 1.0f / textureSize.y * rightDownPosition.y;

	spriteVertices[spriteNumber][0].uv = { leftUpPosition.x,rightDownPosition.y };
	spriteVertices[spriteNumber][1].uv = { leftUpPosition.x,leftUpPosition.y };
	spriteVertices[spriteNumber][2].uv = { rightDownPosition.x,rightDownPosition.y };
	spriteVertices[spriteNumber][3].uv = { rightDownPosition.x,leftUpPosition.y };

	auto vertexSize = spriteVertices[spriteNumber].size();
	for (int i = 0; i < vertexSize; i++)
		spriteVertexBuffSet[spriteNumber].vertexMap[i].uv = spriteVertices[spriteNumber][i].uv;
}

#pragma endregion

#pragma region カメラ関数

void DirectX12::SetCameraDataMatrixPoint(Vector3 eye, Vector3 target, Vector3 up)
{
	mainCameraData.eye.x = eye.x;
	mainCameraData.eye.y = eye.y;
	mainCameraData.eye.z = eye.z;
	mainCameraData.target.x = target.x;
	mainCameraData.target.y = target.y;
	mainCameraData.target.z = target.z;
	mainCameraData.up.x = up.x;
	mainCameraData.up.y = up.y;
	mainCameraData.up.z = up.z;

	DirectInput::SetMatrixAndNearFar
	(
		mainCamera->GetViewMatrix(mainCameraData),
		mainCamera->GetProjectionMatrix(mainCameraData),
		mainCameraData.nearNumber,
		mainCameraData.farNumber
	);
}
void DirectX12::SetCameraData(Vector3 eye, Vector3 target, Vector3 up)
{
	mainCameraData.nowEye.x = eye.x;
	mainCameraData.nowEye.y = eye.y;
	mainCameraData.nowEye.z = eye.z;
	mainCameraData.nowTarget.x = target.x;
	mainCameraData.nowTarget.y = target.y;
	mainCameraData.nowTarget.z = target.z;
	mainCameraData.nowUp.x = up.x;
	mainCameraData.nowUp.y = up.y;
	mainCameraData.nowUp.z = up.z;

	DirectInput::SetMatrixAndNearFar
	(
		mainCamera->GetViewMatrix(mainCameraData),
		mainCamera->GetProjectionMatrix(mainCameraData),
		mainCameraData.nearNumber,
		mainCameraData.farNumber
	);

}


void DirectX12::SetCameraVelocity(DirectX::XMFLOAT3 eyeVelocity, DirectX::XMFLOAT3 targetVelocity)
{
	this->eyeVelocity = eyeVelocity;
	this->targetVelocity = targetVelocity;
}

void DirectX12::SetCameraAngre(DirectX::XMFLOAT3 eyeAngle, DirectX::XMFLOAT3 targetAngle, DirectX::XMFLOAT3 upAngle)
{
	mainCameraData.eyeAngle = eyeAngle;
	mainCameraData.targetAngle = targetAngle;
	mainCameraData.upAngle = upAngle;

	DirectInput::SetMatrixAndNearFar
	(
		mainCamera->GetViewMatrix(mainCameraData),
		mainCamera->GetProjectionMatrix(mainCameraData),
		mainCameraData.nearNumber,
		mainCameraData.farNumber
	);

	cameraMat = DirectX::XMMatrixIdentity();
	cameraMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(-mainCameraData.eyeAngle.z));
	cameraMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-mainCameraData.eyeAngle.x));
	cameraMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(-mainCameraData.eyeAngle.y));

	Model::SetCameraRotateMat(cameraMat);
}

void DirectX12::SetNearAndFar(float nearNum, float farNum)
{
	mainCameraData.nearNumber = nearNum;
	mainCameraData.farNumber = farNum;

	DirectInput::SetMatrixAndNearFar
	(
		mainCamera->GetViewMatrix(mainCameraData),
		mainCamera->GetProjectionMatrix(mainCameraData),
		mainCameraData.nearNumber,
		mainCameraData.farNumber
	);
}
#pragma endregion

#pragma region 見た目操作関数


#pragma region スムースシェーディング
void DirectX12::SetSmoothingFlag(bool flag)
{
	smoothingFlag = flag;
}
#pragma endregion



void DirectX12::SetIsPlane(bool flag)
{
	isPlane = flag;
}

void DirectX12::SetIsBillboard(bool x, bool y, bool z)
{
	isBillBoardX = x;
	isBillBoardY = y;
	isBillBoardZ = z;
}


void DirectX12::SetMulColor(Color color, const std::string& key, int number)
{

	modelConstData[key][number].mulColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 100.0f
	};



	/*int size = constBufferSet[despNum][number].constBuffer.size();
	for (int i = 0; i < size; i++) 
	{
		constBufferSet[despNum][number].constBuffer[i].Get()->Map(0, nullptr, (void**)&constData3D);
		constData3D->mulColor =
		{
			(float)color.r / (float)255,
			(float)color.g / (float)255,
			(float)color.b / (float)255,
			(float)color.a / (float)255
		};
		constBufferSet[despNum][number].constBuffer[i].Get()->Unmap(0, nullptr);
	}*/

}

void DirectX12::SetAddColor(Color color, const std::string& key, int number)
{
	modelConstData[key][number].addColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};

	/*int size = constBufferSet[despNum][number].constBuffer.size();
	for (int i = 0; i < size; i++)
	{
		constBufferSet[despNum][number].constBuffer[i].Get()->Map(0, nullptr, (void**)&constData3D);
		constData3D->addColor =
		{
			(float)color.r / (float)255,
			(float)color.g / (float)255,
			(float)color.b / (float)255,
			(float)color.a / (float)255
		};
		constBufferSet[despNum][number].constBuffer[i].Get()->Unmap(0, nullptr);
	}*/
}

void DirectX12::SetSubColor(Color color, const std::string& key, int number)
{
	modelConstData[key][number].subColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};

	/*int size = constBufferSet[despNum][number].constBuffer.size();
	for (int i = 0; i < size; i++)
	{
		constBufferSet[despNum][number].constBuffer[i].Get()->Map(0, nullptr, (void**)&constData3D);
		constData3D->subColor =
		{
			(float)color.r / (float)255,
			(float)color.g / (float)255,
			(float)color.b / (float)255,
			(float)color.a / (float)255
		};
		constBufferSet[despNum][number].constBuffer[i].Get()->Unmap(0, nullptr);
	}*/
}



void DirectX12::SetSpriteMulColor(Color color, int spriteNum)
{
	spriteConstBufferDatas[spriteNum]->mulColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};
}
void DirectX12::SetSpriteAddColor(Color color, int spriteNum)
{
	spriteConstBufferDatas[spriteNum]->addColor =
	{
		(float)color.r / 255.0f,
		(float)color.g / 255.0f,
		(float)color.b / 255.0f,
		(float)color.a / 255.0f
	};
}
void DirectX12::SetSpriteSubColor(Color color, int spriteNum)
{
	spriteConstBufferDatas[spriteNum]->subColor =
	{
	(float)color.r / 255.0f,
	(float)color.g / 255.0f,
	(float)color.b / 255.0f,
	(float)color.a / 255.0f
	};
}




void DirectX12::SetPointMulColor(Color color, int pointNum, int num)
{
	pointVertices[pointNum][num].color =
	{
	(float)color.r / 255.0f,
	(float)color.g / 255.0f,
	(float)color.b / 255.0f,
	(float)color.a / 255.0f
	};
}

#pragma endregion

#pragma region ライト

void DirectX12::SetLightVector(DirectX::XMFLOAT3 vector)
{
	lightVector = vector;
}

void DirectX12::SetLightColor(Color lightColor)
{
	this->lightColor =
	{
		(float)lightColor.r / (float)255,
		(float)lightColor.g / (float)255,
		(float)lightColor.b / (float)255,
	};
}

//void DirectX12::setAmbient(float ambient) 
//{
//	this->ambient = ambient;
//}
#pragma endregion


#pragma region バッファ作成

bool DirectX12::CreateConstBuffer
(
	ComPtr<ID3D12DescriptorHeap>&  heap,
	const int& heapHandleNum,
	ConstBufferSet* cBufferSet, 
	void** constData,
	const UINT& constDataSize
)
{
	if (!cBufferSet)return false;

	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle = 
		CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		heap.Get()->GetCPUDescriptorHandleForHeapStart(),
		heapHandleNum,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);


	createBuffer->CreateConstBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer((constDataSize + 0xff)&~0xff),
		basicHeapHandle,
		constData,
		*cBufferSet,
		0
	);
	return true;
}

#pragma region モデル読み込み

VertexType DirectX12::LoadObjVertex
(
	const char* path,
	bool loadUV,
	bool loadNormal,
	std::string* materialFireName,
	const std::string& key
)
{

	//objにあるモデル数
	int loadNum = 0;

	//マテリアル
	std::vector<std::string>materialName;

	std::vector<DirectX::XMFLOAT3>bonePos;
	std::vector<std::vector<int>>boneNum;

	//仮配列
	std::vector<std::vector<ObjAnimationVertex>>temporaryVertex;
	std::vector<std::vector<USHORT>>temporaryIndex;

	ModelLoader::GetInstance()->LoadObjModel
	(
		path,
		loadUV,
		loadNormal,
		temporaryVertex,
		temporaryIndex,
		*materialFireName,
		materialName,
		smoothData,
		&loadNum,
		&bonePos,
		&boneNum
	);

	

	std::vector<std::vector<Vertex>> normalVertices(temporaryVertex.size());
	auto size = temporaryVertex.size();
	for(int i = 0; i < size;i++)
	{
		auto size2 = temporaryVertex[i].size();
		normalVertices[i].resize(size2);
		for(int j = 0; j < size2;j++)
		{
			normalVertices[i][j].pos = temporaryVertex[i][j].pos;
			normalVertices[i][j].uv = temporaryVertex[i][j].uv;
			normalVertices[i][j].normal = temporaryVertex[i][j].normal;
		}
	}

	vertices.emplace(key, normalVertices);

	//ボーンあったら入れる
	bool loadBone = false;
	if (bonePos.size() != 0)
	{
		loadBone = true;
		objBoneNums.emplace(key, boneNum);
		objBonePositions.emplace(key, bonePos);
	}

	indices.emplace(key, temporaryIndex);
	

#pragma region テクスチャ反転

	int count = 0;
	float upx = 0;
	float upy = 0;
	float downx = 0;
	float downy = 0;
	for (int i = 0; i < vertices[key].size(); i++)
	{
		for (int j = 0; j < vertices[key][i].size(); j++)
		{
			//反転(ブレンダー、MAYAは2次元でも下が0で上に行くほど1に近づく)
			vertices[key][i][j].uv.y = (vertices[key][i][j].uv.y - 1) * -1;

			count++;
			count = count >= 4 ? 0 : count;

		}
	}
#pragma endregion



	//スムースシェーディング法線計算
	std::vector<std::vector<DirectX::XMFLOAT3>>normals;
	
	normals.resize(vertices[key].size());
	for (int i = 0; i < vertices[key].size(); i++)
	{
		normals[i].resize(vertices[key][i].size());
	}
	smoothNormal.emplace(key, normals);
	//calcSmoothingNormals(key);

#pragma region 頂点 インデックスバッファ作成

#pragma region 頂点バッファ

	std::vector<VertexBufferSet> vBuffs(loadNum);

	//バッファ作成
	if (!loadBone) //ボーンなし
	{
		for (int i = 0; i < loadNum; i++)
		{
			createBuffer->CreateVertexBufferSet
			(
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * vertices[key][i].size()),
				vertices[key][i],
				vBuffs[i]
			);
			vBuffs[i].materialName = materialName[i];
		}
	}
	else//ボーンあり
	{
		for (int i = 0; i < loadNum; i++)
		{
			size_t vertexSize = sizeof(ObjAnimationVertex) * vertices[key][i].size();
			auto result = dev->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(vertexSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&vBuffs[i].vertexBuffer)
			);

			ObjAnimationVertex* aniVertex;
			result = vBuffs[i].vertexBuffer.Get()->Map(0, nullptr, (void**)&aniVertex);
			
			size_t size = vertices[key][i].size();
			for (size_t j = 0; j < size; j++)
			{
				aniVertex[j].pos = vertices[key][i][j].pos;
				aniVertex[j].uv = vertices[key][i][j].uv;
				aniVertex[j].normal = vertices[key][i][j].normal;
				aniVertex[j].boneNumber = objBoneNums[key][i][j];
			}

			vBuffs[i].vertexBuffer->Unmap(0, nullptr);

			vBuffs[i].vertexBufferView.BufferLocation = vBuffs[i].vertexBuffer->GetGPUVirtualAddress();
			vBuffs[i].vertexBufferView.SizeInBytes = vertexSize;
			vBuffs[i].vertexBufferView.StrideInBytes = sizeof(ObjAnimationVertex);

			vBuffs[i].materialName = materialName[i];
		}
	}

	vertexBufferSet.emplace(key, vBuffs);
#pragma endregion

#pragma region インデックスバッファ
	std::vector<IndexBufferSet>iBuffs(loadNum);
	

	for (int i = 0; i < loadNum; i++)
	{
		createBuffer->CreateIndexBufferSet
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			CD3DX12_RESOURCE_DESC::Buffer(sizeof(USHORT) * indices[key][i].size()),
			indices[key][i],
			iBuffs[i]
		);
	}
	indexBufferSet.emplace(key,iBuffs);

#pragma endregion


#pragma endregion



	if (bonePos.size() != 0 && 
		boneNum.size() != 0)
		return VERTEX_TYPE_OBJ_ANIMATION;

	//ボーンなかったらこれ返す
	return VertexType::VERTEX_TYPE_NORMAL;
}

void DirectX12::LoadObjMaterial
(
	std::string materialDirectoryPath, 
	std::string materialFileName, 
	bool setConstDataFlag,
	const std::string& key,
	const int& objectNumber,
	const VertexType& vType
)
{
	ResizeObjectData(objectNumber,key);

	//テクスチャ名
	wchar_t texturePathW[20][256];

	//マテリアル読み込み&テクスチャ名取得
	int loadNum = 0;
	ModelLoader::GetInstance()->LoadObjMaterial
	(
		materialDirectoryPath,
		materialFileName,
		materials[key],
		&loadNum
	);

	for (int i = 0; i < loadNum; i++)
	{
		//mbsrtowcs_s(&size, texturePathW, 256, &texturePath, 256, nullptr);
		MultiByteToWideChar
		(
			CP_ACP, 
			0, 
			materials[key][i].textureName.c_str(), 
			-1, 
			texturePathW[i], 
			_countof(texturePathW[i])
		);
	}

	
#pragma region ヒープ作成

	//basicHeaps.resize(vertices.size());
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//descHeapDesc.NumDescriptors = heapData.objectNum * (1+loadNum)  + heapData.objectNum * loadNum;
	if (!setConstDataFlag)
	{
		descHeapDesc.NumDescriptors = (loadNum * objectNumber + loadNum) + objectNumber + 1;
	}
	else
	{
		descHeapDesc.NumDescriptors = (loadNum * objectNumber + loadNum) + objectNumber + objectNumber + 1;
	}

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	ComPtr<ID3D12DescriptorHeap>heap;
	basicHeaps.emplace(key, heap);
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicHeaps[key]));
	
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle;
	basicHeapHandle = basicHeaps[key]->GetCPUDescriptorHandleForHeapStart();

	
#pragma endregion

#pragma region テクスチャバッファ


	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchimage{};

	const DirectX::Image* imgs;
	TextureBufferSet tSet;
	textureBufferSet.emplace(key, tSet);
	textureBufferSet[key].textureBuffer.resize(loadNum);
	for (int i = 0; i < loadNum; i++)
	{
		basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
			i,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

		imgs = DirectXTexLoader::LoadTexture(texturePathW[i], &metadata, &scratchimage);
		//ここDrawBox書いてないときに処理すると下のやつでエラー出る
		//6/1 なぜかテクスチャが出なくなった
		//uvの設定を頂点バッファ生成後にしていた
		/*TextureBufferSet textureSet;
		textureBufferSet.push_back(textureSet);*/
		createBuffer->CreateTextureBufferSet
		(
			basicHeapHandle,
			metadata, 
			imgs, 
			textureBufferSet[key],
			i);

		heapTags[key].push_back(TEXTURE_BUFFER);
	}
	//一旦複数マテリアルのテクスチャを追加しない(今は最後に読み込んだ奴だけ追加)
	textureData.emplace(key,metadata);


#pragma endregion

#pragma region 定数バッファ

	//ボーン準備

	if(vType == VertexType::VERTEX_TYPE_OBJ_ANIMATION)
	{
		//ボーン用意
		std::vector<std::vector<BoneData>>boneDataVector;

		for (auto& b1 : boneDataVector) 
		{
			for(auto& b2 : b1)
			{
				b2.angle = {0,0,0};
				b2.moveVector = {0,0,0};
				b2.scale = {1,1,1};
			}
		}
		boneData.emplace(key, boneDataVector);


		ParentBoneData pBoneData;
		pBoneData.angleImpact = { 1.0f,1.0f,1.0f };
		pBoneData.scaleImpact = { 1.0f,1.0f,1.0f };
		pBoneData.moveVectorImpact = { 1.0f,1.0f,1.0f };
		pBoneData.parentBoneNum = -1;
		std::vector<ParentBoneData>pBoneDatas(objBonePositions[key].size(), pBoneData);
		parentBoneData.emplace(key, pBoneDatas);

		
	}

	CreateCommonBuffer(loadNum,key);

	//これいらんresizeにする
	ConstBufferSet constSet;

	//生成と移動
	std::vector<ConstBufferSet> constSetV;

	

	int createUserConstBuffCount = 0;
	for (int i = 0; i < objectNumber; i++)
	{
		constSetV.push_back(constSet);

		if (!setConstDataFlag)
		{
			constSetV[constSetV.size() - 1].constBuffer.resize(loadNum + 1);
		}
		else
		{
			constSetV[constSetV.size() - 1].constBuffer.resize(loadNum + 2);
		}

		

		basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
			loadNum + i + i * loadNum + createUserConstBuffCount + 1,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);


		createBuffer->CreateConstBufferSet
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(ModelConstBufferData) + 0xff)&~0xff),
			basicHeapHandle,
			(void**)&constData3D,
			constSetV[i],
			0
		);


		/*createConstBuffer
		(
			basicHeaps[key],
			loadNum + i + i * loadNum + createUserConstBuffCount + 1,
			&constSetV[i],
			(void**)&constData3D,
			sizeof(ConstBufferData)
		);*/
		if (!constData3D)return;

		constData3D->mulColor = { 1,1,1,1 };
		constData3D->addColor = { 0,0,0,0 };
		constData3D->subColor = { 0,0,0,0 };
		constData3D->ex = 0.0f;
		constData3D->mat = DirectX::XMMatrixIdentity();
		
		for (int i = 0; i < _countof(constData3D->boneMat); i++)
			constData3D->boneMat[i] = DirectX::XMMatrixIdentity();
		

		heapTags[key].push_back(LIBRARY_CONST_BUFFER);

		int num = 0;
		if (!setConstDataFlag)
		{
			//ここをマテリアル用に書き換える
			//一旦マテリアルのバッファをUserBuffの中に入れてる
			//constBuff、userBuffから、vectorに変える
			//ここに格納されてるのは何のバッファか、みたいなタグをつけれるようにする

			for (int j = 0; j < loadNum; j++)
			{
				num = loadNum + i + i * loadNum + j + 1 + createUserConstBuffCount + 1;
				basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
				(
					basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
					num,
					dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				);

				createBuffer->CreateConstBufferSet
				(
					CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					CD3DX12_RESOURCE_DESC::Buffer((sizeof(MaterialConstData) + 0xff)&~0xff),
					basicHeapHandle,
					(void**)&materialData,
					constSetV[i],
					j + 1
				);

				materialData->ambient = materials[key][j].ambient;
				materialData->diffuse = materials[key][j].diffuse;
				materialData->specular = materials[key][j].specular;
				materialData->alpha = materials[key][j].alpha;

				constSetV[i].constBuffer[j + 1].Get()->Unmap(0, nullptr);
				heapTags[key].push_back(MATERIAL_CONST_BUFFER);
			}
		}
		else
		{
			num = loadNum + i + i * loadNum + createUserConstBuffCount + 1;

			//ここでユーザー定数セット(オブジェクト数と同じ数作る)
			basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
				num,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

			createBuffer->CreateConstBufferSet
			(
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				CD3DX12_RESOURCE_DESC::Buffer((userConstDataSize3D + 0xff)&~0xff),
				basicHeapHandle,
				(void**)&userConstData3D,
				constSetV[i],
				1
			);

			heapTags[key].push_back(USER_CONST_BUFFER);



			//マテリアル読み込み
			for (int j = 0; j < loadNum; j++)
			{
				num = loadNum + i + i * loadNum + j + 1 + createUserConstBuffCount + 1;
				basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
				(
					basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
					num,
					dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				);

				createBuffer->CreateConstBufferSet
				(
					CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					CD3DX12_RESOURCE_DESC::Buffer((sizeof(MaterialConstData) + 0xff)&~0xff),
					basicHeapHandle,
					(void**)&materialData,
					constSetV[i],
					j + 1 + 1
				);

				materialData->ambient = materials[key][j].ambient;
				materialData->diffuse = materials[key][j].diffuse;
				materialData->specular = materials[key][j].specular;
				materialData->alpha = materials[key][j].alpha;

				constSetV[i].constBuffer[j + 1 + 1].Get()->Unmap(0, nullptr);
				heapTags[key].push_back(MATERIAL_CONST_BUFFER);
			}

			createUserConstBuffCount++;
		}

		constSetV[i].constBuffer[0].Get()->Unmap(0, nullptr);



	}
	constBufferSet.emplace(key,constSetV);
#pragma endregion





}

//void DirectX12::loadOBJ(const char* path, std::string materialDirectoryPath, bool loadUV, bool loadNormal, PolygonData vertData, HeapData heapData)
//{
//	//#pragma region 頂点_インデックスバッファ作成_マテリアル名取得
//	//
//	//	createPolyCounter++;
//	//
//	//	//モデルちゃんと読み込めなかったらコメントアウトしたところでやってみて
//	//	/*std::vector<Vertex>kariV;
//	//	std::vector<unsigned short>kariI;
//	//
//	//	ModelLoader::getInstance()->loadOBJModel(path, kariV, kariI);
//	//
//	//	vertices.push_back(kariV);
//	//	indices.push_back(kariI);*/
//	//
//	//	vertices.resize(vertices.size() + 1);
//	//	indices.resize(indices.size() + 1);
//	//
//	//	std::string materialName;
//	//	ModelLoader::getInstance()->loadOBJModel(path, loadUV, loadNormal, vertices[vertices.size() - 1], indices[indices.size() - 1], &materialName);
//	//
//	//
//	//#pragma region 頂点 インデックスバッファ作成
//	//
//	//#pragma region 頂点バッファ
//	//
//	//	VertexBufferSet vertSet;
//	//	vertexBuffSet.push_back(vertSet);
//	//	createBuffer->createVertexBufferSet
//	//	(
//	//		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//	//		CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * vertices[createPolyCounter - 1].size()),
//	//		vertices[createPolyCounter - 1],
//	//		vertexBuffSet[createPolyCounter - 1]
//	//	);
//	//
//	//
//	//#pragma endregion
//	//
//	//#pragma region インデックスバッファ
//	//	IndexBufferSet indexSet;
//	//	indexBufferSet.push_back(indexSet);
//	//
//	//	createBuffer->createIndexBufferSet(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), CD3DX12_RESOURCE_DESC::Buffer(sizeof(unsigned short) * indices[createPolyCounter - 1].size()), indices[createPolyCounter - 1], indexBufferSet[createPolyCounter - 1]);
//	//
//	//
//	//#pragma endregion
//	//
//	//
//	//#pragma endregion
//	//
//	//#pragma endregion
//	//
//	//#pragma region ヒープ
//	//
//	//	createDespCounter++;
//	//
//	//	resizeObjectData(heapData.objectNum);
//	//	
//	//	
//	//	//パスがobjからスタートするから、exeからobjのパスを入力できるようにする
//	//
//	//
//	//	//テクスチャ名
//	//	wchar_t texturePathW[256];
//	//	
//	//	//マテリアル読み込み&テクスチャ名取得
//	//	ModelLoader::getInstance()->loadObjMaterial(materialDirectoryPath, materialName, materials[materials.size() - 1][0]);
//	//	
//	//
//	//	//mbsrtowcs_s(&size, texturePathW, 256, &texturePath, 256, nullptr);
//	//	MultiByteToWideChar(CP_ACP, 0, materials[materials.size() - 1][0].textureName.c_str(), -1, texturePathW, _countof(texturePathW));
//	//
//	//
//	//
//	//#pragma region ヒープ作成
//	//
//	//	//basicHeaps.resize(vertices.size());
//	//	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
//	//	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
//	//
//	//	descHeapDesc.NumDescriptors = 1 + heapData.objectNum * 2;
//	//	
//	//
//	//	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
//	//	descHeapDesc.NodeMask = 0;
//	//
//	//	basicHeaps.resize(basicHeaps.size() + 1);
//	//	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicHeaps[createDespCounter - 1]));
//	//
//	//	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle;
//	//	basicHeapHandle = basicHeaps[createDespCounter - 1]->GetCPUDescriptorHandleForHeapStart();
//	//
//	//
//	//
//	//#pragma endregion
//	//
//	//#pragma region テクスチャバッファ
//	//
//	//	
//	//#pragma region テクスチャ読み込み
//	//
//	//		DirectX::TexMetadata metadata{};
//	//		DirectX::ScratchImage scratchimage{};
//	//
//	//		const DirectX::Image* imgs = DirectXTexLoader::loadTexture(texturePathW, &metadata, &scratchimage);
//	//
//	//		//ここDrawBox書いてないときに処理すると下のやつでエラー出る
//	//		//6/1 なぜかテクスチャが出なくなった
//	//		//uvの設定を頂点バッファ生成後にしていた
//	//		TextureBufferSet textureSet;
//	//		textureBufferSet.push_back(textureSet);
//	//		createBuffer->createTextureBufferSet(basicHeapHandle, metadata, imgs, textureBufferSet[createDespCounter - 1]);
//	//
//	//		textureData.push_back(metadata);
//	//
//	//#pragma endregion
//	//	
//	//
//	//#pragma endregion
//	//
//	//#pragma region 定数バッファ
//	//	//バッファを2個作る場合は、ハンドルのポインタをずらしてから生成する
//	//	//resizeObjectData(heapData.objectNum);
//	//
//	//	ConstBufferSet constSet;
//	//
//	//	//生成と移動
//	//	std::vector<ConstBufferSet> constSetV;
//	//	for (int i = 0; i < heapData.objectNum; i++)
//	//	{
//	//		constSetV.push_back(constSet);
//	//
//	//		
//	//			basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
//	//			(
//	//				basicHeaps[createDespCounter - 1]->GetCPUDescriptorHandleForHeapStart(),
//	//				i * 2 + 1,
//	//				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//	//			);
//	//
//	//			createBuffer->createConstBufferSet(
//	//				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//	//				CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff)&~0xff),
//	//				basicHeapHandle,
//	//				(void**)&constData3D,
//	//				constSetV[i],
//	//				false);
//	//
//	//			constData3D->mulColor = { 1,1,1,1 };
//	//			constData3D->addColor = { 0,0,0,0 };
//	//			constData3D->subColor = { 0,0,0,0 };
//	//			constData3D->ex = 0.0f;
//	//			constData3D->color = { 1,1,1,1 };
//	//			constData3D->light = { 1,-1,1 };
//	//			constData3D->mat = DirectX::XMMatrixIdentity();
//	//			constData3D->lightMat = DirectX::XMMatrixIdentity();
//	//
//	//
//	//			//ユーザー定数がおかしい
//	//			//通常とユーザー定数の生成順を入れ替えたが、ユーザー定数でエラー出た
//	//			//ルートパラメータは問題ないっぽい
//	//			//i * 2 + 2じゃダメ?
//	//			basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
//	//			(
//	//				basicHeaps[createDespCounter - 1]->GetCPUDescriptorHandleForHeapStart(),
//	//				i * 2 + 2,
//	//				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
//	//			);
//	//
//	//			//ここをマテリアル用に書き換える
//	//			//一旦マテリアルのバッファをUserBuffの中に入れてる
//	//			//constBuff、userBuffから、vectorに変える
//	//			//ここに格納されてるのは何のバッファか、みたいなタグをつけれるようにする
//	//			createBuffer->createConstBufferSet
//	//			(
//	//				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
//	//				CD3DX12_RESOURCE_DESC::Buffer((sizeof(MaterialConstBuffData) + 0xff)&~0xff),
//	//				basicHeapHandle,
//	//				(void**)&materialData,
//	//				constSetV[i],
//	//				true
//	//			);
//	//
//	//			materialData->ambient = materials[materials.size() - 1][0].ambient;
//	//			materialData->diffuse = materials[materials.size() - 1][0].diffuse;
//	//			materialData->specular = materials[materials.size() - 1][0].specular;
//	//			materialData->alpha = materials[materials.size() - 1][0].alpha;
//	//
//	//			constSetV[i].constBuffer.Get()->Unmap(0, nullptr);
//	//		
//	//
//	//
//	//	}
//	//	constBufferSet.push_back(constSetV);
//	//#pragma endregion
//	//
//	//	despDatas.push_back(heapData);
//	//
//	//
//	//#pragma endregion
//	//
//}
#pragma endregion

void DirectX12::CreatePolygonData
(
	const std::vector<Vertex>& vertex,
	const std::vector<USHORT>& index,
	const std::string& key
)
{
	std::vector<std::vector<Vertex>>temporaryVertices = { vertex };
	std::vector<std::vector<USHORT>>temporaryIndices = { index };
	

	vertices.emplace(key,temporaryVertices);
	indices.emplace(key, temporaryIndices);
	

#pragma region 法線ベクトル計算

	//行列で動かした後に法線ベクトルを書き換えないといけない?
	//頂点多いcreateで作らないとちゃんと法線を求められない

	for (int j = 0; j < (int)indices[key][0].size() / 3; j++)
	{
		CalculationNormal
		(
			vertices[key][0][indices[key][0][j * 3 + 0]].pos,
			vertices[key][0][indices[key][0][j * 3 + 1]].pos,
			vertices[key][0][indices[key][0][j * 3 + 2]].pos,
			vertices[key][0][indices[key][0][j * 3 + 0]].normal,
			vertices[key][0][indices[key][0][j * 3 + 1]].normal,
			vertices[key][0][indices[key][0][j * 3 + 2]].normal
		);

	}

	//smoothNOrmalにverticesの法線を移す
	//スムースシェーディング法線計算
	std::vector<std::vector<DirectX::XMFLOAT3>>normals;

	normals.resize(vertices[key].size());
	for (int i = 0; i < vertices[key].size(); i++)
	{
		normals[i].resize(vertices[key][i].size());
	}
	

	//ライブラリ作成オブジェクトはスムースシェーディングしないため、同じのを入れる
	for (int i = 0; i < vertices[key].size(); i++)
	{
		for (int j = 0; j < vertices[key][i].size(); j++)
		{
			normals[i][j] = vertices[key][i][j].normal;
		}
	}
	smoothNormal.emplace(key, normals);
#pragma endregion


#pragma region 頂点 インデックスバッファ作成

#pragma region 頂点バッファ

	std::vector<VertexBufferSet> vSet(1);

	createBuffer->CreateVertexBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * vertices[key][0].size()),
		vertices[key][0],
		vSet[0]
	);

	vertexBufferSet.emplace(key,vSet);
#pragma endregion

#pragma region インデックスバッファ
	std::vector<IndexBufferSet>iSet(1);
	createBuffer->CreateIndexBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(USHORT) * indices[key][0].size()),
		indices[key][0],
		iSet[0]
	);

	indexBufferSet.emplace(key,iSet);

#pragma endregion


#pragma endregion

}

#pragma region ヒープ

void DirectX12::ResizeObjectData(int objectNum,const std::string& key)
{
	std::vector<ModelConstData> objConstData;
	objConstData.resize(objectNum);
	

	modelConstData.emplace(key, objConstData);

	std::vector<Material>m(1);
	materials.emplace(key, m);
	materials[key].resize(1);
	materials[key][0].materialName = "NULL";
	
	std::vector<std::string>parentHeapVector(objectNum, "NONE");
	std::vector<int>parentNumberVector(objectNum, -1);
	parentHeaps.emplace(key, parentHeapVector);
	parentNums.emplace(key, parentNumberVector);

	std::vector<ConstBufferTag>tags;
	heapTags.emplace(key, tags);
}


void DirectX12::CreateHeapData
(
	bool setConstDataFlag, 
	const std::string& key, 
	const int& objectNumber,
	const wchar_t* texPath,
	const Color* color
)
{

	if (!color && texPath == L"")return;

	//バッファを2個作る場合は、ハンドルのポインタをずらしてから生成する
	ResizeObjectData(objectNumber,key);

	

#pragma region ヒープ作成

	//basicHeaps.resize(vertices.size());
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	if (!setConstDataFlag)
	{
		descHeapDesc.NumDescriptors = 1 + objectNumber * 2 + 1;//	テクスチャ + obj数 * 2(addColorなどのバッファとマテリアル) +共通(common)
	}
	else
	{
		descHeapDesc.NumDescriptors = 1 + objectNumber * 3 + 1;
	}

	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NodeMask = 0;

	ComPtr<ID3D12DescriptorHeap> heap;
	basicHeaps.emplace(key, heap);
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicHeaps[key]));
	

	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle;
	basicHeapHandle = basicHeaps[key]->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region テクスチャバッファ

	TextureBufferSet tSet;
	tSet.textureBuffer.resize(1);
	//単色
	if (texPath == L"")
	{
		createBuffer->CreateTextureBufferSet2(basicHeapHandle, *color, tSet, 0);
		textureBufferSet.emplace(key, tSet);

		//空データを入れる
		DirectX::TexMetadata metadata;
		textureData.emplace(key,metadata);

		heapTags[key].push_back(TEXTURE_BUFFER);
	}
	else
	{
		DirectX::TexMetadata metadata{};
		DirectX::ScratchImage scratchimage{};

		const DirectX::Image* imgs = DirectXTexLoader::LoadTexture(texPath, &metadata, &scratchimage);

		createBuffer->CreateTextureBufferSet(basicHeapHandle, metadata, imgs, tSet, 0);
		textureBufferSet.emplace(key, tSet);
		textureData.emplace(key,metadata);

		heapTags[key].push_back(TEXTURE_BUFFER);

	}

#pragma endregion

#pragma region 定数バッファ
	CreateCommonBuffer(1, key);

	ConstBufferSet cSet;

	//オブジェクト分
	std::vector<ConstBufferSet> constSetV;

	//マテリアルバッファ作成ラムダ式
	auto createMaterialBuffer = [&](const int& handleNum,const int& objectNum,const bool& setUserConstData)
	{
		int num = handleNum;
		basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
			num,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);

		int arrayNum = 0;
		if (!setUserConstData)
			arrayNum = 1;
		else 
			arrayNum = 2;

		createBuffer->CreateConstBufferSet
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			CD3DX12_RESOURCE_DESC::Buffer((sizeof(MaterialConstData) + 0xff)&~0xff),
			basicHeapHandle,
			(void**)&materialData,
			constSetV[objectNum],
			arrayNum
		);

		materialData->ambient = materials[key][0].ambient;
		materialData->diffuse = materials[key][0].diffuse;
		materialData->specular = materials[key][0].specular;
		materialData->alpha = materials[key][0].alpha;

		constSetV[objectNum].constBuffer[arrayNum].Get()->Unmap(0, nullptr);
		heapTags[key].push_back(MATERIAL_CONST_BUFFER);
	};

	
	for (int i = 0; i < objectNumber; i++)
	{
		constSetV.push_back(cSet);
		if (!setConstDataFlag)
		{

			constSetV[i].constBuffer.resize(2);
			

			basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
				i * 2 + 1 + 1, //オブジェクト数 + テクスチャ数 + 1(1つ目の定数バッファ)
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

			createBuffer->CreateConstBufferSet(
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				CD3DX12_RESOURCE_DESC::Buffer((sizeof(ModelConstBufferData) + 0xff)&~0xff),
				basicHeapHandle,
				(void**)&constData3D,
				constSetV[i],
				0);

			/*createConstBuffer
			(
				basicHeaps[key],
				i + 1 + 1,
				&constSetV[i],
				(void**)&constData3D,
				sizeof(ConstBufferData)
			);*/

			constData3D->mulColor = { 1,1,1,1 };
			constData3D->addColor = { 0,0,0,0 };
			constData3D->subColor = { 0,0,0,0 };
			constData3D->ex = 0.0f;
			constData3D->mat = DirectX::XMMatrixIdentity();

			constSetV[i].constBuffer[0].Get()->Unmap(0, nullptr);

			heapTags[key].push_back(LIBRARY_CONST_BUFFER);

			//マテリアルバッファ作成
			createMaterialBuffer(i * 2 + 2 + 1, i, setConstDataFlag);

			int z = 0;
		}
		else
		{
			constSetV[i].constBuffer.resize(3);
			basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
				i * 3 + 1 + 1,//オブジェクト数 * 1つのオブジェクトに必要なバッファ数 + テクスチャ数 + 1(1つ目のバッファ)
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

			createBuffer->CreateConstBufferSet(
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				CD3DX12_RESOURCE_DESC::Buffer((sizeof(ModelConstBufferData) + 0xff)&~0xff),
				basicHeapHandle,
				(void**)&constData3D,
				constSetV[i],
				0);

			constData3D->mulColor = { 1,1,1,1 };
			constData3D->addColor = { 0,0,0,0 };
			constData3D->subColor = { 0,0,0,0 };
			constData3D->ex = 0.0f;
			constData3D->mat = DirectX::XMMatrixIdentity();

			heapTags[key].push_back(LIBRARY_CONST_BUFFER);

			//ユーザー定数がおかしい
			//通常とユーザー定数の生成順を入れ替えたが、ユーザー定数でエラー出た
			//ルートパラメータは問題ないっぽい
			//i * 2 + 2じゃダメ?
			basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
				i * 3 + 2 + 1,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

			//void**はsizeofで読み取れないから構造体のサイズを送ってもらう
			createBuffer->CreateConstBufferSet
			(
				CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				CD3DX12_RESOURCE_DESC::Buffer((userConstDataSize3D + 0xff)&~0xff),
				basicHeapHandle,
				userConstData3D,
				constSetV[i],
				1
			);

			constSetV[i].constBuffer[0].Get()->Unmap(0, nullptr);
			heapTags[key].push_back(USER_CONST_BUFFER);

			//マテリアルバッファ作成
			createMaterialBuffer(i * 3 + 3 + 1, i, setConstDataFlag);
		}


	}
	constBufferSet.emplace(key,constSetV);

#pragma endregion


}

void DirectX12::CreateCommonBuffer(const int& texNum, const std::string& key)
{
	auto basicHeapHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE
	(
		basicHeaps[key]->GetCPUDescriptorHandleForHeapStart(),
		texNum,
		dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
	);
	
	ComPtr<ID3D12Resource>commonBuffer;
	commonBuffers.emplace(key, commonBuffer);

	auto result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CommonConstData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&commonBuffers[key])
	);


	//Map
	result = commonBuffers[key]->Map(0, nullptr, (void**)&commonConstData3D);

	commonConstData3D->lightColor = { 1,1,1 ,1 };
	commonConstData3D->light = { 1,1,1,1 };
	commonConstData3D->lightMat = DirectX::XMMatrixIdentity();
	commonConstData3D->cameraPos = { mainCameraData.nowEye.x,mainCameraData.nowEye.y,mainCameraData.nowEye.z,1 };
	
	commonBuffers[key]->Unmap(0, nullptr);


	//ビュー
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;

	cbvDesc.BufferLocation = commonBuffers[key]->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)commonBuffers[key]->GetDesc().Width;
	dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

	heapTags[key].push_back(COMMON_CONST_BUFFER);
}

#pragma endregion


void DirectX12::LoadSpriteFont(const wchar_t *const texturePath, const DirectX::XMFLOAT2& lineNum)
{
	//10種類まで読み込める

	DirectX::TexMetadata metadata{};
	DirectX::ScratchImage scratchimage{};
	const DirectX::Image* imgs = DirectXTexLoader::LoadTexture(texturePath, &metadata, &scratchimage);

	spriteFontTextureBufferSet.resize(spriteFontTextureBufferSet.size() + 1);
	spriteFontTextureBufferSet[spriteFontTextureBufferSet.size() - 1].textureBuffer.resize(1);

	D3D12_CPU_DESCRIPTOR_HANDLE spriteHandle = spriteFontHeap->GetCPUDescriptorHandleForHeapStart();

	createBuffer->CreateTextureBufferSet
	(
		CD3DX12_CPU_DESCRIPTOR_HANDLE
		(
			spriteHandle,
			spriteFontTextureBufferSet.size() - 1,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		),
		metadata,
		imgs,
		spriteFontTextureBufferSet[spriteFontTextureBufferSet.size() - 1],
		0
	);

	SpriteFontData data;
	data.widthLineNum = (int)lineNum.x;
	data.heightLineNum = (int)lineNum.y;
	spriteFontData.push_back(data);

}

void DirectX12::LoadTexture(const wchar_t* texturePath, Color color)
{
	loadTextureCounter++;

	//L""のやつ消す
	if (texturePath == L"")
	{
		/*DirectX::TexMetadata metadata{};

		TextureBufferSet textureSet;
		spriteTextureBufferSet.push_back(textureSet);

		D3D12_CPU_DESCRIPTOR_HANDLE spriteHandle = spriteHeap->GetCPUDescriptorHandleForHeapStart();

		createBuffer->createTextureBufferSet2
		(
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				spriteHandle,
				loadTextureCounter - 1,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			), color,
			spriteTextureBufferSet[loadTextureCounter - 1]
		);

		textureData.push_back(metadata);*/
	}
	else
	{
		DirectX::TexMetadata metadata{};
		DirectX::ScratchImage scratchimage{};
		const DirectX::Image* imgs = DirectXTexLoader::LoadTexture(texturePath, &metadata, &scratchimage);

		//ここDrawBox書いてないときに処理すると下のやつでエラー出る
		//6/1 なぜかテクスチャが出なくなった
		//uvの設定を頂点バッファ生成後にしていた
		TextureBufferSet textureSet;
		spriteTextureBufferSet.push_back(textureSet);
		spriteTextureBufferSet[loadTextureCounter - 1].textureBuffer.resize(1);

		D3D12_CPU_DESCRIPTOR_HANDLE spriteHandle = spriteHeap->GetCPUDescriptorHandleForHeapStart();


		createBuffer->CreateTextureBufferSet
		(
			CD3DX12_CPU_DESCRIPTOR_HANDLE
			(
				spriteHandle,
				loadTextureCounter - 1,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			),
			metadata,
			imgs,
			spriteTextureBufferSet[loadTextureCounter - 1],
			0
		);

		spriteTextureData.push_back(metadata);
	}


}

void DirectX12::CreateSprite(int* sprite, bool font)
{

	if (!font)spriteP.push_back(sprite);

	createSpriteCounter++;
	//spriteSizes.push_back(size);



#pragma region 頂点バッファ
	spriteVertices.resize(createSpriteCounter);
	spriteVertices[createSpriteCounter - 1].resize(4);

	VertexBufferSet vertSet;
	spriteVertexBuffSet.push_back(vertSet);
	createBuffer->CreateVertexBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(Vertex) * spriteVertices[createSpriteCounter - 1].size()),
		spriteVertices[createSpriteCounter - 1],
		spriteVertexBuffSet[createSpriteCounter - 1]
	);




#pragma endregion

#pragma region インデックスバッファ
	spriteIndices.resize(createSpriteCounter);
	spriteIndices[createSpriteCounter - 1].resize(6);


	IndexBufferSet indexSet;
	spriteIndexBufferSet.push_back(indexSet);

	createBuffer->CreateIndexBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(unsigned short) * spriteIndices[createSpriteCounter - 1].size()),
		spriteIndices[createSpriteCounter - 1],
		spriteIndexBufferSet[createSpriteCounter - 1]
	);


#pragma endregion

#pragma region 定数バッファ
	ConstBufferSet constSet;
	spriteConstBufferSet.push_back(constSet);


	//ConstBufferData* constData;
	spriteConstBufferDatas.resize(createSpriteCounter);

	spriteConstBufferSet[createSpriteCounter - 1].constBuffer.resize(1);
	result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ModelConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBufferSet[createSpriteCounter - 1].constBuffer[0])
	);

	spriteConstBufferSet[createSpriteCounter - 1].constBuffer[0]->Map(0, nullptr, (void**)&spriteConstBufferDatas[createSpriteCounter - 1]);

	spriteConstBufferDatas[createSpriteCounter - 1]->mulColor = { 1,1,1,1 };
	spriteConstBufferDatas[createSpriteCounter - 1]->addColor = { 0,0,0,0 };
	spriteConstBufferDatas[createSpriteCounter - 1]->subColor = { 0,0,0,0 };
	spriteConstBufferDatas[createSpriteCounter - 1]->mat = DirectX::XMMatrixIdentity();
#pragma endregion

	//DirectX::XMVECTOR kara;
	//kara = { 0, 0, 0,0 };
	//spritePosition.push_back(kara);


	/*spritePosition.resize(spritePosition.size() + 1);
	spritePosition[spritePosition.size() - 1] = { 0,0,0,0 };*/

	spriteAngle.resize(spriteAngle.size() + 1, {0.0f,0.0f,0.0f});

	spriteScale.resize(spriteScale.size() + 1, { 1,1 });


}

void DirectX12::CreatePoint(int createNum, int* point)
{
	pointP.push_back(point);


#pragma region 頂点バッファ
	pointVertexBuffSet.resize(pointVertexBuffSet.size() + 1);
	pointVertexBuffSet[pointVertexBuffSet.size() - 1].resize(createNum);
	pointVertices.resize(pointVertices.size() + 1);

	for (int i = 0; i < createNum; i++)
	{
		dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(PointVertex) * 4),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&pointVertexBuffSet[pointVertexBuffSet.size() - 1][i].vertexBuffer)
		);

		PointVertex pointVertex;
		pointVertex.pos = { 0,0,0 };
		pointVertex.scale = { 1,1 };
		pointVertex.color = { 1,1,1,1 };
		//pointVertex.uv = 
		pointVertices[pointVertices.size() - 1].push_back(pointVertex);
		

		pointVertexBuffSet[pointVertexBuffSet.size() - 1][i].vertexBuffer->Map(0, nullptr, (void**)&pointVertexMapData);
		pointVertexMapData->pos = pointVertices[pointVertices.size() - 1][i].pos;
		pointVertexMapData->scale = pointVertices[pointVertices.size() - 1][i].scale;
		pointVertexMapData->color = pointVertices[pointVertices.size() - 1][i].color;
		pointVertexBuffSet[pointVertexBuffSet.size() - 1][i].vertexBuffer->Unmap(0, nullptr);



		pointVertexBuffSet[pointVertexBuffSet.size() - 1][i].vertexBufferView.BufferLocation = pointVertexBuffSet[pointVertexBuffSet.size() - 1][i].vertexBuffer->GetGPUVirtualAddress();
		pointVertexBuffSet[pointVertexBuffSet.size() - 1][i].vertexBufferView.SizeInBytes = static_cast<UINT>( sizeof(PointVertex) * pointVertices.size());
		pointVertexBuffSet[pointVertexBuffSet.size() - 1][i].vertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(PointVertex));
	}

#pragma endregion

#pragma region 定数バッファ
	ConstBufferSet constSet;
	//pointConstBufferSet.push_back(constSet);


	//ConstBufferData* constData;
	pointConstBufferSet.resize(pointConstBufferSet.size() + 1);
	pointConstBufferSet[pointConstBufferSet.size() - 1].constBuffer.resize(1);

	result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(PointConstBufferData) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&pointConstBufferSet[pointConstBufferSet.size() - 1].constBuffer[0])
	);

	pointConstBufferSet[pointConstBufferSet.size() - 1].constBuffer[0]->Map(0, nullptr, (void**)&constDataPoint);

	constDataPoint->mat = DirectX::XMMatrixIdentity();
	constDataPoint->billboardMat = DirectX::XMMatrixIdentity();
	
	pointConstBufferSet[pointConstBufferSet.size() - 1].constBuffer[0]->Unmap(0, nullptr);
#pragma endregion



}

#pragma region ユーザー
void DirectX12::AddUserVertex
(
	std::vector<Vector3>& vertexPos, 
	std::vector<Vector2>& vertexUV,
	const std::string& key
)
{
	std::vector<std::vector<Vertex>>xmFloat3Vertex(1);
	xmFloat3Vertex[0].resize(1);
	//サイズが違かったらリターン
	if (vertexPos.size() != vertexUV.size()) return;

	//座標、UVの代入
	for (UINT i = 0; i < vertexPos.size(); i++)
	{
		xmFloat3Vertex[0][i].pos.x = vertexPos[i].x;
		xmFloat3Vertex[0][i].pos.y = vertexPos[i].y;
		xmFloat3Vertex[0][i].pos.z = vertexPos[i].z;

		xmFloat3Vertex[0][i].uv.x = vertexUV[i].x;
		xmFloat3Vertex[0][i].uv.y = vertexUV[i].y;
	}

	//追加
	vertices.emplace(key, xmFloat3Vertex);
}

void DirectX12::AddUserIndex(std::vector<unsigned short>& index, const std::string& key)
{
	std::vector<std::vector<USHORT>>ind;
	ind.push_back(index);
	indices.emplace(key, ind);
}

void DirectX12::CreateUserPolygon
(
	void** vertexData,
	UINT vertexDataSize,
	UINT vertexSumDataSize,
	std::vector<USHORT>&index,
	const std::string& key
)
{
	
#pragma region 頂点バッファ
	std::vector<VertexBufferSet>vBuffs(1);

	createBuffer->CreateUserVertexBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer(vertexSumDataSize),
		vertexData,
		vertexDataSize,
		vertexSumDataSize,
		vBuffs[0]
	);
	vertexBufferSet.emplace(key, vBuffs);

#pragma endregion

#pragma region インデックスバッファ
	std::vector<IndexBufferSet>iBuffs(1);
	

	createBuffer->CreateIndexBufferSet
	(
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		CD3DX12_RESOURCE_DESC::Buffer(sizeof(unsigned short) * indices[key].size()),
		indices[key][0],
		iBuffs[0]
	);
	indexBufferSet.emplace(key, iBuffs);

#pragma endregion


}

#pragma endregion


#pragma endregion

#pragma region 削除関数

void DirectX12::DeletePolygonData(const ModelData& m)
{
	std::string key = m.key;
	vertices.erase(key);
	indices.erase(key);
	smoothNormal.erase(key);

	vertexBufferSet.erase(key);
	indexBufferSet.erase(key);

	if(m.type == VertexType::VERTEX_TYPE_OBJ_ANIMATION)
	{
		boneData.erase(key);
		objBoneNums.erase(key);
		objBonePositions.erase(key);
		parentBoneData.erase(key);
	}

}

void DirectX12::DeleteHeapData(const ModelData& m)
{

	std::string key = m.key;
	textureBufferSet.erase(key);
	constBufferSet.erase(key);
	basicHeaps.erase(key);
	textureData.erase(key);

	modelConstData.erase(key);

	materials.erase(key);

	parentHeaps.erase(key);
	parentNums.erase(key);

	commonBuffers.erase(key);

	heapTags.erase(key);


}


void DirectX12::DeleteSprite(int sprite)
{
	//スプライトフォント分引く
	sprite -= SPRITEFONT_MAX;
	delete spriteP[sprite];
	spriteP.erase(spriteP.begin() + sprite);
	auto sprSize = spriteP.size();
	for (size_t i = sprite; i < sprSize; i++)
	{
		*spriteP[i]--;
	}
	spriteP.shrink_to_fit();

	spriteVertices.erase(spriteVertices.begin() + sprite);
	spriteIndices.erase(spriteIndices.begin() + sprite);
	spriteVertexBuffSet.erase(spriteVertexBuffSet.begin() + sprite);
	spriteIndexBufferSet.erase(spriteIndexBufferSet.begin() + sprite);
	spriteConstBufferSet.erase(spriteConstBufferSet.begin() + sprite);
	spriteConstBufferDatas.erase(spriteConstBufferDatas.begin() + sprite);
	spriteAngle.erase(spriteAngle.begin() + sprite);
	spriteScale.erase(spriteScale.begin() + sprite);

	spriteVertices.shrink_to_fit();
	spriteIndices.shrink_to_fit();
	spriteVertexBuffSet.shrink_to_fit();
	spriteIndexBufferSet.shrink_to_fit();
	spriteConstBufferSet.shrink_to_fit();
	spriteConstBufferDatas.shrink_to_fit();
	spriteAngle.shrink_to_fit();
	spriteScale.shrink_to_fit();

	createSpriteCounter--;
}
#pragma endregion

#pragma region 描画関数

void DirectX12::CalcBillboardMat(DirectX::XMMATRIX& matWorld)
{//ビルボード行列作成
//通常の板ポリのスケールを変えた時にビルボードするとおかしくなる
	if (isBillBoardX || isBillBoardY || isBillBoardZ)
	{
		//回転させた座標の取得

		DirectX::XMFLOAT3 cPos;
		DirectX::XMFLOAT3 cTarget;
		mainCamera->Get3DCameraPosition(mainCameraData, cPos, cTarget);

		DirectX::XMVECTOR vCPos = DirectX::XMLoadFloat3(&cPos);
		DirectX::XMVECTOR vCTarget = DirectX::XMLoadFloat3(&cTarget);


		DirectX::XMVECTOR upVector = DirectX::XMLoadFloat3(&mainCameraData.nowUp);
		//Z軸を求める
		DirectX::XMVECTOR cameraAxisZ = DirectX::XMVectorSubtract(vCTarget, vCPos);

		//除外
		assert(!DirectX::XMVector3Equal(cameraAxisZ, DirectX::XMVectorZero()));
		assert(!DirectX::XMVector3IsInfinite(cameraAxisZ));
		assert(!DirectX::XMVector3Equal(upVector, DirectX::XMVectorZero()));
		assert(!DirectX::XMVector3IsInfinite(upVector));

		//ベクトルを正規化
		cameraAxisZ = DirectX::XMVector3Normalize(cameraAxisZ);

		//X軸を求める
		DirectX::XMVECTOR cameraAxisX = DirectX::XMVector3Cross(upVector, cameraAxisZ);

		//正規化
		cameraAxisX = DirectX::XMVector3Normalize(cameraAxisX);

		//Y軸を求める
		DirectX::XMVECTOR cameraAxisY = DirectX::XMVector3Cross(cameraAxisZ, cameraAxisX);

		//全方位ビルボード行列の計算
		DirectX::XMMATRIX billboardMatrix = DirectX::XMMatrixIdentity();
		if (isBillBoardX)billboardMatrix.r[0] = cameraAxisX;
		if (isBillBoardY)billboardMatrix.r[1] = cameraAxisY;
		if (isBillBoardZ)billboardMatrix.r[2] = cameraAxisZ;
		billboardMatrix.r[3] = DirectX::XMVectorSet(0, 0, 0, 1);

		matWorld *= billboardMatrix;
	}
}

//バッファをセット
void DirectX12::SetCmdList(const ModelData& modelData,  int number)
{
	//if ( despNum >= 0 && number >= 0)
	{
		//パイプライン誤セット防止
		int pNum = pipelineNum;
		if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION &&
			pipelineNum == PIPELINE_OBJ_ANIMATION)
		{
			pNum = PIPELINE_NORMAL;
		}

		cmdList->SetGraphicsRootSignature(rootSignature.Get());
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		cmdList->SetPipelineState(pipelineStates[pNum].Get());

		std::vector<ID3D12DescriptorHeap*> ppHeaps;

		//ハンドルvectorやめてfor内に毎回宣言したほうがいい?そうすればずらしたやつを勝手に直してくれる
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

		//2D
		//if (polyDatas[key].dimention == Dimension::dimention2D)//2D
		//{

			//	cmdList->IASetVertexBuffers(0, 1, &vertexBuffSet[polyNum].vertexBufferView);
			//	cmdList->IASetIndexBuffer(&indexBufferSet[polyNum].indexBufferView);

			//	cmdList->SetGraphicsRootSignature(spriteRootsignature.Get());
			//	cmdList->SetPipelineState(spritePipelinestate.Get());

			//	ppHeaps.push_back(basicHeaps[despNum].Get());
			//	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

			//	gpuDescHandle = basicHeaps[despNum]->GetGPUDescriptorHandleForHeapStart();
			//	//テクスチャ
			//	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

			//	//ヒープ内の定数バッファ指定

			//	for (int j = 0; j < number + 1; j++)
			//	{
			//		//目的の定数バッファまでずらす
			//		gpuDescHandle.ptr += dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//	}
			//	//定数セット
			//	cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

			//	cmdList->DrawIndexedInstanced(indices[polyNum].size(), 1, 0, 0, 0);
	//	}

		//3D
		//if (polyDatas[key].dimention == Dimension::dimention3D)//3D
		{

			ppHeaps.push_back(basicHeaps[modelData.key].Get());
			cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

			gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
			(
				basicHeaps[modelData.key]->GetGPUDescriptorHandleForHeapStart(),
				0,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);

			int handleNum = 0;//どのくらいハンドルをずらすか決めるための番号


			//共通
			handleNum = static_cast<int>(textureBufferSet[modelData.key].textureBuffer.size());
			gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
			(
				basicHeaps[modelData.key]->GetGPUDescriptorHandleForHeapStart(),
				handleNum,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
			);
			cmdList->SetGraphicsRootDescriptorTable(4, gpuDescHandle);

			//頂点バッファ分ループ
			for (int i = 0; i < vertexBufferSet[modelData.key].size(); i++)
			{
				handleNum = 0;

				//マテリアル名がobjに書かれてるのにmtl読まなかった場合、最初のテクスチャが選ばれる
				for (int j = 0; j < materials[modelData.key].size(); j++)
				{
					//マテリアル紐づけ
					if (vertexBufferSet[modelData.key][i].materialName == materials[modelData.key][j].materialName)handleNum = j;
				}

				cmdList->IASetIndexBuffer(&indexBufferSet[modelData.key][i].indexBufferView);
				cmdList->IASetVertexBuffers(0, 1, &vertexBufferSet[modelData.key][i].vertexBufferView);

				//テクスチャ
				gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
				(
					basicHeaps[modelData.key]->GetGPUDescriptorHandleForHeapStart(),
					handleNum,
					dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				);
				cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);

				//定数バッファセット
				handleNum = 0;
				handleNum += static_cast<int>(textureBufferSet[modelData.key].textureBuffer.size()) + 1;//テクスチャと共通分ずらす
				handleNum += static_cast<int>(constBufferSet[modelData.key][number].constBuffer.size()) * number;//オブジェクトの場所までずらす

				gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
				(
					basicHeaps[modelData.key]->GetGPUDescriptorHandleForHeapStart(),
					handleNum,
					dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
				);
				cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

				//ユーザー定数セット

				//要素数を超えてアクセスしないようにするためのif
				if (heapTags[modelData.key].size() > handleNum + 1)
				{
					//ユーザー定数があったら、セットする
					if (heapTags[modelData.key][handleNum + 1] == USER_CONST_BUFFER)
					{
						handleNum++;
						gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
						(
							basicHeaps[modelData.key]->GetGPUDescriptorHandleForHeapStart(),
							handleNum,
							dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
						);
						cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandle);
					}
				}

				//マテリアルセット
				if (heapTags[modelData.key].size() > handleNum + 1)
				{
					//マテリアルがあったら、テクスチャ分ずらして、マテリアル定数をセット
					if (heapTags[modelData.key][handleNum + 1] == MATERIAL_CONST_BUFFER)
					{
						handleNum += i + 1;
						gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
						(
							basicHeaps[modelData.key]->GetGPUDescriptorHandleForHeapStart(),
							handleNum,
							dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
						);
						cmdList->SetGraphicsRootDescriptorTable(3, gpuDescHandle);
					}
				}

				//描画
				cmdList->DrawIndexedInstanced(static_cast<UINT>(indices[modelData.key][i].size()), 1, 0, 0, 0);

			}

		}

	}
}

//Map処理
void DirectX12::DataMap(const ModelData& modelData,int number )
{


	//constBufferSet[despNumber][number].constBuffer.Get()->Unmap(0, nullptr);
	result = constBufferSet[modelData.key][number].constBuffer[0].Get()->Map(0, nullptr, (void**)&constData3D);

	//if (polyDatas[modelData.key].dimention == Dimension::dimention2D)
	//{

	//	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	//	matWorld *= DirectX::XMMatrixRotationZ
	//	(
	//		DirectX::XMConvertToRadians(modelConstData[modelData.key].angle[number].z)
	//	);
	//	matWorld *= DirectX::XMMatrixScaling
	//	(
	//		modelConstData[modelData.key].scale[number].x,
	//		modelConstData[modelData.key].scale[number].y,
	//		modelConstData[modelData.key].scale[number].z
	//	);
	//	matWorld *= DirectX::XMMatrixTranslation
	//	(
	//		modelConstData[modelData.key].position[number].x,
	//		modelConstData[modelData.key].position[number].y,
	//		modelConstData[modelData.key].position[number].z
	//	);

	//	constData3D->mat = matWorld * mainCamera->get2DCameraMatrix();
	//}


	constData3D->addColor = modelConstData[modelData.key][number].addColor;
	constData3D->subColor = modelConstData[modelData.key][number].subColor;
	constData3D->mulColor = modelConstData[modelData.key][number].mulColor;
	constData3D->ex = modelConstData[modelData.key][number].pushPolygonNum;

	DirectX::XMMATRIX normalMat;
	normalMat = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstData[modelData.key][number].angle.z));
	normalMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstData[modelData.key][number].angle.x));
	normalMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstData[modelData.key][number].angle.y));
	constData3D->normalMat = normalMat * cameraMat;

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

	//ビルボード
	//板ポリ以外ビルボードしないしこっちに書かずに3Dspriteのほうだけでよくねってなってコメントアウト
	//calcBillboardMat(matWorld);
	

	matWorld *= DirectX::XMMatrixScaling
	(
		modelConstData[modelData.key][number].scale.x,
		modelConstData[modelData.key][number].scale.y,
		modelConstData[modelData.key][number].scale.z
	);
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstData[modelData.key][number].angle.z));
	matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstData[modelData.key][number].angle.x));
	matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstData[modelData.key][number].angle.y));

	matWorld *= DirectX::XMMatrixTranslation
	(
		modelConstData[modelData.key][number].position.x,
		modelConstData[modelData.key][number].position.y,
		modelConstData[modelData.key][number].position.z
	);

	std::string parentHeap = parentHeaps[modelData.key][number];
	int parentNum = parentNums[modelData.key][number];
	while (1)
	{
		if (parentNum == -1 || parentHeap == "NONE")
		{
			break;
		}

		//親子構造は親を動かしたら同時に子を動かすもの
		//下のコメントアウトされている処理を行うと、setPositionに親からどのくらい離れているかではなく、
		//通常通り座標を入力することになるが、一緒に移動させる場合、親の座標を加算したものをsetPositionに書かなければいけない
		//連動させるものなのに、子をわざわざ動かさないといけないのはどうかと思ったので、とりあえずコメントアウトした
		//matWorld *= DirectX::XMMatrixTranslationFromVector(
		//	DirectX::XMVECTOR(
		//		DirectX::XMVectorSet(
		//			-position[parentHeap][parentNum].x,
		//			-position[parentHeap][parentNum].y,
		//			-position[parentHeap][parentNum].z,
		//			1.0f)));

		matWorld *= DirectX::XMMatrixScaling
		(
			modelConstData[parentHeap][parentNum].scale.x,
			modelConstData[parentHeap][parentNum].scale.y,
			modelConstData[parentHeap][parentNum].scale.z
		);
		matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstData[parentHeap][parentNum].angle.z));
		matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstData[parentHeap][parentNum].angle.x));
		matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstData[parentHeap][parentNum].angle.y));

		matWorld *= DirectX::XMMatrixTranslation
		(
			modelConstData[parentHeap][parentNum].position.x,
			modelConstData[parentHeap][parentNum].position.y,
			modelConstData[parentHeap][parentNum].position.z
		);



		std::string kariParentHeap = parentHeaps[parentHeap][parentNum];
		int kariParentNum = parentNums[parentHeap][parentNum];

		parentHeap = kariParentHeap;
		parentNum = kariParentNum;

	}


	DirectX::XMMATRIX viewAndProjectionMat = mainCamera->Get3DCameraMatrix(mainCameraData);
	if (isPlane)
	{
		DirectX::XMFLOAT4 shadowNormal =
		{ 0.0f,1.0f,0.0f, 0.0f };

		DirectX::XMFLOAT4 rLight = { lightVector.x * -1,lightVector.y * -1 ,lightVector.z * -1 ,0.0f };

		//1つ目の引数は、平らにした時にどの方向に法線ベクトルが向いてるかを入力する
		DirectX::XMMATRIX matShadow = DirectX::XMMatrixShadow
		(
			DirectX::XMLoadFloat4(&shadowNormal),
			DirectX::XMLoadFloat4(&rLight)
		);

		constData3D->mat = matWorld * matShadow * viewAndProjectionMat;
	}
	else
	{
		constData3D->mat = matWorld * viewAndProjectionMat;
	}

	constData3D->worldMat = matWorld;



	//オブジェクト分ループ
	size_t size = vertices[modelData.key].size();
	size_t size2 = 0;

	

	//ボーンデータがあるかどうか
	if (modelData.type != VertexType::VERTEX_TYPE_OBJ_ANIMATION)
	{
		for (size_t i = 0; i < size; i++)
		{
			vertexBufferSet[modelData.key][i].vertexBuffer.Get()->Map(0, nullptr, (void**)&vertexBufferSet[modelData.key][i].vertexMap);

			//スムージングを行うか
			if (smoothingFlag)
			{
				size2 = smoothNormal[modelData.key][i].size();
				for (size_t j = 0; j < size2; j++)
				{
					vertexBufferSet[modelData.key][i].vertexMap[j].normal = smoothNormal[modelData.key][i][j];
				}
			}
			else
			{
				size2 = vertices[modelData.key][i].size();
				for (size_t j = 0; j < size2; j++)
				{
					vertexBufferSet[modelData.key][i].vertexMap[j].normal = vertices[modelData.key][i][j].normal;
				}
			}
			vertexBufferSet[modelData.key][i].vertexBuffer.Get()->Unmap(0, nullptr);
		}
	}
	else//ボーンがあったら
	{
		//スムースシェーディング
		for (size_t i = 0; i < size; i++)
		{
			ObjAnimationVertex* aniVertex;
			vertexBufferSet[modelData.key][i].vertexBuffer.Get()->Map(0, nullptr, (void**)&aniVertex);

			//スムージングを行うか
			if (smoothingFlag)
			{
				size2 = smoothNormal[modelData.key][i].size();
				for (size_t j = 0; j < size2; j++)
				{
					aniVertex[j].normal = smoothNormal[modelData.key][i][j];
				}
			}
			else
			{
				size2 = vertices[modelData.key][i].size();
				for (size_t j = 0; j < size2; j++)
				{
					aniVertex[j].normal = vertices[modelData.key][i][j].normal;
				}
			}
			vertexBufferSet[modelData.key][i].vertexBuffer.Get()->Unmap(0, nullptr);
		}

		//ボーンの値マップ
		DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 boneScale;
		DirectX::XMFLOAT3 boneAngle;
		DirectX::XMFLOAT3 boneMoveVector;
		UINT boneNum = static_cast<UINT>(boneData[modelData.key][number].size());

		//親ボーンの行列乗算
		int parentBoneNum = 0;
		int bone = 0;

		DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
		DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
		DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
		DirectX::XMFLOAT3 bonePos = { 0,0,0 };


		for (UINT i = 0; i < boneNum; i++)//0は設定しないようにする(0はボーン未割当ての頂点の行列なので、いじらないようにする)
		{
			boneMat = DirectX::XMMatrixIdentity();

			//ボーンから頂点の距離分移動
			bonePos = objBonePositions[modelData.key][i];
			boneMat *= DirectX::XMMatrixTranslation(-bonePos.x, -bonePos.y, -bonePos.z);

			boneScale = boneData[modelData.key][number][i].scale;
			boneMat *= DirectX::XMMatrixScaling(boneScale.x, boneScale.y, boneScale.z);

			boneAngle = boneData[modelData.key][number][i].angle;
			boneMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(boneAngle.z));
			boneMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(boneAngle.x));
			boneMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(boneAngle.y));

			boneMoveVector = boneData[modelData.key][number][i].moveVector;

			//なぜかxとzが+-逆になってる
			//シェーダーでモデルの行列を乗算する前にボーンの行列を乗算してたからだった
			//(モデルはY軸基準で180度回転してた)
			boneMat *= DirectX::XMMatrixTranslation(boneMoveVector.x, boneMoveVector.y, boneMoveVector.z);

			//回転させたら戻す
			boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

			boneAngleImpact = parentBoneData[modelData.key][i].angleImpact;
			boneScaleImpact = parentBoneData[modelData.key][i].scaleImpact;
			boneMoveVectorImpact = parentBoneData[modelData.key][i].moveVectorImpact;

			//親のボーン番号代入
			parentBoneNum = parentBoneData[modelData.key][i].parentBoneNum;

			//これを使って影響度とか加算してく
			//子　→　親の順で入れる
			std::vector<int>parentNums;
			parentNums.reserve(50);

			//こちらは親　→　子
			std::vector<DirectX::XMMATRIX>parentMat;
			parentMat.reserve(50);


			//先にすべての親を取得&影響度取得
			while (1)
			{
				//-1だったら(親がセットされてなかったら)抜ける
				if (parentBoneNum == -1)break;

				//parentAngleImpacts.push_back(parentBoneData[modelData.key][parentBoneNum].angleImpact);
				//parentScaleImpacts.push_back(parentBoneData[modelData.key][parentBoneNum].scaleImpact);
				//parentMoveVectorImpacts.push_back(parentBoneData[modelData.key][parentBoneNum].moveVectorImpact);
				//parentPosition.push_back(objBonePositions[modelData.key][parentBoneNum]);
				parentNums.push_back(parentBoneNum);

				//親のボーン番号代入
				parentBoneNum = parentBoneData[modelData.key][parentBoneNum].parentBoneNum;

			}
			if (parentNums.size() != 0)
			{


				DirectX::XMMATRIX mulMat = DirectX::XMMatrixIdentity();
				const int maxParentSize = static_cast<int>(parentNums.size());

				DirectX::XMFLOAT3 pAngle = { 0,0,0 };
				DirectX::XMFLOAT3 pScale = { 1,1,1 };
				DirectX::XMFLOAT3 pMoveVector = { 0,0,0 };
				DirectX::XMFLOAT3 pPos = { 0,0,0 };

				DirectX::XMFLOAT3 pAngleImpact = { 1,1,1 };
				DirectX::XMFLOAT3 pScaleImpact = { 1,1,1 };
				DirectX::XMFLOAT3 pMoveVectorImpact = { 1,1,1 };

				mulMat = DirectX::XMMatrixIdentity();

				//最後にある親のボーンを基準に回すので、入れる
				pPos.x = objBonePositions[modelData.key][parentNums[maxParentSize - 1]].x;
				pPos.y = objBonePositions[modelData.key][parentNums[maxParentSize - 1]].y;
				pPos.z = objBonePositions[modelData.key][parentNums[maxParentSize - 1]].z;
				
				int impactIndex = i;
				for (auto& num : parentNums)
				{
					pAngle.x += boneData[modelData.key][number][num].angle.x;
					pAngle.y += boneData[modelData.key][number][num].angle.y;
					pAngle.z += boneData[modelData.key][number][num].angle.z;

					pScale.x *= boneData[modelData.key][number][num].scale.x;
					pScale.y *= boneData[modelData.key][number][num].scale.y;
					pScale.z *= boneData[modelData.key][number][num].scale.z;

					pMoveVector.x += boneData[modelData.key][number][num].moveVector.x;
					pMoveVector.y += boneData[modelData.key][number][num].moveVector.y;
					pMoveVector.z += boneData[modelData.key][number][num].moveVector.z;



					pAngleImpact.x *= parentBoneData[modelData.key][impactIndex].angleImpact.x;
					pAngleImpact.y *= parentBoneData[modelData.key][impactIndex].angleImpact.y;
					pAngleImpact.z *= parentBoneData[modelData.key][impactIndex].angleImpact.z;
					pScaleImpact.x *= parentBoneData[modelData.key][impactIndex].scaleImpact.x;
					pScaleImpact.y *= parentBoneData[modelData.key][impactIndex].scaleImpact.y;
					pScaleImpact.z *= parentBoneData[modelData.key][impactIndex].scaleImpact.z;
					pMoveVectorImpact.x *= parentBoneData[modelData.key][impactIndex].moveVectorImpact.x;
					pMoveVectorImpact.y *= parentBoneData[modelData.key][impactIndex].moveVectorImpact.y;
					pMoveVectorImpact.z *= parentBoneData[modelData.key][impactIndex].moveVectorImpact.z;
				}

				pAngle.x *= pAngleImpact.x;
				pAngle.y *= pAngleImpact.y;
				pAngle.z *= pAngleImpact.z;

				pScale.x *= pScaleImpact.x;
				pScale.y *= pScaleImpact.y;
				pScale.z *= pScaleImpact.z;

				pMoveVector.x *= pMoveVectorImpact.x;
				pMoveVector.y *= pMoveVectorImpact.y;
				pMoveVector.z *= pMoveVectorImpact.z;


				//ボーンから頂点の距離分移動
				mulMat *= DirectX::XMMatrixTranslation(-pPos.x, -pPos.y, -pPos.z);

				mulMat *= DirectX::XMMatrixScaling(pScale.x, pScale.y, pScale.z);

				mulMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(pAngle.z));
				mulMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(pAngle.x));
				mulMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(pAngle.y));

				mulMat *= DirectX::XMMatrixTranslation(pMoveVector.x, pMoveVector.y, pMoveVector.z);

				//回転させたら戻す
				mulMat *= DirectX::XMMatrixTranslation(pPos.x, pPos.y, pPos.z);


				boneMat *= mulMat;
			}
			constData3D->boneMat[i + 1] = boneMat;

		}


	}



	constBufferSet[modelData.key][number].constBuffer[0].Get()->Unmap(0, nullptr);

}


void DirectX12::SpriteSetCmdList(int spriteNumber, int textureNumber, const bool& sprite3DFlag)
{
	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	std::vector<ID3D12DescriptorHeap*> ppHeaps;

	//ハンドルvectorやめてfor内に毎回宣言したほうがいい?そうすればずらしたやつを勝手に直してくれる
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	cmdList->SetGraphicsRootSignature(spriteRootsignature.Get());
	if(!sprite3DFlag)
		cmdList->SetPipelineState(spritePipelineStates[spritePipelineNum].Get());
	else
		cmdList->SetPipelineState(sprite3DPipelineStates[sprite3DPipelineNum].Get());

	cmdList->IASetVertexBuffers(0, 1, &spriteVertexBuffSet[spriteNumber].vertexBufferView);
	cmdList->IASetIndexBuffer(&spriteIndexBufferSet[spriteNumber].indexBufferView);

	ppHeaps.push_back(spriteHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

	gpuDescHandle = spriteHeap->GetGPUDescriptorHandleForHeapStart();
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(gpuDescHandle, textureNumber, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
	//テクスチャ
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	//定数セット
	cmdList->SetGraphicsRootConstantBufferView(0, spriteConstBufferSet[spriteNumber].constBuffer[0]->GetGPUVirtualAddress());

	cmdList->DrawIndexedInstanced(static_cast<UINT>(spriteIndices[spriteNumber].size()), 1, 0, 0, 0);

}

void DirectX12::SpriteDataMap(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, int spriteNumber, int textureNum)
{

#pragma region 頂点決定
	if (size.x == 0 && size.y == 0)
	{
		size.x = (float)spriteTextureData[textureNum].width;
		size.y = (float)spriteTextureData[textureNum].height;
	}

	//辺の長さを2DはYを+にして3DはYを-しないといけないのは面倒なので、どちらも+に統一するため
	//辺の長さを入れるようにしたいのに、3Dだと+で入力したときに表示されないから
	//*= -1して+にする


	float width = size.x;
	float height = size.y;
	width /= 2;
	height /= 2;

	std::vector<Vertex>vertex;
	vertex = createPolygon->SetBoardPolygonVertex
	(
		{ 0 - width,size.y - height,0, },
		{ 0 - width,0 - height, 0, },
		{ size.x - width,size.y - height,0 },
		{ size.x - width,0 - height,0, }
	);

	//スケールを乗算
	//for (int i = 0; i < 4; i++) 
	//{
	//	vertex[i].pos.x *= spriteScale[spriteNumber].x;
	//	vertex[i].pos.y *= spriteScale[spriteNumber].y;
	//}

	//vertex = createPolygon->setBoardPolygonVertex
	//(
	//	{0,100,0},
	//	{0,0,0},
	//	{100,100,0},
	//	{100,0,0}
	//);

	vertex[1].uv = { 0.0f,0.0f };
	vertex[0].uv = { 0.0f,1.0f };
	vertex[3].uv = { 1.0f,0.0f };
	vertex[2].uv = { 1.0f,1.0f };
	spriteVertices[spriteNumber] = vertex;


	std::vector<unsigned short>index;
	index = createPolygon->SetBoardPolygonIndex();
	spriteIndices[spriteNumber] = index;

	for (int i = 0; i < (int)vertex.size(); i++)
	{
		spriteVertexBuffSet[spriteNumber].vertexMap[i] = vertex[i];
	}
	for (int i = 0; i < (int)index.size(); i++)
	{
		spriteIndexBufferSet[spriteNumber].indexMap[i] = index[i];
	}

#pragma endregion

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();
	matWorld *= DirectX::XMMatrixScaling(spriteScale[spriteNumber].x, spriteScale[spriteNumber].y, 0.0f);
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(spriteAngle[spriteNumber].z));
	//左上基準にするためにサイズとか足してる
	matWorld *= DirectX::XMMatrixTranslation
	(
		position.x + width * spriteScale[spriteNumber].x,
		position.y + height * spriteScale[spriteNumber].y,
		0.0f
	);

	spriteConstBufferDatas[spriteNumber]->mat = matWorld * mainCamera->Get2DCameraMatrix();


}

void DirectX12::Sprite3DDataMap
(
	const DirectX::XMFLOAT3& position,
	const DirectX::XMFLOAT2& size,
	const int& spriteNumber,
	const int& textureNumber
)
{
#pragma region 頂点決定
	

	float width = size.x;
	float height = size.y;
	width /= 2;
	height /= 2;

	std::vector<Vertex>vertex;
	vertex = createPolygon->SetBoardPolygonVertex
	(
		{ 0 - width,0 - height, 0, },
		{ 0 - width,size.y - height,0, },
		{ size.x - width,0 - height,0, },
		{ size.x - width, size.y - height, 0 }
	);

	vertex[0].uv = { 0.0f,1.0f };
	vertex[1].uv = { 0.0f,0.0f };
	vertex[2].uv = { 1.0f,1.0f };
	vertex[3].uv = { 1.0f,0.0f };

	spriteVertices[spriteNumber] = vertex;

	std::vector<unsigned short>index;
	index = createPolygon->SetBoardPolygonIndex();
	spriteIndices[spriteNumber] = index;

	for (int i = 0; i < (int)vertex.size(); i++)
		spriteVertexBuffSet[spriteNumber].vertexMap[i] = vertex[i];
	for (int i = 0; i < (int)index.size(); i++)
		spriteIndexBufferSet[spriteNumber].indexMap[i] = index[i];
	
#pragma endregion


	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

	CalcBillboardMat(matWorld);
	matWorld *= DirectX::XMMatrixScaling(spriteScale[spriteNumber].x, spriteScale[spriteNumber].y,1.0f);
	matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(spriteAngle[spriteNumber].z));
	matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(spriteAngle[spriteNumber].x));
	matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(spriteAngle[spriteNumber].y));
	matWorld *= DirectX::XMMatrixTranslation
	(
		position.x,
		position.y,
		position.z
	);

	spriteConstBufferDatas[spriteNumber]->mat = matWorld * mainCamera->Get3DCameraMatrix(mainCameraData);

}

void DirectX12::PointSetCmdList(DirectX::XMFLOAT3 pos, int pointNum, int textureNum, int num)
{
	pointVertices[pointNum][num].pos = pos;
	pointVertexBuffSet[pointNum][num].vertexBuffer->Map(0, nullptr, (void**)&pointVertexMapData);

	pointVertexMapData[0] = pointVertices[pointNum][num];

	pointVertexBuffSet[pointNum][num].vertexBuffer->Unmap(0, nullptr);

	DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

	pointConstBufferSet[pointNum].constBuffer[0]->Map(0, nullptr, (void**)&constDataPoint);
	matWorld = mainCamera->Get3DCameraMatrix(mainCameraData);
	constDataPoint->mat = matWorld;
	constDataPoint->billboardMat = DirectX::XMMatrixIdentity();

	//ビルボード行列作成
	DirectX::XMMATRIX billboardMat = DirectX::XMMatrixIdentity();
	CalcBillboardMat(billboardMat);
	constDataPoint->billboardMat = billboardMat;

	pointConstBufferSet[pointNum].constBuffer[0]->Unmap(0, nullptr);


	//コマンドセット
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	std::vector<ID3D12DescriptorHeap*> ppHeaps;

	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

	cmdList->SetGraphicsRootSignature(pointRootsignature.Get());
	cmdList->SetPipelineState(pointPipelineStates[0].Get());

	cmdList->IASetVertexBuffers(0, 1, &pointVertexBuffSet[pointNum][num].vertexBufferView);

	ppHeaps.push_back(spriteHeap.Get());
	cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

	gpuDescHandle = spriteHeap->GetGPUDescriptorHandleForHeapStart();
	gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(gpuDescHandle, textureNum, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));


	//テクスチャ
	if (textureNum != -1)
		cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	//定数セット
	cmdList->SetGraphicsRootConstantBufferView(0, pointConstBufferSet[pointNum].constBuffer[0]->GetGPUVirtualAddress());

	cmdList->DrawInstanced(1, 1, 0, 0);
}

#pragma endregion

#pragma region 操作関数

std::vector<DirectX::XMFLOAT3>DirectX12::GetBonePosition(const std::string& key)
{
	return objBonePositions[key];
}

void DirectX12::SetObjModelRotatePoint
(
	const DirectX::XMFLOAT3& position,
	const UINT& boneNum,
	const ModelData& modelData
)
{
}

void DirectX12::SetObjectPosition(DirectX::XMFLOAT3 position, const std::string& key, int number)
{
	modelConstData[key][number].position = position;
}

void DirectX12::SetObjectAngle(DirectX::XMFLOAT3 angle, const std::string& key, int number)
{	
	modelConstData[key][number].angle = angle;
}

void DirectX12::SetObjectScale(DirectX::XMFLOAT3 scale, const std::string& key, int number)
{
	
	modelConstData[key][number].scale = scale;
	
}

void DirectX12::SetObjectPushNum(float objectEX, const std::string& key, int number)
{
	modelConstData[key][number].pushPolygonNum = objectEX;
}


//これ消す
void DirectX12::SetSpritePosition(DirectX::XMFLOAT2 position, int spriteNum)
{
	/*float width = spriteSizes[spriteNum].x;
	float height = spriteSizes[spriteNum].y;
	width /= 2;
	height /= 2;

	position.x += width;
	position.y += height;

	spritePosition[spriteNum] = DirectX::XMVECTOR(DirectX::XMVectorSet(position.x, position.y, 0, 1.0f));*/
}

void DirectX12::SetSpriteScale(DirectX::XMFLOAT2 scale, int spriteNum)
{
	spriteScale[spriteNum] = scale;
}

void DirectX12::SetSpriteAngle(const DirectX::XMFLOAT3& angle, const int& spriteNum)
{
	spriteAngle[spriteNum] = angle;
}


void DirectX12::ChangeSpriteSize(DirectX::XMFLOAT2 size, int spriteData)
{
	size.y *= -1;

	float width = size.x;
	float height = size.y;
	width /= 2;
	height /= 2;


	spriteVertices[spriteData] = createPolygon->SetBoardPolygonVertex
	(
		{ 0 - width,size.y - height,0, },
		{ 0 - width,0 - height, 0, },
		{ size.x - width,size.y - height,0 },
		{ size.x - width,0 - height,0, }
	);

	spriteVertices[spriteData][0].uv = { 0.0f,0.0f };
	spriteVertices[spriteData][1].uv = { 0.0f,1.0f };
	spriteVertices[spriteData][2].uv = { 1.0f,0.0f };
	spriteVertices[spriteData][3].uv = { 1.0f,1.0f };

	for (int j = 0; j < (int)spriteVertices[spriteData].size(); j++)
	{
		spriteVertexBuffSet[spriteData].vertexMap[j] = spriteVertices[spriteData][j];
	}

}



void DirectX12::SetPointScale(DirectX::XMFLOAT2 scale, int pointNum, int num)
{
	pointVertices[pointNum][num].scale = scale;
}


#pragma region ポストエフェクト

void DirectX12::SetRenderTargerPosition(const DirectX::XMFLOAT3& pos, const int& rtNum)
{
	postEffectWorldMatData[rtNum].pos = pos;
}

void DirectX12::SetRenderTargetAngle(const DirectX::XMFLOAT3& angle, const int& rtNum)
{
	postEffectWorldMatData[rtNum].angle = angle;
}

void DirectX12::SetRenderTargetScale(const DirectX::XMFLOAT3& scale, const int& rtNum)
{
	postEffectWorldMatData[rtNum].scale = scale;
}

void DirectX12::SetPostEffectCameraFlag(const bool& flag, const int& rtNum)
{
	postEffectCametaFlag = flag;
}
#pragma endregion

#pragma region アニメーション
void DirectX12::SetObjBoneMoveVector
(
	const DirectX::XMFLOAT3& vector,
	const UINT& boneNum,
	const std::string& key,
	const UINT& objectNum
)
{
	boneData[key][objectNum][boneNum].moveVector = vector;
}

void DirectX12::SetObjBoneScale
(
	const DirectX::XMFLOAT3& scale,
	const UINT& boneNum,
	const std::string& key,
	const UINT& objectNum
)
{
	boneData[key][objectNum][boneNum].scale = scale;
}

void DirectX12::SetObjBoneAngle
(
	const DirectX::XMFLOAT3& angle,
	const UINT& boneNum,
	const std::string& key,
	const UINT& objectNum
)
{
	boneData[key][objectNum][boneNum].angle = angle;
}

void DirectX12::SetParentObjBone
(
	const UINT& boneNum,
	const UINT& parentBoneNum,
	const std::string& key
)
{
	parentBoneData[key][boneNum].parentBoneNum = parentBoneNum;
}

void DirectX12::SetParentObjBoneScaleImpact
(
	const UINT& boneNum,
	const DirectX::XMFLOAT3& scaleImpact,
	const std::string& key
)
{
	parentBoneData[key][boneNum].scaleImpact = scaleImpact;
}

void DirectX12::SetParentObjBoneAngleImpact
(
	const UINT& boneNum,
	const DirectX::XMFLOAT3& angleImpact,
	const std::string& key
)
{
	parentBoneData[key][boneNum].angleImpact = angleImpact;
}

void DirectX12::SetParentObjBoneMoveVectorImpact
(
	const UINT& boneNum,
	const DirectX::XMFLOAT3& moveVectorImpact,
	const std::string& key
)
{
	parentBoneData[key][boneNum].moveVectorImpact = moveVectorImpact;
}


#pragma endregion


#pragma endregion

#pragma region 頂点座標取得
std::vector<std::vector<DirectX::XMFLOAT3>>  DirectX12::GetModelVerticesPosition(const std::string& key)
{

	std::vector<std::vector<DirectX::XMFLOAT3>>kari(vertices[key].size());

	int count = 0;
	for (auto& v : kari)
	{
		v.resize(vertices[key][count].size());
		count++;
	}

	for (int i = 0; i < kari.size(); i++)
	{
		for (int j = 0; j < kari[i].size(); j++)
		{
			kari[i][j] = vertices[key][i][j].pos;
		}
	}

	return kari;
}

//値が狂ってないけどちゃんと表示できてない
//Unmapしてたせい?
bool DirectX12::OverrideWriteVertexPosition(std::vector<std::vector<DirectX::XMFLOAT3>> vertPos, const std::string& key)
{
	//書き換えミス?

	//サイズが変わってたらfalse(indexとか書き換えてないから困る)
	if (vertices[key].size() != vertPos.size())return false;
	for (int i = 0; i < vertices[key].size(); i++)
	{
		if (vertices[key][i].size() != vertPos[i].size())
		{
			return false;
		}
	}

	//vertex書き換え
	size_t num, num2;
	num = vertPos.size();
	for (size_t i = 0; i < num; i++)
	{
		num2 = vertPos[i].size();
		for (size_t j = 0; j < num2; j++)
		{
			vertices[key][i][j].pos.x = vertPos[i][j].x;
			vertices[key][i][j].pos.y = vertPos[i][j].y;
			vertices[key][i][j].pos.z = vertPos[i][j].z;
		}
	}

	//map処理

	num = vertPos.size();
	for (size_t i = 0; i < num; i++)
	{
		vertexBufferSet[key][i].vertexBuffer.Get()->Map(0, nullptr, (void**)vertexBufferSet[key][i].vertexMap);
	}

	num = vertPos.size();
	for (size_t i = 0; i < num; i++)
	{
		num2 = vertPos[i].size();
		for (size_t j = 0; j < num2; j++)
		{
			vertexBufferSet[key][i].vertexMap[j] = vertices[key][i][j];
		}
	}

	vertexBufferSet[key][0].vertexBuffer.Get()->Unmap(0, nullptr);

	return true;
}
#pragma endregion

#pragma region 行列による変換

DirectX::XMFLOAT3 DirectX12::MatchEyeVelocityToCamera(DirectX::XMFLOAT3 eyeVector, bool flag)
{


	DirectX::XMMATRIX x = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(mainCameraData.eyeAngle.x));
	DirectX::XMMATRIX y = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(mainCameraData.eyeAngle.y));
	DirectX::XMMATRIX z = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(mainCameraData.eyeAngle.z));


#pragma region カメラ回転と移動
	if (eyeVector.x == 0 && eyeVector.y == 0 && eyeVector.z == 0)return { eyeVector };



	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation
	(
		eyeVector.x,
		eyeVector.y,
		eyeVector.z
	);


	matrix *= z * x*y;
	//matrix *= mainCamera->get3DCameraMatrix(mainCameraData);


	DirectX::XMFLOAT3 changeVector = { matrix.r[3].m128_f32[0],matrix.r[3].m128_f32[1] ,matrix.r[3].m128_f32[2] };


	if (flag)
	{
		float vectorSqrt = sqrt(pow(changeVector.x, 2) + pow(changeVector.y, 2) + pow(changeVector.z, 2));
		eyeVector.x = changeVector.x / vectorSqrt;
		eyeVector.y = changeVector.y / vectorSqrt;
		eyeVector.z = changeVector.z / vectorSqrt;
	}
	else
	{
		return changeVector;
	}


#pragma endregion

	return eyeVector;
}


DirectX::XMFLOAT3 DirectX12::GetRotateCameraPosition()
{
	//mainCameraの値書き換えるのよくない?
	DirectX::XMFLOAT3 sum;
	sum.x = mainCameraData.nowEye.x - mainCameraData.eye.x;
	sum.y = mainCameraData.nowEye.y - mainCameraData.eye.y;
	sum.z = mainCameraData.nowEye.z - mainCameraData.eye.z;

	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation
	(
		mainCameraData.eye.x,
		mainCameraData.eye.y,
		mainCameraData.eye.z
	);
	matrix *=
		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(mainCameraData.eyeAngle.z)) *
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(mainCameraData.eyeAngle.x))*
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(mainCameraData.eyeAngle.y)) *
		DirectX::XMMatrixTranslation
		(
			sum.x,
			sum.y,
			sum.z
		);

	return
	{ matrix.r[3].m128_f32[0],
	matrix.r[3].m128_f32[1],
	matrix.r[3].m128_f32[2] };
}

//ちょっと値が狂ってる?(注視点が座標と同じ高さじゃなかった)
DirectX::XMFLOAT3 DirectX12::GetRotateCameraTarget()
{
	DirectX::XMFLOAT3 sum;
	sum.x = mainCameraData.nowTarget.x - mainCameraData.target.x;
	sum.y = mainCameraData.nowTarget.y - mainCameraData.target.y;
	sum.z = mainCameraData.nowTarget.z - mainCameraData.target.z;

	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslationFromVector(DirectX::XMVectorSet(
		mainCameraData.eye.x,
		mainCameraData.eye.y,
		mainCameraData.eye.z,
		1.0f));
	matrix *=
		DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(mainCameraData.targetAngle.z)) *
		DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(mainCameraData.targetAngle.x))*
		DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(mainCameraData.targetAngle.y)) *
		DirectX::XMMatrixTranslation
		(
			sum.x,
			sum.y,
			sum.z
		);

	return
	{ matrix.r[3].m128_f32[0],
	matrix.r[3].m128_f32[1],
	matrix.r[3].m128_f32[2] };
}

#pragma endregion

#pragma region 文字表示
void DirectX12::DrawSpriteFontString(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 size, std::string text, int spriteTextureNum)
{
	int loopCount = 0;
	for (auto& t : text)
	{
		//最大文字数超えたらリターン
		if (spriteFontDrawCounter > SPRITEFONT_MAX)
			return;

		int width = t - 32;//一旦widthに
		int height = 0;

		//場所を確認する
		while (width >= 14)
		{
			width -= 14;
			height++;
		}

		//描画処理
		SpriteDataMap({ position.x + size.x * loopCount,position.y + size.y }, size, spriteFontDrawCounter, 0);
		SetSpriteAnimationVertex
		(
			spriteFontDrawCounter,
			-1,
			{
				static_cast<float>(spriteFontData[spriteTextureNum].widthLineNum),
			static_cast<float>(spriteFontData[spriteTextureNum].heightLineNum)
			},
			{ static_cast<float>(width + 1),static_cast<float>(height + 1) }
		);



#pragma region 描画処理
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		std::vector<ID3D12DescriptorHeap*> ppHeaps;

		//ハンドルvectorやめてfor内に毎回宣言したほうがいい?そうすればずらしたやつを勝手に直してくれる
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;

		cmdList->SetGraphicsRootSignature(spriteRootsignature.Get());
		cmdList->SetPipelineState(spritePipelineStates[spritePipelineNum].Get());

		cmdList->IASetVertexBuffers(0, 1, &spriteVertexBuffSet[spriteFontDrawCounter].vertexBufferView);
		cmdList->IASetIndexBuffer(&spriteIndexBufferSet[spriteFontDrawCounter].indexBufferView);

		ppHeaps.push_back(spriteFontHeap.Get());
		cmdList->SetDescriptorHeaps(1, &ppHeaps[0]);

		gpuDescHandle = spriteFontHeap->GetGPUDescriptorHandleForHeapStart();
		gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(gpuDescHandle, spriteTextureNum, dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));
		//テクスチャ
		cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

		//定数セット
		cmdList->SetGraphicsRootConstantBufferView(0, spriteConstBufferSet[spriteFontDrawCounter].constBuffer[0]->GetGPUVirtualAddress());

		cmdList->DrawIndexedInstanced(static_cast<UINT>(spriteIndices[spriteFontDrawCounter].size()), 1, 0, 0, 0);
#pragma endregion

		spriteFontDrawCounter++;
		loopCount++;

	}


}
#pragma endregion

#pragma region 親子構造

void DirectX12::SetParent(const std::string& key, const int& number, const std::string& parentKey, const int& parentNum)
{
	parentHeaps[key][number] = parentKey;
	parentNums[key][number] = parentNum;
}
#pragma endregion

#pragma region 計算
void DirectX12::CalculationNormal
(
	DirectX::XMFLOAT3 pos1, DirectX::XMFLOAT3 pos2, DirectX::XMFLOAT3 pos3,
	DirectX::XMFLOAT3& normal1, DirectX::XMFLOAT3& normal2, DirectX::XMFLOAT3& normal3
)
{

	DirectX::XMVECTOR p0 = DirectX::XMLoadFloat3(&pos1);
	DirectX::XMVECTOR p1 = DirectX::XMLoadFloat3(&pos2);
	DirectX::XMVECTOR p2 = DirectX::XMLoadFloat3(&pos3);

	DirectX::XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
	DirectX::XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);

	DirectX::XMVECTOR normal = DirectX::XMVector3Cross(v1, v2);
	normal = DirectX::XMVector3Normalize(normal);

	DirectX::XMStoreFloat3(&normal1, normal);
	DirectX::XMStoreFloat3(&normal2, normal);
	DirectX::XMStoreFloat3(&normal3, normal);
}

void DirectX12::calcSmoothingNormals(const std::string key)
{

	for (int i = 0; i < vertices[key].size(); i++)
	{
		for (int j = 0; j < vertices[key][i].size(); j++)
		{
			smoothNormal[key][i][j] = vertices[key][i][j].normal;
		}
	}

	//法線(平均求める用配列。ここに入れて、平均を求める)
	std::vector<DirectX::XMFLOAT3>sNor;

	//オブジェクト分ループ
	for (int i = 0; i < smoothData.size(); i++)
	{
		auto itr = smoothData[i].begin();
		std::vector<USHORT>ver;
		for (; itr != smoothData[i].end(); ++itr)
		{
			ver = itr->second;
			for (auto& v : ver)
			{
				//一気に24個入ってるし、clearしてないからおかしかった
				sNor.push_back(vertices[key][i][v].normal);
			}

			//法線平均化
			DirectX::XMVECTOR aveNormal = { 0,0,0 };
			for (auto& n : sNor)
			{
				aveNormal.m128_f32[0] += n.x;
				aveNormal.m128_f32[1] += n.y;
				aveNormal.m128_f32[2] += n.z;
			}
			aveNormal.m128_f32[0] /= sNor.size();
			aveNormal.m128_f32[1] /= sNor.size();
			aveNormal.m128_f32[2] /= sNor.size();
			aveNormal = DirectX::XMVector3Normalize(aveNormal);
			for (auto& v : ver)
			{
				smoothNormal[key][i][v] = { aveNormal.m128_f32[0],aveNormal.m128_f32[1], aveNormal.m128_f32[2] };
			}
			sNor.clear();
		}
	}


}
#pragma endregion

#pragma region ライブラリ使用関数
void DirectX12::SortModelData(std::vector<std::tuple<ModelData, int>>& modelDatas)
{
	std::sort(modelDatas.begin(), modelDatas.end(), [&]
	(
		const std::tuple<ModelData, int>& m1,
		const std::tuple<ModelData, int>& m2
		)
	{
		ModelData m1Data = std::get<0>(m1);
		ModelData m2Data = std::get<0>(m2);
		int m1Num = std::get<1>(m1);
		int m2Num = std::get<1>(m2);

		DirectX::XMFLOAT3 m1Pos = modelConstData[m1Data.key][m1Num].position;
		DirectX::XMFLOAT3 m2Pos = modelConstData[m2Data.key][m2Num].position;

		Vector3 nearPos;
		Vector3 farPos;
		DirectInput::GetMouse3DLine(nearPos, farPos);
		float dis1 = LibMath::CalcDistance3D(m1Pos, nearPos);
		float dis2 = LibMath::CalcDistance3D(m2Pos, nearPos);

		return dis1 > dis2;

	});
}


#pragma endregion

