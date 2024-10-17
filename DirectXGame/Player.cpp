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

	// 衝突情報を初期化
	CollisionMapInfo collisionMapInfo;
	// 移動量に速度の値をコピー
	collisionMapInfo.velocity = velocity_;



	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

// Drawの関数定義
void Player::Draw() { model_->Draw(worldTransform_, *camera_); }

void Player::InputMove() {

	// 　左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D)) {

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {
			MapChipType mapChipType =
				mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 1.0f), (int)worldTransform_.translation_.z);
			if (mapChipType==MapChipType::kBlank) {
				worldTransform_.translation_.x += 0.1f;
			}


		} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {
			MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x + 1.0f), (int)worldTransform_.translation_.z);
			if (mapChipType == MapChipType::kBlank) {
				worldTransform_.translation_.x -= 0.1f;
			}
		}
	}
	if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_W)) {
		MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x), (int)(worldTransform_.translation_.z-1.0f));
		if (mapChipType == MapChipType::kBlank) {
			worldTransform_.translation_.z += 0.1f;
		}
	} else if (Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_S)) {
		MapChipType mapChipType = mapChipField_->GetMapChipTypeByIndex((int)(worldTransform_.translation_.x), (int)(worldTransform_.translation_.z - 1.0f));
		if (mapChipType == MapChipType::kBlank) {
			worldTransform_.translation_.z -= 0.1f;
		}
	}
}

void Player::AnimateTurn() {

	// 旋回制御
	if (turnTimer_ > 0.0f) {

		turnTimer_ -= 1.0f / 60.0f;

		// 左右の自キャラの角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
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
//void Player::OnCollision(const MapChipField* mapchipField) {
//	(void)mapchipField;
//	isBlockHit_ = true;
//}
// void Player::CheckMapCollision(CollisionMapInfo& info) {
//	CheckMapCollisionUp(info);
//	CheckMapCollisionDown(info);
//	CheckMapCollisionRight(info);
//	CheckMapCollisionLeft(info);
// }
//
// void Player::CheckMapCollisionUp(CollisionMapInfo& info) {
//	// 上昇あり？
//	if (info.velocity.y <= 0) {
//		return;
//	}
//
//	// 移動後の4つの角の座標
//	std::array<Vector3, kNumCorner> positionNew;
//	for (uint32_t i = 0; i < positionNew.size(); ++i) {
//		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
//	}
//
//	// 移動後の4つの角の座標の計算
//	MapChipType mapChipType;
//	// 真上の当たり判定を行う
//	bool isHit = false;
//	// 左上の判定
//	MapChipField::IndexSet indexSet;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//	// 右上点の判定
//	// MapChipField::IndexSet indexSet_;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//
//	//	ブロックにヒット？
//	if (isHit == true) {
//		// めり込みを排除する方向に移動量を設定する
//		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, +kHeight / 2.0f, 0));
//		// めり込み先ブロックの範囲矩形
//		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
//		info.velocity.z = std::max(0.0f, rect.bottom - worldTransform_.translation_.z - (kHeight / 2.0f + kBlank));
//		// 天井に当たったことを記録する
//		info.isCeiling = true;
//	}
// }
//
// void Player::CheckMapCollisionDown(CollisionMapInfo& info) {
//
//	// 下降あり？
//	if (info.velocity.z >= 0) {
//		return;
//	}
//
//	// 移動後の4つの角の座標
//	std::array<Vector3, kNumCorner> positionNew;
//	for (uint32_t i = 0; i < positionNew.size(); ++i) {
//		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
//	}
//
//	// 移動後の4つの角の座標の計算
//	MapChipType mapChipType;
//	// 真上の当たり判定を行う
//	bool isHit = false;
//
//	// 左下の判定
//	MapChipField::IndexSet indexSet;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//	// 右下点の判定
//	// MapChipField::IndexSet indexSet_;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//
//	//	ブロックにヒット？
//	if (isHit == true) {
//		// めり込みを排除する方向に移動量を設定する
//		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(0, -kHeight / 2.0f, 0));
//		// めり込み先ブロックの範囲矩形
//		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
//		info.velocity.z = std::max(0.0f, rect.bottom - worldTransform_.translation_.z - (kHeight / 2.0f + kBlank));
//		// 天井に当たったことを記録する
//		info.isLanding = true;
//	}
// }
//
// void Player::CheckMapCollisionRight(CollisionMapInfo& info) {
//	// 左移動あり？
//	if (info.velocity.x <= 0) {
//		return;
//	}
//
//	// 移動後の4つの角の座標
//	std::array<Vector3, kNumCorner> positionNew;
//	for (uint32_t i = 0; i < positionNew.size(); ++i) {
//		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
//	}
//
//	// 移動後の4つの角の座標の計算
//	MapChipType mapChipType;
//	// 真上の当たり判定を行う
//	bool isHit = false;
//
//	// 右上の判定
//	MapChipField::IndexSet indexSet;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//	// 右下点の判定
//	// MapChipField::IndexSet indexSet_;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//
//	//	ブロックにヒット？
//	if (isHit == true) {
//		// めり込みを排除する方向に移動量を設定する
//		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));
//		// めり込み先ブロックの範囲矩形
//		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
//		info.velocity.x = std::max(0.0f, rect.bottom - worldTransform_.translation_.z - (kWidth / 2.0f + kBlank));
//		// 天井に当たったことを記録する
//		info.IsHitWall = true;
//	}
// }
//
// void Player::CheckMapCollisionLeft(CollisionMapInfo& info) {
//	// 左移動あり？
//	if (info.velocity.x >= 0) {
//		return;
//	}
//
//	// 移動後の4つの角の座標
//	std::array<Vector3, kNumCorner> positionNew;
//	for (uint32_t i = 0; i < positionNew.size(); ++i) {
//		positionNew[i] = CornerPosition(worldTransform_.translation_ + info.velocity, static_cast<Corner>(i));
//	}
//
//	// 移動後の4つの角の座標の計算
//	MapChipType mapChipType;
//	// 真上の当たり判定を行う
//	bool isHit = false;
//
//	// 左上の判定
//	MapChipField::IndexSet indexSet;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//	// 左下点の判定
//	// MapChipField::IndexSet indexSet_;
//	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
//	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
//	if (mapChipType == MapChipType::kBlock) {
//		isHit = true;
//	}
//
//	//	ブロックにヒット？
//	if (isHit == true) {
//		// めり込みを排除する方向に移動量を設定する
//		indexSet = mapChipField_->GetMapChipIndexSetByPosition(worldTransform_.translation_ + Vector3(-kWidth / 2.0f, 0, 0));
//		// めり込み先ブロックの範囲矩形
//		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
//		info.velocity.x = std::max(0.0f, rect.bottom - worldTransform_.translation_.z - (kWidth / 2.0f + kBlank));
//		// 天井に当たったことを記録する
//		info.IsHitWall = true;
//	}
// }
//
// Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
//	Vector3 offsetTable[kNumCorner] = {
//	    {+kWidth / 2.0f, -kHeight / 2.0f, 0}, // kRightBottom
//	    {-kWidth / 2.0f, -kHeight / 2.0f, 0}, // kLeftBottom
//	    {+kWidth / 2.0f, +kHeight / 2.0f, 0}, // kRightTop
//	    {-kWidth / 2.0f, +kHeight / 2.0f, 0}, // kLeftTop
//	};
//	return center + offsetTable[static_cast<uint32_t>(corner)];
// }
//
// void Player::isHitCeiling(const CollisionMapInfo& info) {
//	// 天井に当たった?
//	if (info.isCeiling == true) {
//		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
//		velocity_.z = 0;
//	}
// }
// void Player::collisionHitMove(const CollisionMapInfo& info) {
//	// 移動
//	worldTransform_.translation_ += info.velocity;
// }
//
// void Player::isHitWall(const CollisionMapInfo& info) {
//	// 壁接触による減速
//	if (info.IsHitWall) {
//		velocity_.x *= (1.0f - kAttenuationWall);
//	}
// }
