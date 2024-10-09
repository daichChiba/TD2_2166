#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MapChipField.h"
#include "AABB.h"


class MapChipField;
class Enemy;

enum class LRDirection { 
	kRight,
	kLeft 
};

//マップとの当たり判定情報
struct CollisionMapInfo {
	bool isCeiling=false;
	bool isLanding=false;
	bool IsHitWall=false;
	Vector3 velocity;
};

//角
enum Corner {
	kRightBottom,	//右下
	kLeftBottom,	//左下
	kRightTop,		//右上
	kLeftTop,		//左上

	kNumCorner		//要素数
};

/// <summary>
/// 自キャラ
/// <summary>
class Player {

public:
	/// <summary>
	/// 初期化
	/// <summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos);

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw();

	LRDirection lrDirection_ = LRDirection::kRight;

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) {mapChipField_ = mapChipField; }

	void InputMove();

	void AnimateTurn();

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	void isHitCeiling(const CollisionMapInfo& info);
	
	void collisionHitMove(const CollisionMapInfo& info);

	void landingSwitch(const CollisionMapInfo& info);

	void isHitWall(const CollisionMapInfo& info);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision(const Enemy* enemy);

	//デスフラグのgetter
	bool IsDead() const { return isDead_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

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

	//　重力加速度（下方向）
	static inline const float kGravityAcceleration = { 0.05f };
	//　最大落下速度（下方向）
	static inline const float kLimitFallSpeed = { 0.5f };
	//　ジャンプ加速（上方向）
	static inline const float kJumpAcceleration = { 0.5f };

	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//	キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	static inline const float kBlank = 2.0f;

	static inline const float kAttenuationLanding = 0.005f;
	static inline const float landShift=0.1f;

	static inline const float kAttenuationWall = 0.1f;

	//デスフラグ
	bool isDead_ = false;
	
};
