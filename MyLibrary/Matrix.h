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
	
	std::array<float, 4>&operator[](const unsigned int num);
	std::array<float, 4>operator[](const unsigned int num)const;

};


Matrix GetZeroMatrix();

Matrix GetIdentityMatrix();
Matrix GetRotateXMatrix(const float angle);
Matrix GetRotateYMatrix(const float angle);
Matrix GetRotateZMatrix(const float angle);
Matrix GetScalingMatrix(const Vector3& vector);
Matrix GetTranslationMatrix(const Vector3& vector);

