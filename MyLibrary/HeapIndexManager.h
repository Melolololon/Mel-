#pragma once
#include<vector>
#include<string>
class HeapIndexManager
{
private:
	int createNum;

	//どの番号のモデルが描画されてるか(残ってるか)を確認するための配列
	std::vector<bool>drawCheckVector;
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="modelNum">画面に表示するモデルの最大数</param>
	HeapIndexManager(const int maxModelNum);
	~HeapIndexManager();

	/// <summary>
	/// ヒープのインデックスを取得します。
	/// </summary>
	/// <returns></returns>
	int GetHeapIndex();

	/// <summary>
	/// モデルの描画が終了するとき、(クラスのデストラクタなど)で必ず呼び出してください。
	/// </summary>
	/// <param name="heapIndex"></param>
	void DrawEndCallFunction(const int heapIndex);

};

