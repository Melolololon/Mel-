#pragma once
#include<array>
#include"Vector.h"
struct Matrix
{
	std::array<std::array<float,4>,4> matrix;
};


Matrix getZeroMatrix();

Matrix getIdentityMatrix();
Matrix getRotateXMatrix(const float& angle);
Matrix getRotateYMatrix(const float& angle);
Matrix getRotateZMatrix(const float& angle);
Matrix getScalingMatrix(const Vector3& vector);
Matrix getTranslationMatrix(const Vector3& vector);

