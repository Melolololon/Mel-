#include "Matrix.h"
#include<cmath>

Matrix::Matrix()
{
}

Matrix::Matrix(const DirectX::XMMATRIX& m)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			matrix[i][j] = m.r[i].m128_f32[j];
}

#pragma region 演算子

Matrix Matrix::operator*(const Matrix& mat)const
{
	Matrix m = GetZeroMatrix();

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				m[i][j] += matrix[i][k] * mat[k][j];

	return m;
}


void Matrix::operator*=(const Matrix& mat)
{
	Matrix m;
	m.matrix = matrix;
	m = m * mat;
	matrix = m.matrix;
}

std::array<float, 4>& Matrix::operator[](const unsigned int num)
{
	return matrix[num];
}

std::array<float, 4> Matrix::operator[](const unsigned int num)const
{
	return matrix[num];
}



#pragma endregion




/// <summary>
/// 零行列を作成します 
/// </summary>
Matrix GetZeroMatrix()
{
	Matrix m;
	for(auto& mat : m.matrix)
		for(auto& mat2 : mat)
			mat2 = 0;
	return m;
}

/// <summary>
/// 単位行列を作成します
/// </summary>
/// <returns></returns>
Matrix GetIdentityMatrix()
{
	Matrix m = GetZeroMatrix();

	for(int i = 0; i < 4;i++)
	m[i][i] = 1;

	return m;
}

/// <summary>
/// X軸を基準に回転する行列を作成します
/// </summary>
/// <returns></returns>
Matrix GetRotateXMatrix(const float angle)
{
	Matrix m = GetIdentityMatrix();

	m[1][1] = std::cos(angle);
	m[1][2] = std::sin(angle);
	m[2][1] = std::sin(-angle);
	m[2][2] = std::cos(angle);

	return m;
}

Matrix GetRotateYMatrix(const float angle)
{
	Matrix m = GetIdentityMatrix();

	m[0][0] = std::cos(angle);
	m[0][2] = std::sin(-angle);
	m[2][0] = std::sin(angle);
	m[2][2] = std::cos(angle);

	return m;
}

Matrix GetRotateZMatrix(const float angle)
{
	Matrix m = GetIdentityMatrix();

	m[0][0] = std::cos(angle);
	m[0][1] = std::sin(angle);
	m[1][0] = std::sin(-angle);
	m[1][1] = std::cos(angle);
	
	return m;
}

Matrix GetScalingMatrix(const Vector3& vector)
{
	Matrix m = GetIdentityMatrix();

	m[0][0] = vector.x;
	m[1][1] = vector.y;
	m[2][2] = vector.z;

	return m;
}

Matrix GetTranslationMatrix(const Vector3& vector)
{
	Matrix m = GetIdentityMatrix();

	m[3][0] = vector.x;
	m[3][1] = vector.y;
	m[3][2] = vector.z;

	return m;
}

