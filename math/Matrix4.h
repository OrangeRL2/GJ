#pragma once

#include "Vector3.h"

struct Matrix4
{
	float m[4][4];
};

Matrix4 Identity();
Matrix4 scale(const Vector3& s);

Matrix4 rotationX(float angle);
Matrix4 rotationY(float angle);
Matrix4 rotationZ(float angle);

Matrix4 translate(const Vector3& t);

Vector3 Transform(const Vector3& v, const Matrix4& m);

Matrix4& operator*=(Matrix4& m1, const Matrix4& m2);

Matrix4 operator*(const Matrix4& m1, const Matrix4& m2);
Vector3 operator*(const Vector3& v, const Matrix4& m);

