#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include"AABB.h"
class Player;
class Mirror;

class Botton {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& pos);
	void Update();
	void Draw();

	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	//// 衝突応答
	void OnCollision(const Player* player);
	//// 衝突応答
	void OnCollision(const Mirror* mirror);



	// std::list<Enemy*> enemies_;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;
	// ビュープロジェクション
	Camera* camera_ = nullptr;

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.01f;

	// 速度
	Vector3 velocity_ = {};

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 　最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 1.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 3.0f;

	// 経過時間
	float walkTimer_ = 0.0f;

	//	キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;


};
