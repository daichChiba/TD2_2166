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

	//// 衝突情報を初期化
	// CollisionMapInfo collisionMapInfo;
	//// 移動量に速度の値をコピー
	// collisionMapInfo.velocity = velocity_;

	AnimateTurn();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

// Drawの関数定義
void Player::Draw() {
	if (isAive) {
		model_->Draw(worldTransform_, *camera_);
	}
}

void Player::InputMove() {
	if (isAive) {

		// 　左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {

			if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {
				MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 1.0f), (int)(worldTransform_.translation_.z - 0.0f));
				MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 1.0f), (int)(worldTransform_.translation_.z + 0.9f));

				if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
					worldTransform_.translation_.x += 0.1f;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {
				MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x - 0.1f), (int)(worldTransform_.translation_.z - 0.0f));
				MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x - 0.1f), (int)(worldTransform_.translation_.z + 0.9f));

				if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
					worldTransform_.translation_.x -= 0.1f;
				}
			}
		}
		if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_W)) {
			MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.0f), (int)(worldTransform_.translation_.z + 1.0f));
			MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.9f), (int)(worldTransform_.translation_.z + 1.0f));
			if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
				worldTransform_.translation_.z += 0.1f;
			}
		} else if (Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_S)) {
			MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.0f), (int)(worldTransform_.translation_.z - 0.1f));
			MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.9f), (int)(worldTransform_.translation_.z - 0.1f));
			if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
				worldTransform_.translation_.z -= 0.1f;
			}
		}
	}
}

void Player::AnimateTurn() {

	// 旋回制御
	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラの角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 4.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
		// 状況に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);

		MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド座標の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kHeight / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kHeight / 2.0f};

	return aabb;
}
void Player::OnCollision(const Botton* botton, bool isMirrorHit) {
	(void)botton;
	isHit_ = true;
	if (isMirrorHit == true) {
		isAive = false;
	}
}
