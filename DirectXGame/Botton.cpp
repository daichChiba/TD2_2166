#include "Botton.h"
#include "cassert"
#include <algorithm>
#include <numbers>
#include "MathUtilityForText.h"

void Botton::Initialize(Model* model, Camera* camera, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);

	// データを引数に記録する
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;

	// 速度を制限する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Botton::Update() {
	walkTimer_ += 1.0f / 60.0f;
	worldTransform_.UpdateMatrix();
}

void Botton::Draw() { 
	model_->Draw(worldTransform_, *camera_);
}

Vector3 Botton::GetWorldPosition() {
	//ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド座標の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Botton::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kHeight / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kHeight / 2.0f};

	return aabb;
}

void Botton::OnCollision(const Player* player) {
	(void)player; }

void Botton::OnCollision(const Mirror* mirror) {
	(void)mirror;
}
