#include"ModelObject.h"
#include"CreateBuffer.h"
#include"FbxLoader.h"
#include"DirectionalLight.h"
#include"Matrix.h"
#include"CollisionType.h"
#include"LibMath.h"
#include"Values.h"
#include"Collision.h"

using namespace MelLib;

std::unordered_map<std::string, std::unique_ptr<ModelObject>>ModelObject::pModelObjects;


ID3D12Device* ModelObject::device;
std::vector<ID3D12GraphicsCommandList*>ModelObject::cmdLists;
ComPtr<ID3D12RootSignature> ModelObject::rootSignature;

ModelObject::ModelObject(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	Create(pModelData, userConstBufferData);
}

void ModelObject::CreateConstBuffer()
{
	ConstBufferData::BufferType modelConstBufferType = modelConstBufferData.bufferType;
	ConstBufferData::BufferType userConstBufferType = userConstBufferData.bufferType;

#pragma region リサイズ

	constBuffer.resize(modelFileObjectNum);


	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		modelConstBuffer.resize(1);
	}
	else if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		modelConstBuffer.resize(modelFileObjectNum);
	}


	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		userConstBuffer.resize(1);
	}
	else if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
	{
		userConstBuffer.resize(modelFileObjectNum);
	}

#pragma endregion


	auto CreateBuffer = [&]
	(
		ID3D12Resource** pBuffer,
		const size_t& dataSize
		)
	{
		CreateBuffer::GetInstance()->CreateConstBuffer
		(
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			dataSize,
			pBuffer
		);
	};


	//モデル
	if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&modelConstBuffer[0],
			modelConstBufferData.bufferDataSize
		);
	}

	//ユーザー
	if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
	{
		CreateBuffer
		(
			&userConstBuffer[0],
			userConstBufferData.bufferDataSize
		);
	}

	//モデル内のオブジェクト分ループ
	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region 生成


		//メイン
		CreateBuffer
		(
			&constBuffer[i],
			sizeof(ModelConstBufferData)
		);


		//モデル
		if (modelConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&modelConstBuffer[i],
				modelConstBufferData.bufferDataSize
			);
		}

		//ユーザー
		if (userConstBufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
		{
			CreateBuffer
			(
				&userConstBuffer[i],
				userConstBufferData.bufferDataSize
			);
		}
#pragma endregion

#pragma region 初期化

		ModelConstBufferData* constBufferData;
		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);

		//ライト
		for (int j = 0, size = DirectionalLight::LIGTH_MAX; j < size; j++)
		{
			constBufferData->light[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
			constBufferData->lightColor[j] = DirectX::XMFLOAT4(0, 0, 0, 0);
		}

		constBuffer[i]->Unmap(0, nullptr);
#pragma endregion
	}


}

void ModelObject::DrawCommonProcessing(const std::string& rtName)
{
	if (!pModelData)return;
	MapConstData(RenderTarget::Get(rtName)->GetCamera());
	SetCmdList();
}

void ModelObject::MapConstData(const Camera* camera)
{
	ModelConstBufferData* constBufferData;

	for (int i = 0; i < modelFileObjectNum; i++)
	{
#pragma region 基本的な情報のマップ

		constBuffer[i]->Map(0, nullptr, (void**)&constBufferData);
		constBufferData->addColor = modelConstDatas[i].addColor;
		constBufferData->subColor = modelConstDatas[i].subColor;
		constBufferData->mulColor = modelConstDatas[i].mulColor;
		constBufferData->ex = modelConstDatas[i].pushPolygonNum;

		std::vector<DirectionalLight*> pLights = DirectionalLight::GetAll();
		for (int i = 0, size = pLights.size(); i < size; i++)
		{
			Vector3 lightDir = pLights[i]->GetDirection();
			constBufferData->light[i] = DirectX::XMFLOAT4(lightDir.x, lightDir.y, lightDir.z, 0);
			Color lightCor = pLights[i]->GetColor();
			constBufferData->lightColor[i] = DirectX::XMFLOAT4
			(
				(float)lightCor.r / 255.0f,
				(float)lightCor.g / 255.0f,
				(float)lightCor.b / 255.0f,
				(float)lightCor.a / 255.0f
			);


		}

		Vector3 cameraPos = camera->GetCameraPosition();
		constBufferData->cameraPos = DirectX::XMFLOAT4(cameraPos.x, cameraPos.y, cameraPos.z, 0);

#pragma region 行列計算
		DirectX::XMMATRIX matWorld = DirectX::XMMatrixIdentity();

		matWorld *= DirectX::XMMatrixScaling
		(
			modelConstDatas[i].scale.x,
			modelConstDatas[i].scale.y,
			modelConstDatas[i].scale.z
		);
		matWorld *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(modelConstDatas[i].angle.z));
		matWorld *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(modelConstDatas[i].angle.x));
		matWorld *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(modelConstDatas[i].angle.y));

		constBufferData->normalMat = matWorld;

		matWorld *= DirectX::XMMatrixTranslation
		(
			modelConstDatas[i].position.x,
			modelConstDatas[i].position.y,
			modelConstDatas[i].position.z
		);


		constBufferData->mat = matWorld * camera->GetViewAndProjectionMat();
		constBufferData->worldMat = matWorld;

#pragma endregion


#pragma region レンダリング方向
		//上下左右前後
		std::array<float, 6>dirPos = pModelData->GetDirectionMaxPosition()[i];

		//仮に前から奧
		constBufferData->tex3DStart = dirPos[4];
		constBufferData->tex3DEnd = dirPos[5];
#pragma endregion


		constBuffer[i]->Unmap(0, nullptr);

#pragma endregion

#pragma region ボーンのマップ

		int boneNum = pModelData->GetBoneNumber();

		if (boneNum == 0
			|| pModelData->GetModelFormat() != ModelData::ModelFormat::MODEL_FORMAT_OBJ
			&& i != 0)return;

		SkinConstBufferData* skinConstData;

		modelConstBuffer[i]->Map(0, nullptr, (void**)&skinConstData);

		if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
		{
			std::vector<Vector3>objBonePositions = pModelData->GetObjBonePosition();

			DirectX::XMMATRIX boneMat = DirectX::XMMatrixIdentity();
			DirectX::XMFLOAT3 boneScale;
			DirectX::XMFLOAT3 boneAngle;
			DirectX::XMFLOAT3 boneMoveVector;

			//親ボーンの行列乗算
			int parentBoneNum = 0;
			int bone = 0;

			DirectX::XMFLOAT3 bonePos = { 0,0,0 };


			for (UINT j = 0; j < boneNum; j++)
			{
				boneMat = DirectX::XMMatrixIdentity();

				//ボーンから頂点の距離分移動
				bonePos = objBonePositions[j].ToXMFLOAT3();
				boneMat *= DirectX::XMMatrixTranslation(-bonePos.x, -bonePos.y, -bonePos.z);

				boneScale = boneDatas[j].scale;
				boneMat *= DirectX::XMMatrixScaling(boneScale.x, boneScale.y, boneScale.z);

				boneAngle = boneDatas[j].angle;
				boneMat *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(boneAngle.z));
				boneMat *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(boneAngle.x));
				boneMat *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(boneAngle.y));

				boneMoveVector = boneDatas[j].moveVector;

				//モデル自体のスケールの乗算により、ボーンの平行移動の値にスケールが乗算されるため、
				//割って増減を抑えている
				//ボーン→行列の順番で掛けるからモデルの倍率が影響しちゃう
				boneMat *= DirectX::XMMatrixTranslation
				(
					boneMoveVector.x / modelConstDatas[i].scale.x,
					boneMoveVector.y / modelConstDatas[i].scale.y,
					boneMoveVector.z / modelConstDatas[i].scale.z
				);

				//回転させたら戻す
				boneMat *= DirectX::XMMatrixTranslation(bonePos.x, bonePos.y, bonePos.z);

#pragma region 親の処理


				DirectX::XMFLOAT3 boneAngleImpact = { 0.0f,0.0f,0.0f };
				DirectX::XMFLOAT3 boneScaleImpact = { 1.0f,1.0f,1.0f };
				DirectX::XMFLOAT3 boneMoveVectorImpact = { 0.0f,0.0f,0.0f };
				boneAngleImpact = parentBoneDatas[j].angleImpact;
				boneScaleImpact = parentBoneDatas[j].scaleImpact;
				boneMoveVectorImpact = parentBoneDatas[j].moveVectorImpact;

				//親のボーン番号代入
				parentBoneNum = parentBoneDatas[j].parentBoneNum;

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
					parentNums.push_back(parentBoneNum);

					//親のボーン番号代入
					parentBoneNum = parentBoneDatas[parentBoneNum].parentBoneNum;

				}
				if (parentNums.size() != 0)
				{

					DirectX::XMMATRIX mulMat = DirectX::XMMatrixIdentity();

					//親数
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
					pPos.x = objBonePositions[parentNums[maxParentSize - 1]].x;
					pPos.y = objBonePositions[parentNums[maxParentSize - 1]].y;
					pPos.z = objBonePositions[parentNums[maxParentSize - 1]].z;

					int impactIndex = j;
					for (auto& pNum : parentNums)
					{
						pAngle.x += boneDatas[pNum].angle.x;
						pAngle.y += boneDatas[pNum].angle.y;
						pAngle.z += boneDatas[pNum].angle.z;

						pScale.x *= boneDatas[pNum].scale.x;
						pScale.y *= boneDatas[pNum].scale.y;
						pScale.z *= boneDatas[pNum].scale.z;

						pMoveVector.x += boneDatas[pNum].moveVector.x;
						pMoveVector.y += boneDatas[pNum].moveVector.y;
						pMoveVector.z += boneDatas[pNum].moveVector.z;



						pAngleImpact.x *= parentBoneDatas[impactIndex].angleImpact.x;
						pAngleImpact.y *= parentBoneDatas[impactIndex].angleImpact.y;
						pAngleImpact.z *= parentBoneDatas[impactIndex].angleImpact.z;
						pScaleImpact.x *= parentBoneDatas[impactIndex].scaleImpact.x;
						pScaleImpact.y *= parentBoneDatas[impactIndex].scaleImpact.y;
						pScaleImpact.z *= parentBoneDatas[impactIndex].scaleImpact.z;
						pMoveVectorImpact.x *= parentBoneDatas[impactIndex].moveVectorImpact.x;
						pMoveVectorImpact.y *= parentBoneDatas[impactIndex].moveVectorImpact.y;
						pMoveVectorImpact.z *= parentBoneDatas[impactIndex].moveVectorImpact.z;

						impactIndex = pNum;
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


					//親はまとめて掛けているので、モデルと自分の拡縮だけ掛ければいい
					//modelConstDatas[i][0].scale に boneScale掛ける必要あるか要確認
					mulMat *= DirectX::XMMatrixTranslation
					(
						pMoveVector.x / (modelConstDatas[i].scale.x * boneScale.x),
						pMoveVector.y / (modelConstDatas[i].scale.y * boneScale.y),
						pMoveVector.z / (modelConstDatas[i].scale.z * boneScale.z)
					);

					//回転させたら戻す
					mulMat *= DirectX::XMMatrixTranslation(pPos.x, pPos.y, pPos.z);


					boneMat *= mulMat;
				}
#pragma endregion

				skinConstData->bones[j + 1] = boneMat;

			}


		}
		else if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_FBX)
		{

			std::vector<ModelData::FbxBone> bones = pModelData->GetFbxBone();

			for (int i = 0; i < boneNum; i++)
			{
				//変換
				DirectX::XMMATRIX matCurrentPose;
				FbxAMatrix fbxCurrentPose =
					bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxAnimationData.currentTime);
				FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matCurrentPose, fbxCurrentPose);

				//乗算
				skinConstData->bones[i] = bones[i].invInitialPose * matCurrentPose;

			}
		}

		modelConstBuffer[i]->Unmap(0, nullptr);
#pragma endregion

	}


}

void ModelObject::SetCmdList()
{

	cmdLists[0]->SetGraphicsRootSignature(rootSignature.Get());
	cmdLists[0]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);



	//モデル特有バッファセット
	if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[0]->GetGPUVirtualAddress());

	//ユーザーモデルバッファセット
	if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL)
		cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[0]->GetGPUVirtualAddress());



	const std::vector<VertexBufferSet>& vertexBufferSets = pModelData->GetVertexBufferSet();
	const std::vector<IndexBufferSet>& indexBufferSets = pModelData->GetIndexBufferSet();


	//モデルのオブジェクトごとをセット
	//頂点バッファ分ループ
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		//cmdLists[0]->SetPipelineState(pPipeline[i]->GetPipelineState().Get());
		cmdLists[0]->SetPipelineState(materials[i]->GetPPipelineState()->GetPipelineState().Get());


		cmdLists[0]->IASetVertexBuffers(0, 1, &vertexBufferSets[i].vertexBufferView);
		cmdLists[0]->IASetIndexBuffer(&indexBufferSets[i].indexBufferView);

#pragma region テクスチャ

		ID3D12DescriptorHeap* textureDescHeap = materials[i]->GetPTextureHeap();
		std::vector<ID3D12DescriptorHeap*> ppHeaps;
		ppHeaps.push_back(textureDescHeap);
		cmdLists[0]->SetDescriptorHeaps(1, &ppHeaps[0]);


		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE
		(
			materials[i]->GetPTextureHeap()->GetGPUDescriptorHandleForHeapStart(),
			i,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		);
		cmdLists[0]->SetGraphicsRootDescriptorTable(TEXURE_ROOTPARAM_NUM, gpuDescHandle);
#pragma endregion

#pragma region 定数



		//定数バッファ
		cmdLists[0]->SetGraphicsRootConstantBufferView(CONST_BUFFER_REGISTER, constBuffer[i]->GetGPUVirtualAddress());

		//マテリアルバッファ
		cmdLists[0]->SetGraphicsRootConstantBufferView
		(MATERIAL_BUFFER_REGISTER, materials[i]->GetPConstBuffer(Material::MaterialConstBufferType::MATERIAL_DATA)->GetGPUVirtualAddress());

		//Colorマテリアル
		cmdLists[0]->SetGraphicsRootConstantBufferView
		(COLOR_MATERIAL_BUFFER_REGISTER, materials[i]->GetPConstBuffer(Material::MaterialConstBufferType::COLOR)->GetGPUVirtualAddress());

		//モデルバッファ
		if (modelConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, modelConstBuffer[i]->GetGPUVirtualAddress());

		//ユーザー定数バッファ
		if (userConstBufferData.bufferType == ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT)
			cmdLists[0]->SetGraphicsRootConstantBufferView(MODEL_BUFFER_REGISTER, userConstBuffer[i]->GetGPUVirtualAddress());


#pragma endregion


		//描画
		cmdLists[0]->DrawIndexedInstanced(static_cast<UINT>(pModelData->GetIndices()[i].size()), 1, 0, 0, 0);

	}

}

void ModelObject::Draw(const std::string& rtName)
{
	RenderTarget::ChangeCurrentRenderTarget(RenderTarget::Get(rtName));

	FbxAnimation();

	DrawCommonProcessing(rtName);
}

void MelLib::ModelObject::MeshCat(const PlaneData& plane, ModelData* pFront, ModelData* pBack)
{
	//緊急(解決優先度高めの現在の問題点)
	//1.そもそも四角形でも斬り方によっては5角形になるので、どっちみち面の再形成は必要
	//やり方は、斬られた辺の自分側(反対側じゃない方)の頂点と衝突点で多角形の三角形化を行う
	//斬られた辺に関わる部分だけ行えばよい

	//やること
	//1.当たった辺だけじゃなく、全頂点に対して表裏判定を行う。
	//(反対側の頂点をすべて消さないといけないため)

	//2.断面に使用する衝突点と入れ替える頂点かどうかの判断。
	//使うか使わないかだけわかればいい。
	//やり方は、ポートフォリオに書いてある
	//判断できたら、配列の何番目かとかの数値を取得しといて、断面の形成時に取得してインデックスとして使えるようにしとく
	//正直入れ替えずに消し飛ばして、後から追加してもいい

	//3.消し飛ばしたら、頂点だけではなくインデックスも変更する。
	//反対側の頂点が関係してるインデックスのセット(三角形を形成する添え字3つ)を消し飛ばす。
	//平面をまたいでる部分は消しちゃダメ。

	//現在の問題点
	//1.断面を形成するのに使う頂点とそうじゃない頂点があるが、断面を形成するのに使う頂点をどう取り出すか。
	//法線を元の頂点のやつを使いまわそうと思ったが、断面のように面を形成してない可能性があるので無理だった。
	//(四角形は断面と同じインデックスの面が元からあるからできる)
	//そもそも断面のように面を形成してない場合は、その頂点を追加しないといけない
	//そもそも切断してなくなる頂点を入れ替えずに消す?
	//結局断面を形成するのに使うか使わないかを判断しないといけない。
	//結局三角形を斬るときと同じように元からある面を形成しなおさないといけない?

	//2,斬った時に3つ以上分かれる場合はどうするか(wの真ん中斬った時など5個に分かれる)


	//思いついたこと
	//1.3角形も斬った時に面が消えるから、入れ替えればできる?
	//結局四角形に変えないといけないから無理。
	//多角形を三角形の集合体にするやつで面を形成しなおせばできる?

	//この処理はBoxとBoardで行う

	//とりあえず当たり判定処理をすっ飛ばして、
	//インデックスのセットと断面の描画を行う(断面用のテクスチャは割り当てない)


	//スケールだけではなく、回転と平行移動も考慮愛ないといけない
	//平行移動は問題ない。回転は、平面を回転させればいい

	//頂点に裏表の情報を持たせる?
	//シェーダーで分割する?

	//シェーダーで分割して、ストリーム出力で、インデックスも受け取れれば順序の問題は解決。
	//受け取れそうな気はするけど。そうしないと分割しても扱えないし


	//衝突点を求める
	//その位置を元に、シェーダーで点を生成。
	//ストリームで分割した頂点を出力
	//出力した頂点の表裏判定
	//2つのバッファに分ける

	//分割する三角形かどうか判別しないといけないため、
	//結局頂点に情報を追加しないといけない?
	//それとも、シェーダーで求められる?

	//ストリーム出力だとバッファに書き込むから、頂点データ取得と操作ができない?それともテクスチャバッファみたいに読み取れる?

	//ジオメトリシェーダーでやる処理をこちらで実装すれば楽に分割できる?


	//先に三角形ごとに分けて処理したほうがやりやすいかも
	//タプルに、インデックスを参考にValue3で頂点、インデックスの添え字(インデックス書き換え時にアクセスする用)


	//最初の段階で、隣を求めたほうがいい
	//既存の点(元々の三角形の点)と衝突点で距離を求め、その情報を元に、順序を決める

	//それでも無理だから、辺ごとに衝突点格納して、
	//三角形の座標どっちか、その座標が使われてる辺の衝突点、もう一個の衝突点、残りの座標という順序を使えばよい


	//三角形内部に頂点があるかないかの確認実装しといてね



	// 平面情報(回転適応のため、作り直し)
	PlaneData rotPlane;
	rotPlane.SetPosition(plane.GetPosition());
	rotPlane.SetNormal(LibMath::RotateZXYVector3(plane.GetNormal(),
		DirectX::XMFLOAT3(-modelConstDatas[0].angle.x, -modelConstDatas[0].angle.y, -modelConstDatas[0].angle.z)));

	//モデルの頂点を三角形ごとにまとめたもの
	struct ModelTri
	{
		//頂点
		Value3<FbxVertex> vertData;

		//表裏判定結果
		Value3<char>vertFB;

		//三角形の辺の情報
		Value3<Segment3DData>segmentData;

		//平面との衝突判定
		Value3<bool>hitResult;

		//衝突点の頂点情報
		Value3<FbxVertex>hitPosVert;

		//インデックスの配列のインデックス
		Value3<int> indicesIndex = 0;
	};


	std::vector<std::vector<FbxVertex>>vertPos = pModelData->GetVertices();
	std::vector<std::vector<USHORT>>indices = pModelData->GetIndices();

	std::vector<ModelTri>modelTri(indices[0].size() / 3);

	//三角形ごとにデータ格納
	for (int i = 0, size = indices[0].size(); i < size; i += 3)
	{
		int triIndex = 0;
		if (i == 0)triIndex = 0;
		else triIndex = i / 3;

		//頂点情報
		modelTri[triIndex].vertData.v1 = vertPos[0][indices[0][i]];
		modelTri[triIndex].vertData.v2 = vertPos[0][indices[0][i + 1]];
		modelTri[triIndex].vertData.v3 = vertPos[0][indices[0][i + 2]];

		//辺情報
		modelTri[triIndex].segmentData.v1.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v1.pos, modelTri[triIndex].vertData.v2.pos));
		modelTri[triIndex].segmentData.v2.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v2.pos, modelTri[triIndex].vertData.v3.pos));
		modelTri[triIndex].segmentData.v3.SetPosition
		(Value2<Vector3>(modelTri[triIndex].vertData.v3.pos, modelTri[triIndex].vertData.v1.pos));

		//インデックスの配列のインデックス
		modelTri[triIndex].indicesIndex.v1 = i;
		modelTri[triIndex].indicesIndex.v2 = i + 1;
		modelTri[triIndex].indicesIndex.v3 = i + 2;

		//裏表判定結果
		modelTri[triIndex].vertFB.v1 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v1.pos, rotPlane);
		modelTri[triIndex].vertFB.v2 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v2.pos, rotPlane);
		modelTri[triIndex].vertFB.v3 = LibMath::PointPlaneFrontBackCheck(modelTri[triIndex].vertData.v3.pos, rotPlane);

		//辺の衝突確認、衝突点取得
		Segment3DCalcResult res;
		modelTri[triIndex].hitResult.v1
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v1, &res);
		modelTri[triIndex].hitPosVert.v1.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v1.normal = modelTri[triIndex].vertData.v1.normal;
		//modelTri[triIndex].hitPosVert.v1.uv = modelTri[triIndex].vertData.v1.normal;

		modelTri[triIndex].hitResult.v2
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v2, &res);
		modelTri[triIndex].hitPosVert.v2.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v2.normal = modelTri[triIndex].vertData.v2.normal;
		//modelTri[triIndex].hitPosVert.v2.uv = modelTri[triIndex].vertData.v2.normal;

		modelTri[triIndex].hitResult.v3
			= Collision::PlaneAndSegment3D(rotPlane, modelTri[triIndex].segmentData.v3, &res);
		modelTri[triIndex].hitPosVert.v3.pos = res.planeHitPos.ToXMFLOAT3();
		modelTri[triIndex].hitPosVert.v3.normal = modelTri[triIndex].vertData.v3.normal;
		//modelTri[triIndex].hitPosVert.v3.uv = modelTri[triIndex].vertData.v3.normal;
	}

	//全頂点をまとめる配列。表と裏で分ける
	std::vector<FbxVertex>frontVertices;
	std::vector<FbxVertex>backVertices;
	std::vector<USHORT>frontIndices;
	std::vector<USHORT>backIndices;

	//三角形を見ていって、格納していく
	USHORT frontIndex = 0;
	USHORT backIndex = 0;
	for (const auto& tri : modelTri)
	{
		//一時的に格納するための配列
		std::vector<FbxVertex>fVert;
		std::vector<FbxVertex>bVert;


		//片方に偏ってたら(斬られてなかったら)インデックスを通常通り格納して次へ
		if (!tri.hitResult.v1 && !tri.hitResult.v2 && !tri.hitResult.v3)
		{
			if (tri.vertFB.v1 == 1)
			{
				frontVertices.push_back(tri.vertData.v1);
				frontVertices.push_back(tri.vertData.v2);
				frontVertices.push_back(tri.vertData.v3);

				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);

				frontIndex += 3;
			}
			else
			{
				backVertices.push_back(tri.vertData.v1);
				backVertices.push_back(tri.vertData.v2);
				backVertices.push_back(tri.vertData.v3);

				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);

				backIndex += 3;
			}
			continue;
		}


		//偏ってなかったら、表裏ごとに格納

		//順序決める

		
		//頂点のインデックス(frontInd、backIndに格納する値)
		std::vector<USHORT>fVertInd;
		std::vector<USHORT>bVertInd;


		//三角形の点がどちらに偏ってるかを調べる
		if (tri.vertFB.v1 == 1)
		{
			fVert.push_back(tri.vertData.v1);
		}
		else
		{
			bVert.push_back(tri.vertData.v1);
		}
		if (tri.vertFB.v2 == 1)
		{
			fVert.push_back(tri.vertData.v2);
		}
		else
		{
			bVert.push_back(tri.vertData.v2);
		}
		if (tri.vertFB.v3 == 1)
		{
			fVert.push_back(tri.vertData.v3);
		}
		else
		{
			bVert.push_back(tri.vertData.v3);
			
		}
		
		//片方の場合(多角形の面形成いらない場合)
		if(fVert.size() == 1)
		{
			frontVertices.push_back(fVert[0]);

			if (tri.hitResult.v1)
			{
				frontVertices.push_back(tri.hitPosVert.v1);
				fVert.push_back(tri.hitPosVert.v1);
			}
			if (tri.hitResult.v2)
			{
				frontVertices.push_back(tri.hitPosVert.v2);
				fVert.push_back(tri.hitPosVert.v2);
			}
			if (tri.hitResult.v3)
			{
				frontVertices.push_back(tri.hitPosVert.v3);
				fVert.push_back(tri.hitPosVert.v3);
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(fVert[0].pos, fVert[1].pos, fVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				frontIndices.push_back(frontIndex);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex + 2);
			}
			else//法線が逆だったら、並びを逆にして格納
			{
				frontIndices.push_back(frontIndex + 2);
				frontIndices.push_back(frontIndex + 1);
				frontIndices.push_back(frontIndex);
			}

			frontIndex += 3;
		}
		else
		{
			frontVertices.push_back(fVert[0]);

			int fVertVNum = 0;
			//fVert[0]が含まれててかつ線分が平面に当たってたら衝突点を格納
			if (tri.hitResult.v1) 
			{
				if (tri.segmentData.v1.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == fVert[0].pos)
				{
					//fVert0Seg = 
					fVertVNum = 1;
					frontVertices.push_back(tri.hitPosVert.v1);
				}
			}
			else if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 2;
					frontVertices.push_back(tri.hitPosVert.v2);
				}
			}
			else if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == fVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == fVert[0].pos)
				{
					fVertVNum = 3;
					frontVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//もう一個の衝突点と残りの頂点を格納
			if (tri.hitResult.v1 && fVertVNum != 1)
			{
				frontVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && fVertVNum != 2)
			{
				frontVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && fVertVNum != 3)
			{
				frontVertices.push_back(tri.hitPosVert.v3);
			}

			frontVertices.push_back(fVert[1]);



			//三角形を形成
			
			//fVertをクリアして、今回追加した三角形を追加
			fVert.clear();
			fVert.resize(4);
			for (int i = frontIndex; i < frontIndex + 4; i++)
			{
				fVert[i - frontIndex] = frontVertices[i];
			}
			 
			//n多角形の三角形分割を利用してインデックスを求める
			//原点から一番遠い頂点
			FbxVertex farVertex;

			//一番遠い頂点の座標
			float farVertDis = 0.0f;

			//farVertexの添え字(形成後の削除用)
			int farVertIndex = 0;

			//表
			while (1)
			{
				//三角形内部に頂点があるかないかの確認実装しといてね

				//一番遠い頂点を求める
				for (int i = 0, size = fVert.size(); i < size; i++)
				{
					float dis = Vector3(fVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = fVert[i];
						farVertDis = dis;
						farVertIndex = i;
					}
				}

				//10/11 ここ書き換えないといけない(距離じゃなくて配列のインデックスから求める)
				//隣の頂点を求める
				int farAddIndex = farVertIndex + 1;
				if (farAddIndex >= fVert.size())farAddIndex = 0;
				int farSubIndex = farVertIndex - 1;
				if (farSubIndex <= 0)farSubIndex = fVert.size() - 1;

				


				//三角形を形成

				//求めた外積と、面の外積が一致したら計算終了
				//三角形の外積
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(fVert[farSubIndex].pos,farVertex.pos, fVert[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					frontIndices.push_back(frontIndex + farSubIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farAddIndex);
				}
				else//法線が逆だったら、逆にして格納 2,1,far
				{
					frontIndices.push_back(frontIndex + farAddIndex);
					frontIndices.push_back(frontIndex + farVertIndex);
					frontIndices.push_back(frontIndex + farSubIndex);
				}

				//原点から一番遠い点を削除
				fVert.erase(fVert.begin() + farVertIndex);

				//形成不可になったら終了
				if (fVert.size() == 2)break;

			}
			frontIndex += 6;

		}

		if (bVert.size() == 1)
		{
			backVertices.push_back(bVert[0]);

			if (tri.hitResult.v1)
			{
				backVertices.push_back(tri.hitPosVert.v1);
				bVert.push_back(tri.hitPosVert.v1);
			}
			if (tri.hitResult.v2)
			{
				backVertices.push_back(tri.hitPosVert.v2);
				bVert.push_back(tri.hitPosVert.v2);
			}
			if (tri.hitResult.v3)
			{
				backVertices.push_back(tri.hitPosVert.v3);
				bVert.push_back(tri.hitPosVert.v3);
			}

			Vector3 cross;
			cross = LibMath::CalcNormal(bVert[0].pos, bVert[1].pos, bVert[2].pos);
			if (Vector3(tri.vertData.v1.normal) == cross)
			{
				backIndices.push_back(backIndex);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex + 2);
			}
			else//法線が逆だったら、並びを逆にして格納
			{
				backIndices.push_back(backIndex + 2);
				backIndices.push_back(backIndex + 1);
				backIndices.push_back(backIndex);
			}

			backIndex += 3;
		}
		else
		{
			backVertices.push_back(bVert[0]);

			int bVertVNum = 0;
			//bVert[0]が含まれててかつ線分が平面に当たってたら衝突点を格納
			if (tri.hitResult.v1)
			{
				if (tri.segmentData.v1.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v1.GetPosition().v2 == bVert[0].pos)
				{
					//bVert0Seg = 
					bVertVNum = 1;
					backVertices.push_back(tri.hitPosVert.v1);
				}
			}
			else if (tri.hitResult.v2)
			{
				if (tri.segmentData.v2.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v2.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 2;
					backVertices.push_back(tri.hitPosVert.v2);
				}
			}
			else if (tri.hitResult.v3)
			{
				if (tri.segmentData.v3.GetPosition().v1 == bVert[0].pos
					|| tri.segmentData.v3.GetPosition().v2 == bVert[0].pos)
				{
					bVertVNum = 3;
					backVertices.push_back(tri.hitPosVert.v3);
				}
			}

			//もう一個の衝突点と残りの頂点を格納
			if (tri.hitResult.v1 && bVertVNum != 1)
			{
				backVertices.push_back(tri.hitPosVert.v1);
			}
			else if (tri.hitResult.v2 && bVertVNum != 2)
			{
				backVertices.push_back(tri.hitPosVert.v2);
			}
			else if (tri.hitResult.v3 && bVertVNum != 3)
			{
				backVertices.push_back(tri.hitPosVert.v3);
			}

			backVertices.push_back(bVert[1]);



			//三角形を形成

			//bVertをクリアして、今回追加した三角形を追加
			bVert.clear();
			bVert.resize(4);
			for (int i = backIndex; i < backIndex + 4; i++)
			{
				bVert[i - backIndex] = backVertices[i];
			}

			//n多角形の三角形分割を利用してインデックスを求める
			//原点から一番遠い頂点
			FbxVertex farVertex;

			//一番遠い頂点の座標
			float farVertDis = 0.0f;

			//farVertexの添え字(形成後の削除用)
			int farVertIndex = 0;

			//裏
			while (1)
			{
				//三角形内部に頂点があるかないかの確認実装しといてね

				//一番遠い頂点を求める
				for (int i = 0, size = bVert.size(); i < size; i++)
				{
					float dis = Vector3(bVert[i].pos).Length();
					if (farVertDis <= dis)
					{
						farVertex = bVert[i];
						farVertDis = dis;
						farVertIndex = i;
					}
				}

				//10/11 ここ書き換えないといけない(距離じゃなくて配列のインデックスから求める)
				//隣の頂点を求める
				int farAddIndex = farVertIndex + 1;
				if (farAddIndex >= bVert.size())farAddIndex = 0;
				int farSubIndex = farVertIndex - 1;
				if (farSubIndex <= 0)farSubIndex = bVert.size() - 1;




				//三角形を形成

				//求めた外積と、面の外積が一致したら計算終了
				//三角形の外積
				Vector3 cross;

				//sub,far,add
				cross = LibMath::CalcNormal(bVert[farSubIndex].pos, farVertex.pos, bVert[farAddIndex].pos);
				if (Vector3(farVertex.normal) == cross)
				{
					backIndices.push_back(backIndex + farSubIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farAddIndex);
				}
				else//法線が逆だったら、逆にして格納 2,1,far
				{
					backIndices.push_back(backIndex + farAddIndex);
					backIndices.push_back(backIndex + farVertIndex);
					backIndices.push_back(backIndex + farSubIndex);
				}

				//原点から一番遠い点を削除
				bVert.erase(bVert.begin() + farVertIndex);

				//形成不可になったら終了
				if (bVert.size() == 2)break;

			}
			backIndex += 6;

		}


#pragma region 旧



		//平面と辺が当たっていたら衝突点を格納
		//if (tri.hitResult.v1)
		//{
		//	frontVertices.push_back(tri.hitPosVert.v1);
		//	fVert.push_back(tri.hitPosVert.v1);
		//	fVertInd.push_back(frontIndex);
		//	frontIndex++;

		//	backVertices.push_back(tri.hitPosVert.v1);
		//	bVert.push_back(tri.hitPosVert.v1);
		//	bVertInd.push_back(backIndex);
		//	backIndex++;

		//}
		//if (tri.hitResult.v2)
		//{
		//	frontVertices.push_back(tri.hitPosVert.v2);
		//	fVert.push_back(tri.hitPosVert.v2);
		//	fVertInd.push_back(frontIndex);
		//	frontIndex++;

		//	backVertices.push_back(tri.hitPosVert.v2);
		//	bVert.push_back(tri.hitPosVert.v2);
		//	bVertInd.push_back(backIndex);
		//	backIndex++;
		//}
		//if (tri.hitResult.v3)
		//{
		//	frontVertices.push_back(tri.hitPosVert.v3);
		//	fVert.push_back(tri.hitPosVert.v3);
		//	fVertInd.push_back(frontIndex);
		//	frontIndex++;

		//	backVertices.push_back(tri.hitPosVert.v3);
		//	bVert.push_back(tri.hitPosVert.v3);
		//	bVertInd.push_back(backIndex);
		//	backIndex++;
		//}

		////三角形の頂点を格納するついでに、衝突点も格納するようにする
		//if (tri.vertFB.v1 == 1)
		//{
		//	frontVertices.push_back(tri.vertData.v1);

		//	//tri.vertData.v1が含まれてるかつ、切断
		//}
		//else
		//{
		//	backVertices.push_back(tri.vertData.v1);
		//	bVert.push_back(tri.vertData.v1);

		//	bVertInd.push_back(backIndex);
		//	backIndex += 2;
		//}
		//if (tri.vertFB.v2 == 1)
		//{
		//	frontVertices.push_back(tri.vertData.v2);
		//	fVert.push_back(tri.vertData.v2);

		//	fVertInd.push_back(frontIndex);
		//	frontIndex += 2;
		//}
		//else
		//{
		//	backVertices.push_back(tri.vertData.v2);
		//	bVert.push_back(tri.vertData.v2);

		//	bVertInd.push_back(backIndex);

		//	backIndex += 2;
		//}
		//if (tri.vertFB.v3 == 1)
		//{
		//	frontVertices.push_back(tri.vertData.v3);
		//	fVert.push_back(tri.vertData.v3);

		//	fVertInd.push_back(frontIndex);
		//	frontIndex += 2;
		//}
		//else
		//{
		//	backVertices.push_back(tri.vertData.v3);
		//	bVert.push_back(tri.vertData.v3);

		//	bVertInd.push_back(backIndex);

		//	backIndex += 2;
		//}

#pragma endregion

#pragma region 変更前



		//////三角形の頂点を格納
		////if (tri.vertFB.v1 == 1)
		////{
		////	frontVertices.push_back(tri.vertData.v1);
		////	fVert.push_back(tri.vertData.v1);

		////	fVertInd.push_back(frontIndex);
		////	frontIndex += 2;
		////}
		////else
		////{
		////	backVertices.push_back(tri.vertData.v1);
		////	bVert.push_back(tri.vertData.v1);

		////	bVertInd.push_back(backIndex);
		////	backIndex+= 2;
		////}
		////if (tri.vertFB.v2 == 1)
		////{
		////	frontVertices.push_back(tri.vertData.v2);
		////	fVert.push_back(tri.vertData.v2);

		////	fVertInd.push_back(frontIndex);
		////	frontIndex += 2;
		////}
		////else
		////{
		////	backVertices.push_back(tri.vertData.v2);
		////	bVert.push_back(tri.vertData.v2);

		////	bVertInd.push_back(backIndex);

		////	backIndex += 2;
		////}
		////if (tri.vertFB.v3 == 1)
		////{
		////	frontVertices.push_back(tri.vertData.v3);
		////	fVert.push_back(tri.vertData.v3); 
		////	
		////	fVertInd.push_back(frontIndex);
		////	frontIndex += 2;
		////}
		////else
		////{
		////	backVertices.push_back(tri.vertData.v3);
		////	bVert.push_back(tri.vertData.v3);

		////	bVertInd.push_back(backIndex);

		////	backIndex += 2;
		////}


		//////平面と辺が当たっていたら衝突点を格納
		////if (tri.hitResult.v1)
		////{
		////	frontVertices.push_back(tri.hitPosVert.v1);
		////	fVert.push_back(tri.hitPosVert.v1);
		////	fVertInd.push_back(frontIndex);
		////	frontIndex++;

		////	backVertices.push_back(tri.hitPosVert.v1);
		////	bVert.push_back(tri.hitPosVert.v1);
		////	bVertInd.push_back(backIndex);
		////	backIndex++;

		////}
		////if (tri.hitResult.v2)
		////{
		////	frontVertices.push_back(tri.hitPosVert.v2);
		////	fVert.push_back(tri.hitPosVert.v2);
		////	fVertInd.push_back(frontIndex);
		////	frontIndex++;

		////	backVertices.push_back(tri.hitPosVert.v2);
		////	bVert.push_back(tri.hitPosVert.v2);
		////	bVertInd.push_back(backIndex);
		////	backIndex++;
		////}
		////if (tri.hitResult.v3)
		////{
		////	frontVertices.push_back(tri.hitPosVert.v3);
		////	fVert.push_back(tri.hitPosVert.v3);
		////	fVertInd.push_back(frontIndex);
		////	frontIndex++;

		////	backVertices.push_back(tri.hitPosVert.v3);
		////	bVert.push_back(tri.hitPosVert.v3);
		////	bVertInd.push_back(backIndex);
		////	backIndex++;
		////}


		////n多角形の三角形分割を利用して計算
		////表

		////原点から一番遠い頂点
		//FbxVertex farVertex;

		////一番遠い頂点の座標
		//float farVertDis = 0.0f;

		////farVertexの添え字(形成後の削除用)
		//int farVertIndex = 0;

		////ferから1、2番目に近い頂点を求める為のデータ格納用変数
		//float farFirDir = FLT_MAX;
		//FbxVertex farFirVertex;
		//int farFirVertIndex = 0;

		//float farSecDir = FLT_MAX;
		//FbxVertex farSecVertex;
		//int farSecVertIndex = 0;

		////表
		//while (1)
		//{
		//	//三角形内部に頂点があるかないかの確認実装しといてね

		//	//一番遠い頂点を求める
		//	for (int i = 0, size = fVert.size(); i < size; i++)
		//	{
		//		float dis = Vector3(fVert[i].pos).Length();
		//		if (farVertDis <= dis)
		//		{
		//			farVertex = fVert[i];
		//			farVertDis = dis;
		//			farVertIndex = i;
		//		}
		//	}

		//	//farから1、2番目に近い頂点(隣の頂点)を求める
		//	for (int i = 0, size = fVert.size(); i < size; i++)
		//	{
		//		//自分比較防止
		//		if (i == farVertIndex)continue;

		//		float dis = Vector3(farVertex.pos - fVert[i].pos).Length();
		//		if (farFirDir >= dis)
		//		{
		//			farSecVertex = farFirVertex;
		//			farSecDir = farFirDir;
		//			farSecVertIndex = farFirVertIndex;

		//			farFirVertex = fVert[i];
		//			farFirDir = dis;
		//			farFirVertIndex = i;
		//		}
		//		else if (farSecDir >= dis)
		//		{
		//			farSecVertex = fVert[i];
		//			farSecDir = dis;
		//			farSecVertIndex = i;
		//		}
		//	}


		//	//三角形を形成

		//	//求めた外積と、面の外積が一致したら計算終了
		//	//三角形の外積
		//	Vector3 cross;

		//	//far,1,2
		//	cross = LibMath::CalcNormal(farVertex.pos, farFirVertex.pos, farSecVertex.pos);
		//	if(Vector3(farVertex.normal) == cross)
		//	{
		//		frontIndices.push_back(fVertInd[farVertIndex]);
		//		frontIndices.push_back(fVertInd[farFirVertIndex]);
		//		frontIndices.push_back(fVertInd[farSecVertIndex]);
		//	}
		//	else//法線が逆だったら、逆にして格納 2,1,far
		//	{
		//		frontIndices.push_back(fVertInd[farSecVertIndex]);
		//		frontIndices.push_back(fVertInd[farFirVertIndex]);
		//		frontIndices.push_back(fVertInd[farVertIndex]);
		//	}

		//	//原点から一番遠い点を削除
		//	fVert.erase(fVert.begin() + farVertIndex);

		//	//形成不可になったら終了
		//	if (fVert.size() == 2)break;
		//	
		//}

		////裏
		//while(1)
		//{//三角形内部に頂点があるかないかの確認実装しといてね

		//	//一番遠い頂点を求める
		//	for (int i = 0, size = bVert.size(); i < size; i++)
		//	{
		//		float dis = Vector3(bVert[i].pos).Length();
		//		if (farVertDis <= dis)
		//		{
		//			farVertex = bVert[i];
		//			farVertDis = dis;
		//			farVertIndex = i;
		//		}
		//	}

		//	//farから1、2番目に近い頂点(隣の頂点)を求める
		//	farFirDir = FLT_MAX;
		//	farSecDir = FLT_MAX;
		//	for (int i = 0, size = bVert.size(); i < size; i++)
		//	{
		//		//自分比較防止
		//		if (i == farVertIndex)continue;

		//		float dis = Vector3(farVertex.pos - bVert[i].pos).Length();
		//		if (farFirDir >= dis)
		//		{
		//			farSecVertex = farFirVertex;
		//			farSecDir = farFirDir;
		//			farSecVertIndex = farFirVertIndex;

		//			farFirVertex = bVert[i];
		//			farFirDir = dis;
		//			farFirVertIndex = i;
		//		}
		//		else if (farSecDir >= dis)
		//		{
		//			farSecVertex = bVert[i];
		//			farSecDir = dis;
		//			farSecVertIndex = i;
		//		}
		//	}


		//	//三角形を形成

		//	//求めた外積と、面の外積が一致したら計算終了
		//	//三角形の外積
		//	Vector3 cross;

		//	//far,1,2
		//	cross = LibMath::CalcNormal(farVertex.pos, farFirVertex.pos, farSecVertex.pos);
		//	if (Vector3(farVertex.normal) == cross)
		//	{
		//		backIndices.push_back(bVertInd[farVertIndex]);
		//		backIndices.push_back(bVertInd[farFirVertIndex]);
		//		backIndices.push_back(bVertInd[farSecVertIndex]);
		//	}
		//	else//法線が逆だったら、逆にして格納 2,1,far
		//	{
		//		backIndices.push_back(bVertInd[farSecVertIndex]);
		//		backIndices.push_back(bVertInd[farFirVertIndex]);
		//		backIndices.push_back(bVertInd[farVertIndex]);
		//	}

		//	//原点から一番遠い点を削除
		//	bVert.erase(bVert.begin() + farVertIndex);

		//	//形成不可になったら終了
		//	if (bVert.size() == 2)break;
		//}

#pragma endregion

	}

	//ここで、断面の情報をセットしていく

	//if (!pFront)
	//{
	//	//頂点とインデックスを元にバッファを作成&更新(Map)
	//	//バッファ作成よりモデルデータを作成する感じにする?
	//	std::vector<std::vector<FbxVertex>> vert(1, frontVertices);
	//	std::vector<std::vector<USHORT>> ind(1, frontIndices);
	//	catFrontModelData = std::make_unique<ModelData>();
	//	catFrontModelData->Create(vert, ind);
	//	pFront = catFrontModelData.get();
	//}
	//if (!pBack) 
	//{
	//	std::vector<std::vector<FbxVertex>> vert(1, backVertices);
	//	std::vector<std::vector<USHORT>> ind(1, backIndices);
	//	catBackModelData = std::make_unique<ModelData>();
	//	catBackModelData->Create(vert, ind);
	//	pBack = catBackModelData.get();
	//}
	//// 全頂点の表裏判定
	//std::vector<std::vector<Vector3>> vertices = pModelData->GetVerticesPosition();
	//size_t size = vertices[0].size();
	//std::vector<char>verticesFB(size);
	//
	//for (int i = 0; i < size; i++)
	//{
	//	verticesFB[i] = LibMath::PointPlaneFrontBackCheck(vertices[0][i] * modelConstDatas[0].scale + modelConstDatas[0].position,planeData);
	//}

	////全辺を計算
	//std::vector<Segment3DData>sDatas;
	//std::vector<bool>sDatasHit;
	//std::vector<Vector3>sDatasHitPos;
	////大体確保
	//sDatas.reserve(size / 2);

	//std::vector<std::vector<USHORT>>indices = pModelData->GetIndices();
	//size_t iSize = indices.size();
	//for(int i = 0; i < iSize;i+= 3)
	//{
	//	Segment3DData sData;
	//	sData.SetPosition(Value2<Vector3>(vertices[0][indices[0][i]], vertices[0][indices[0][i + 1]]));
	//	sDatas.push_back(sData);

	//	sData.SetPosition(Value2<Vector3>(vertices[0][indices[0][i + 1]], vertices[0][indices[0][i + 2]]));
	//	sDatas.push_back(sData);

	//	sData.SetPosition(Value2<Vector3>(vertices[0][indices[0][i + 2]], vertices[0][indices[0][i]]));
	//	sDatas.push_back(sData);
	//}

	////判定
	//size_t sDatasSize = sDatas.size();
	//sDatasHit.resize(sDatasSize);
	//sDatasHitPos.resize(sDatasSize, FLT_MAX);
	//for (int i = 0; i < sDatasSize; i++)
	//{
	//	Segment3DCalcResult result;
	//	sDatasHit[i] = Collision::PlaneAndSegment3D(planeData, sDatas[i], &result);
	//	sDatasHitPos[i] = result.planeHitPos;
	//}


	//やっぱn多角形の三角形分割形成で行ける
	//面ごとにちゃんと指定すれば


	//int z = 0;
}

void ModelObject::SetPosition(const Vector3& position)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].position = position.ToXMFLOAT3();
	}
}

void ModelObject::SetScale(const Vector3& scale)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].scale = scale.ToXMFLOAT3();
		if (modelConstDatas[i].scale.x == 0.0f)modelConstDatas[i].scale.x = 0.0001f;
		if (modelConstDatas[i].scale.y == 0.0f)modelConstDatas[i].scale.y = 0.0001f;
		if (modelConstDatas[i].scale.z == 0.0f)modelConstDatas[i].scale.z = 0.0001f;
	}
}

void ModelObject::SetAngle(const Vector3& angle)
{
	for (int i = 0; i < modelFileObjectNum; i++)
	{
		modelConstDatas[i].angle = angle.ToXMFLOAT3();
	}
}


bool ModelObject::Initialize(ID3D12Device* dev, const std::vector<ID3D12GraphicsCommandList*>& cmdList)
{
	device = dev;
	cmdLists = cmdList;


#pragma region ディスクリプタレンジ_ルートパラメーター
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparam[6] = {};

	//行列や色など
	rootparam[CONST_BUFFER_REGISTER].InitAsConstantBufferView(CONST_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//ユーザー定数
	rootparam[USER_BUFFER_REGISTER].InitAsConstantBufferView(USER_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);
	//マテリアル
	rootparam[MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//PBRマテリアル
	rootparam[COLOR_MATERIAL_BUFFER_REGISTER].InitAsConstantBufferView(COLOR_MATERIAL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_PIXEL);
	//モデルごと
	rootparam[MODEL_BUFFER_REGISTER].InitAsConstantBufferView(MODEL_BUFFER_REGISTER, 0, D3D12_SHADER_VISIBILITY_ALL);

	//テクスチャ
	rootparam[TEXURE_ROOTPARAM_NUM].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_PIXEL);

#pragma endregion

#pragma region ルートシグネチャ


	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

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

	ComPtr<ID3DBlob>rootSigBlob;
	ComPtr<ID3DBlob>errorBlob;

	auto result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);


	if (FAILED(result))
	{
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	result = device->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

#pragma endregion

	PipelineState::SetModelRootSignature(rootSignature.Get());




}

void MelLib::ModelObject::SetMaterial(Material* mtl, const UINT index)
{
	if (!mtl)
	{
#ifdef _DEBUG
		OutputDebugString(L"マテリアルのセットに失敗しました。マテリアルがnullptrです。\n");
#endif // _DEBUG
		return;
	}
	if (index >= materials.size())
	{
#ifdef _DEBUG
		OutputDebugString(L"マテリアルのセットに失敗しました。indexの値が大きくてセットできません。\n");
#endif // _DEBUG
		return;
	}

	materials[index] = mtl;
}



bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData, const std::string& name)
{
	if (!pModelData)
	{
#ifdef _DEBUG

		OutputDebugStringA(name.data());
		OutputDebugStringW(L"の生成に失敗しました。\n");
#endif // _DEBUG
		return false;
	}

	pModelObjects.emplace(name, std::make_unique<ModelObject>(pModelData, userConstBufferData));


	return true;
}

void ModelObject::Delete(const std::string& name)
{
	pModelObjects.erase(name);
}

void ModelObject::ResetAnimation()
{
	fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
}

void ModelObject::SetCurrentFream(const UINT fream)
{
	FbxTime setTime = fbxAnimationData.animationTimes.startTime * fream;
	if (setTime > fbxAnimationData.animationTimes.endTime)
		setTime = fbxAnimationData.animationTimes.endTime;

	fbxAnimationData.currentTime = setTime;
}



void ModelObject::FbxAnimation()
{
	if (!isAnimation)return;

	//タイムを進める
	fbxAnimationData.currentTime += fbxAnimationData.animationTimes.freamTime * fbxAnimationData.timeMag;

	if (fbxAnimationData.currentTime > fbxAnimationData.animationTimes.endTime)
		fbxAnimationData.currentTime = fbxAnimationData.animationTimes.startTime;
	if (fbxAnimationData.currentTime < fbxAnimationData.animationTimes.startTime)
		fbxAnimationData.currentTime = fbxAnimationData.animationTimes.endTime;
}

bool ModelObject::Create(ModelData* pModelData, ConstBufferData* userConstBufferData)
{
	if (!pModelData)
	{
#ifdef _DEBUG
		OutputDebugStringW(L"pModelDataがnullptrです。生成に失敗しました\n");
#endif // _DEBUG
		return false;
	}

#pragma region データセット
	if (userConstBufferData)this->userConstBufferData = *userConstBufferData;

	//objのボーンのMoveVectorの割る処理のために、オブジェクトごとにバッファ作成
	if (pModelData->GetModelFormat() == ModelData::ModelFormat::MODEL_FORMAT_OBJ)
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL_OBJECT;
	}
	else
	{
		modelConstBufferData.bufferType = ConstBufferData::BufferType::BUFFER_TYPE_EACH_MODEL;
	}
	modelConstBufferData.bufferDataSize = sizeof(SkinConstBufferData);

	modelFileObjectNum = pModelData->GetModelFileObjectNumber();
	this->pModelData = pModelData;

#pragma endregion

	CreateConstBuffer();

	modelConstDatas.resize(modelFileObjectNum);


	//マテリアル取得
	std::vector<ADSAMaterial*>modelDataMtl = pModelData->GetPMaterial();

	size_t size = modelDataMtl.size();
	materials.resize(size);
	for (int i = 0; i < size; i++)
	{
		materials[i] = modelDataMtl[i];
	}

#pragma region アニメーション関係


	boneDatas.resize(pModelData->GetBoneNumber());
	parentBoneDatas.resize(pModelData->GetBoneNumber());
	SkinConstBufferData* skinConstData = nullptr;
	modelConstBuffer[0]->Map(0, nullptr, (void**)&skinConstData);
	for (int i = 0; i < BONE_MAX; i++)
	{
		skinConstData->bones[i] = DirectX::XMMatrixIdentity();
	}
	modelConstBuffer[0]->Unmap(0, nullptr);

	fbxAnimationData.animationTimes = pModelData->GetFbxAnimationTimes();
#pragma endregion



	return true;
}



std::vector<std::vector<Vector3>> MelLib::ModelObject::GetVerticesData(const bool scaleImpact, const bool angleImpact, const bool transformImpact)
{
	std::vector<std::vector<Vector3>>verticesPos = pModelData->GetVerticesPosition();

	for (int i = 0, size = verticesPos.size(); i < size; i++)
	{
		for (auto& pos : verticesPos[i])
		{
			Matrix mat = Matrix::GetTranslationMatrix(pos);
			if (scaleImpact) mat *= Matrix::GetScalingMatrix(modelConstDatas[i].scale);
			if (angleImpact)mat *= Matrix::GetRotateZXYMatrix(modelConstDatas[i].angle);
			if (transformImpact)mat *= Matrix::GetTranslationMatrix(modelConstDatas[i].position);
			pos = Vector3(mat[3][0], mat[3][1], mat[3][2]);
		}
	}
	return verticesPos;

}