#pragma once
#include "DirectXMath.h"
using namespace DirectX;

//向いている方向に回転
void rollRotation(XMFLOAT3 *vector, XMFLOAT3 rotation);

float vector3Length(XMFLOAT3 vector_);
XMFLOAT3 vector3Normalize(XMFLOAT3 vector_);

int RNG(int min, int max, bool preciseMode);

float easeInOutQuart(float x);

float easeOutQuint(float x);

float easeOutBounce(float x);

float easeInBounce(float x);



