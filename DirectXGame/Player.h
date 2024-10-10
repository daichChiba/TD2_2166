#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include "AABB.h"
#include "MapChipField.h"
class MapChipField;

enum class LRDirection { kRight, kLeft };

// マップとの当たり判定情報
struct CollisionMapInfo {
	bool isCeiling = false;
	bool isLanding = false;
	bool IsHitWall = false;
	Vector3 velocity;
};

// 角
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

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

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	LRDirection lrDirection_ = LRDirection::kRight;


	void InputMove();


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

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.01f;
	static inline const float kAttenuation = 0.005f;
	static inline const float kLimitRunSpeed = 0.5f;

	// 旋回開始の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 設置状況フラグ
	bool onGround_ = true;
};
