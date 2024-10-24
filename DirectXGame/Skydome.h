#pragma once
#include <KamataEngine.h>

using namespace KamataEngine;
class Skydome {
public:
	//<summary>
	//初期化
	//</summary>
	void Initialize(Model* model, Camera* camera);

	//<summary>
	//更新
	//</summary>
	void Update();

	//<summary>
	//描画
	//</summary>
	void Draw();

	private:
		//ワールド変換データ
	    WorldTransform worldTransform_;
		//モデル
	    Model* model_ = nullptr;

		Model* modelSkydome_ = nullptr;
	    // テクスチャハンドル
	   
		Camera* camera_ = nullptr;
};
