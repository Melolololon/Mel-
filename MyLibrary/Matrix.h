#pragma once
#include<array>
#include"Vector.h"
#include<DirectXMath.h>
struct Matrix
{
	Matrix();
	Matrix(const DirectX::XMMATRIX& m);

	std::array<std::array<float,4>,4> matrix;

	Matrix operator*(const Matrix& mat)const;
	void operator*=(const Matrix& mat);
	
	std::array<float, 4>&operator[](const unsigned int& num);
	std::array<float, 4>operator[](const unsigned int& num)const;

};


Matrix getZeroMatrix();

Matrix getIdentityMatrix();
Matrix getRotateXMatrix(const float& angle);
Matrix getRotateYMatrix(const float& angle);
Matrix getRotateZMatrix(const float& angle);
Matrix getScalingMatrix(const Vector3& vector);
Matrix getTranslationMatrix(const Vector3& vector);

