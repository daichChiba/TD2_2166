#include "Player.h"
#include "Math.h"
#include "MathUtilityForText.h"
#include "cassert"
#include <algorithm>
#include <numbers>
using namespace KamataEngine;

// Initializeの関数定義
void Player::Initialize(Model* model, Camera* camera, const Vector3& pos) {

	// NULLポインタチェック
	assert(model);

	// データを引数に記録する
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;
}

void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

// Drawの関数定義
void Player::Draw() { 
	model_->Draw(worldTransform_, *camera_);
}