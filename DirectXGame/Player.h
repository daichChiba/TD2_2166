#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;

class Player {
public:
	/// <summary>
	/// 初期化
	/// <summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, Camera* camera_, const Vector3& pos);

	/// <summary>
	/// 更新
	/// <summary>
	void Update();


	/// <summary>
	/// 描画
	/// <summary>
	void Draw();

	private:
		// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ビュープロジェクション
	Camera* camera_ = nullptr;
};
