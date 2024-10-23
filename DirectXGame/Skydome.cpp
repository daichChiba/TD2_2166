#include "Skydome.h"
#include <cassert>

void Skydome::Initialize(Model* model, Camera* camera) {
	assert(model);
	worldTransform_.Initialize();
	model_ = model;
	camera_ = camera;
}

void Skydome::Update() { worldTransform_.UpdateMatrix(); }

void Skydome::Draw() { 
	model_->Draw(worldTransform_, *camera_); 
}
