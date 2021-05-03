#pragma once
#include"Model.h"
#include"Texture.h"

//objの頂点構造体や、ボーンセット関数を使いつつ、
//さらに頂点に用意したデータを持たせたい場合は、
//これを継承して、Objの頂点と持たせたいデータを混ぜた自作構造体を作り、
//それを使ってmapする
//そうすれば、データ足しつつ、ボーンセット関数、マップ関数を使うことができる
//値は、Objの頂点と追加データで配列を分けて管理してもらう

//これ継承しなくても自作頂点使えるようにできそう
//どっちにするか考える
//継承しないと、自作頂点をObjectに作らないといけないし、
//それを防ぐためにクラスとDrawなどの関数を作らないといけないし、
//しかもポリモーフィズムの恩恵を受けられない(他のモデルはできるのに!)
//だから継承のほうがいい?
//自作頂点のサイズセット関数作る?
//ObjModelやFbxModelの時はセットした数の影響を受けないようにする?

//ObjModelを使うときなどに自作構造体の定数バッファのマップを継承しなくてもできるようになっていて、
//データをまとめるにはクラス作らないといけないからどっちみちポリモーフィズムの恩恵を受けられない
//ポリモーフィズムの恩恵を受けたい場合は、Modelデータを所持したクラスに自分で親クラス作って継承してもらう?
//それだと既存のModelクラスと一緒にできない
//自分で親クラス作らずにModelやFbxObjectなどのクラスを継承すればポリモーフィズムの恩恵を受けれる?

//セット関数作るくらいなら、
//ロード関数にセット持たせたままでもいいかも
//とりあえずObjModelやFbxModelの時はセットした数の影響を受けないようにした
//セット関数作らないとプリミティブモデルの関数全部にセット関数作らないといけなくなる。
//自作定数バッファ構造体もセット関数作らないとそうなる
//定数は全部に実装しちゃってもいいかも

//継承しないと頂点とかを外部(Objectクラス)に載せることになるので、
//やっぱ継承したほうがよさそう

//一旦自作頂点保留する?

class ObjModel:public Model
{
private:

	static PipelineState defaultPipeline;

	std::string materialFileName;

	//頂点
	std::vector<std::vector<ObjAnimationVertex>> vertices;
	std::vector<std::vector<DirectX::XMFLOAT3>> smoothNormal;

	//[obj内のオブジェクト分]スムーズシェーディングの計算用データ
	std::vector< std::unordered_map < USHORT, std::vector<USHORT> >>smoothData;

	//[.obj内のオブジェクトごと][頂点ごと]
	std::vector<std::vector<int>> objBoneNums;//頂点に割り当てられているボーン番号
	//[モデルごと(keyでアクセス)][ボーンごと]
	std::vector<DirectX::XMFLOAT3> objBonePositions;//ボーン座標
	//[ボーンごと](オブジェクトごとに変える必要なさそうなので、こうしてる)

	//ボーン数
	UINT boneNum;
	//[モデルごと][ボーンごと]
	std::vector<std::vector<BoneData>>boneDatas;
	std::vector < std::vector<ParentBoneData>> parentBoneDatas;//親ボーンと影響度

	std::vector<std::unique_ptr<Texture>>textures;

	/// <summary>
/// objモデルの頂点を読み込みます。
/// </summary>
/// <param name="path">モデルのパス</param>
/// <param name="loadUV">uvを読み込むか</param>
/// <param name="loadNormal">normalを読み込むか</param>
/// <param name="verticesSize">シェーダーに送る利用者が用意した頂点データのバイト数(用意しない場合は0)</param>
	void LoadModelVertices
	(
		const std::string& path,
		const bool loadUV,
		const bool loadNormal,
		const size_t vertexSize
	);

	void LoadModelMaterial
	(
		const std::string& directryPath,
		const int createNum,
		const size_t constDataSize
	);


	void MapBoneMatrix(const int modelNum);

public:
	ObjModel();
	~ObjModel();


	/// <summary>
	/// .objモデルを読み込みます。
	/// </summary>
	/// <param name="path">パス</param>
	/// <param name="loadUV">UVを読み込むかどうか</param>
	/// <param name="createNum">生成数</param>
	///// <param name="vertexSize">シェーダーに送る利用者が用意した頂点データのバイト数(用意しない場合は0)</param>
	/// <param name="constDataSize">シェーダーに送る利用者が用意した定数データのバイト数(用意しない場合は0)</param>
	void LoadModel
	(
		const std::string& path,
		const bool loadUV,
		const int modelNum,
		//const size_t vertexSize,
		const size_t constDataSize
	);

#pragma region 操作


	void SetBoneMoveVector
	(
		const Vector3& vector,
		const int boneNum,
		const int modelNum
	)
	{
		boneDatas[modelNum][boneNum].moveVector = vector.ToXMFLOAT3();
	}

	void SetBoneScale
	(
		const Vector3& scale,
		const int boneNum,
		const int modelNum
	)
	{
		boneDatas[modelNum][boneNum].scale = scale.ToXMFLOAT3();
	}

	void SetBoneAngle
	(
		const Vector3& angle,
		const int boneNum,
		const int modelNum
	)
	{
		boneDatas[modelNum][boneNum].angle = angle.ToXMFLOAT3();
	}

	void SetParentBone
	(
		const int bone,
		const int parentBone,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][bone].parentBoneNum = parentBone;
	}

	/// <summary>
	/// 親のウィ協をどのくらい受けるかを設定します
	/// </summary>
	/// <param name="impact">倍率</param>
	/// <param name="boneNum"></param>
	/// <param name="modelNum"></param>
	void SetMoveVectorImpact
	(
		const Vector3& impact,
		const int boneNum,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][boneNum].moveVectorImpact = impact.ToXMFLOAT3();
	}


	void SetAngleImpact
	(
		const Vector3& impact,
		const int boneNum,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][boneNum].angleImpact = impact.ToXMFLOAT3();
	}

	void SetScaleImpact
	(
		const Vector3& impact,
		const int boneNum,
		const int modelNum
	)
	{
		parentBoneDatas[modelNum][boneNum].scaleImpact = impact.ToXMFLOAT3();
	}


#pragma endregion

#pragma region 情報取得
	std::vector<Vector3>GetBonePosition();
#pragma endregion


	static bool Initialize();
	void Draw(const int modelNum)override;

	static PipelineState GetDefaultPipeline() { return defaultPipeline; }
};

