#pragma once
#include "Vector3.h"
#include "Matrix4.h"

struct Quaternion
{
	Vector3 v;
	float w;
	Quaternion(float x, float y, float z, float w)
	{
		this->v.x = x;
		this->v.y = y;
		this->v.z = z;
		this->w = w;
	}
	Quaternion()
	{
		this->v.x = 0;
		this->v.y = 0;
		this->v.z = 0;
		this->w = 0;
	};
};

//単位Quaternionを返す
Quaternion IdentityQuaternion();
//積
Quaternion Multiply(const Quaternion& q1, const Quaternion& q2);
//共役Quaternionを返す
Quaternion Conjugate(const Quaternion& q);
//Quaternionのnormを返す
float Norm(const Quaternion& q);
//正規化
Quaternion Normalize(const Quaternion& q);
//逆Quaternionを返す
Quaternion Inverse(const Quaternion& q);
//Quaternionの内積を返す
float dot(const Quaternion& q0, const Quaternion& q1);

//任意軸回転を表すQuaternionの生成
Quaternion MakeAxisAngle(const Vector3& vector, float angle);
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& v, const Quaternion& q);
//quaternionから回転行列を求める
Matrix4 MakeRotateMatrix(const Quaternion& q);

//球面線形補間
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

