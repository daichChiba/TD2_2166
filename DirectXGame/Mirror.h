#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include "AABB.h"
#include "MapChipField.h"
#include "MathUtilityForText.h"
class Player;
class Botton;

class Mirror {

public:
	/// <summary>
	/// 初期化
	/// <summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, Camera* camera, const Vector3& pos);

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void InputMove();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	//// 衝突応答
	void OnCollision(const Botton* botton, bool isPlayerHit);

	// スイッチと当たったか
	bool GetIsHit() { return isHit_; }

	bool IsAlive() { return isAive; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ビュープロジェクション
	Camera* camera_ = nullptr;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//
	Vector3 velocity_ = {0.1f, 0.1f, 0.1f};

	// 旋回開始の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	//	キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 2.0f;

	static inline const float kAttenuationWall = 0.1f;

	bool isAive = true;

	bool isHit_ = false;
};
