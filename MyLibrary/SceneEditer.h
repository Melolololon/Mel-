#pragma once

#include"GameObject.h"

namespace MelLib 
{
	// objectsのオブジェクトをどうマネージャーに追加する?

	// 保存するごとに自動バックアップとるようにしたり、
	// 履歴保存してCtrl+Zで戻れるようにした方が良いか

	// オブジェクト一覧はエディターの機能なのでこちらに実装していい
	// オブジェクトマネージャーに実装すると向こうにもオンオフフラグ用意しないといけないし利用者もややこしくなる(こっちのフラグ参照してもいいけど)
	// オブジェクトマネージャーにオブジェクト取得する関数作って、選ばれたオブジェクトを取得してそれに応じて描画するパラメーターのウィンドウを変える
	// GUIValueManagerにウィンドウの表示フラグをセットする関数作る
	// GameObjectクラスでそのフラグをセットできるようにする
	// それにより、シーンエディターで選ばれたもののみ表示することができる

	// 今選ばれてるオブジェクトのウィンドウを非表示
	// 新しいオブジェクトを現在選択されているオブジェクトを入れる変数に追加
	// 新しいオブジェクトのウィンドウを表示

	// 追加したオブジェクトを登録できるようにする
	// 登録するときに作った登録関数を使えば楽

	class SceneEditer
	{

	private:
		SceneEditer(){}
		~SceneEditer(){}

		SceneEditer(SceneEditer& s) = delete;
		SceneEditer& operator=(SceneEditer& s) = delete;


		void Save();
		void SaveEditData();
		void InputEditDataName();
		void SaveRegisterObject();

		void Load();

		void UpdateSelectObject();

		void DrawObjectList();

		void SetDrawWindowFlag(const std::vector<std::string>& objNames);

		void RegisterSelectObject();
		void InputObjectName();
		void InputObjectType();

		std::string GetObjectType(const GameObject& object)const;
	private:
		static const std::string EDIT_DATA_FORMAT;
		static const std::string REGISTER_OBJECT_DATA_FORMAT;

		// 順序保持しとけばラジオボタンの値分forで回してデータにアクセスできる(intでアクセスできる)からmap
		// 順序を他の配列に保存するのもいいかも。ループする必要なくなるから早い。けどメモリ使う
		// forで取得すると参照に代入できないし、vectorのポインタは個人的に使いたくないし、forでしか参照できないから分ける
		// ラジオボタンの順番めちゃくちゃになるからmapに変更
		// mapはstringだと名前順に並べてくれる
		// オブジェクト
		std::map< std::string,std::map<std::string,std::shared_ptr<MelLib::GameObject>>>pRegisterObjects;
		// ラジオボタンの戻り値で上のobjectsにアクセスするための配列
		// ラジオボタンの戻り値でこの配列からpObjectsのキーを取り出したり、pObjectsのvectorにアクセスする
		std::unordered_map<std::string,std::unordered_map<int ,std::string>>registerObjectOrderDatas;
		std::vector<std::string>registerObjectTypes;
		std::vector<std::string>registerObjectNames;
		 
		// 選択されているタイプ
		int selectType = 0;

		GameObject* pEditSelectObject = nullptr;

		bool isEdit = false;
		bool releaseEdit = false;

		int selectListObjectNum = 0;
		std::string selectListObjectName;

		bool inpttingObjectName = false;
		bool inpttingObjectType = false;
		bool inpttingEditDataName = false;

		std::string inputObjectName;
		std::string inputObjectType;
		std::string inputEditDataName;
	public:
		static SceneEditer* GetInstance();


		/// <summary>
		/// オブジェクトをエディタに登録します。
		/// </summary>
		/// <param name="object">オブジェクト</param>
		/// <param name="objectType">オブジェクトの種類(ActorやStageObjectなど)</param>
		void RegisterObject(const std::shared_ptr<MelLib::GameObject>& pObject,const std::string& objectType);
		
		void Update();
		void Draw();

		/// <summary>
		/// エディタのオンオフを設定します。リリースビルド時にエディタをオンにできるようにしてない場合はオンにできません。
		/// </summary>
		/// <param name="flag"></param>
		void SetEditFlag(const bool flag);
		
		/// <summary>
		/// リリースビルド時にエディタをオンにできるようにするかどうかを設定します。
		/// </summary>
		/// <param name="flag"></param>
		void SetReleaseEditFlag(const bool flag) { releaseEdit = flag; }
	};
}
