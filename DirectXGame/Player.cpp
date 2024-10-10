#define NOMINMAX
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
	// 移動入力
	InputMove();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

// Drawの関数定義
void Player::Draw() { 
	model_->Draw(worldTransform_, *camera_); }

void Player::InputMove() {
	// 接地状態
	if (onGround_) {
		Vector3 acceleration = {};

		// 　左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				if (velocity_.x > 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAcceleration);
				}
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録する
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
				acceleration.x -= kAcceleration;
			}
			// 左右加速

			// 加速/減速
			velocity_ += acceleration;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, 0, 1);
		}
		//// ジャンプ開始
		// if (velocity_.y > 0.0f) {
		//	// 空中状態に移行
		//	onGround_ = false;
		// }
	} else {
		// 空中
		// 落下速度
		velocity_ += Vector3(0, 0, 0);
	}
}
