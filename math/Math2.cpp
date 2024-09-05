#include "Math2.h"
#include "cmath"
#include "Matrix4.h"
#include "math.h"
#include "stdlib.h"

void rollRotation(XMFLOAT3 *vector, XMFLOAT3 rotation)
{
	XMFLOAT3 vec = *vector;

	Matrix4 matRotX, matRotY, matRotZ;
	matRotX = rotationX(rotation.x);
	matRotY = rotationY(rotation.y);
	matRotZ = rotationZ(rotation.z);

	matRotZ *= matRotX;
	matRotZ *= matRotY;

	vector->x = vec.x * matRotZ.m[0][0] + vec.y * matRotZ.m[1][0] + vec.z * matRotZ.m[2][0];
	vector->y = vec.x * matRotZ.m[0][1] + vec.y * matRotZ.m[1][1] + vec.z * matRotZ.m[2][1];
	vector->z = vec.x * matRotZ.m[0][2] + vec.y * matRotZ.m[1][2] + vec.z * matRotZ.m[2][2];
}

float vector3Length(XMFLOAT3 vector_)
{
	float x2 = vector_.x * vector_.x;
	float y2 = vector_.y * vector_.y;
	float z2 = vector_.z * vector_.z;
	return sqrt(x2 + y2 + z2);
}

XMFLOAT3 vector3Normalize(XMFLOAT3 vector_)
{
	XMFLOAT3 v;
	v.x = vector_.x / vector3Length(vector_);
	v.y = vector_.y / vector3Length(vector_);
	v.z = vector_.z / vector3Length(vector_);

	return v;
}

int RNG(int min, int max, bool preciseMode)
{
	if (!preciseMode) {
		return (rand() % (max + 1 - min) + min);
	}

	int ret = 0;
	do {
		ret = rand();
	} while (ret >= RAND_MAX - RAND_MAX % (max + 1 - min));
	ret = ret % (max + 1 - min) + min;
}

float easeInOutQuart(float x) {
	return x < 0.5 ? 8 * x * x * x * x : 1 - pow(-2 * x + 2, 4) / 2;
}

float easeOutQuint(float x) {
	return 1 - pow(1 - x, 5);
}

float easeOutBounce(float x)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}

float easeInBounce(float x)
{
	return 1 - easeOutBounce(1 - x);
}
