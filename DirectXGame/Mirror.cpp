#include "Mirror.h"
#include "MathUtilityForText.h"
#include "cassert"
#include <algorithm>
#include <numbers>

void Mirror::Initialize(Model* model, Camera* camera, const Vector3& pos) {
	// NULLポインタチェック
	assert(model);

	// データを引数に記録する
	model_ = model;

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	worldTransform_.rotation_.y = -std::numbers::pi_v<float> / 2.0f;
	camera_ = camera;
}

void Mirror::Update() {
	// 移動入力
	InputMove();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Mirror::Draw() {
	if (isAive) {
		model_->Draw(worldTransform_, *camera_);
	}
}

void Mirror::InputMove() {
	if (isAive) {

		// 　左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {

			if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {
				MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x - 0.1f), (int)(worldTransform_.translation_.z - 0.0f));
				MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x - 0.1f), (int)(worldTransform_.translation_.z + 0.9f));

				if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
					worldTransform_.translation_.x -= 0.1f;
				}

			} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {
				MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 1.0f), (int)(worldTransform_.translation_.z - 0.0f));
				MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 1.0f), (int)(worldTransform_.translation_.z + 0.9f));

				if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
					worldTransform_.translation_.x += 0.1f;
				}
			}
		}
		if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_W)) {
			MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.0f), (int)(worldTransform_.translation_.z - 0.1f));
			MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.9f), (int)(worldTransform_.translation_.z - 0.1f));
			if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
				worldTransform_.translation_.z -= 0.1f;
			}
		} else if (Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_S)) {
			MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.0f), (int)(worldTransform_.translation_.z + 1.0f));
			MapChipType mapChipType2 = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 0.9f), (int)(worldTransform_.translation_.z + 1.0f));
			if (mapChipType == MapChipType::kBlank && mapChipType2 == MapChipType::kBlank) {
				worldTransform_.translation_.z += 0.1f;
			}
		}
	}
}

Vector3 Mirror::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド座標の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Mirror::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;
	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kHeight / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kHeight / 2.0f};

	return aabb;
}

void Mirror::OnCollision(const Botton* botton, bool isPlayerHit) {
	(void)botton;
	isHit_ = true;
	if (isPlayerHit==true) {
		isAive = false;
	}
}
