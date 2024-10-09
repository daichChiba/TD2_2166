#include "Enemy.h"
#include "cassert"
#include <algorithm>
#include <numbers>
#include "MathUtilityForText.h"


void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);

	// データを引数に記録する
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
	viewProjection_ = viewProjection;

	//速度を制限する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;



}

void Enemy::Update() {

	//移動
	worldTransform_.translation_ += velocity_;

	walkTimer_ += 1.0f / 60.0f;

	//回転アニメーション
	worldTransform_.rotation_.x = std::sin(2 * std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	model_->Draw(worldTransform_, *viewProjection_); }

Vector3 Enemy::GetWorldPosition() { 
	//ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド座標の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kHeight / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kHeight / 2.0f};

	return aabb;
}



void Enemy::OnCollision(const Player* player) {
	(void)player;

}
