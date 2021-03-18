#include "Matrix.h"
#include<cmath>

#pragma region ���Z�q

Matrix Matrix::operator*(const Matrix& mat)
{
	Matrix m = getZeroMatrix();

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				m.matrix[i][j] += matrix[i][k] * mat.matrix[k][j];

	return m;
}


void Matrix::operator*=(const Matrix& mat)
{
	Matrix m;
	m.matrix = matrix;
	m = m * mat;
	matrix = m.matrix;
}

#pragma endregion




/// <summary>
/// ��s���Ԃ��܂�
/// </summary>
Matrix getZeroMatrix()
{
	Matrix m;
	for(auto& mat : m.matrix)
	{
		for(auto& mat2 : mat)
		{
			mat2 = 0;
		}
	}
	return m;
}

/// <summary>
/// �P�ʍs���Ԃ��܂�
/// </summary>
/// <returns></returns>
Matrix getIdentityMatrix()
{
	Matrix m = getZeroMatrix();

	m.matrix[0][0] = 1;
	m.matrix[1][1] = 1;
	m.matrix[2][2] = 1;
	m.matrix[3][3] = 1;

	return m;
}

/// <summary>
/// X������ɉ�]����s���Ԃ��܂�
/// </summary>
/// <returns></returns>
Matrix getRotateXMatrix(const float& angle)
{
	Matrix m = getIdentityMatrix();

	m.matrix[1][1] = std::cos(angle);
	m.matrix[1][2] = std::sin(angle);
	m.matrix[2][1] = std::sin(-angle);
	m.matrix[2][2] = std::cos(angle);

	return m;
}

Matrix getRotateYMatrix(const float& angle)
{
	Matrix m = getIdentityMatrix();

	m.matrix[0][0] = std::cos(angle);
	m.matrix[0][2] = std::sin(-angle);
	m.matrix[2][0] = std::sin(angle);
	m.matrix[2][2] = std::cos(angle);

	return m;
}

Matrix getRotateZMatrix(const float& angle)
{
	Matrix m = getIdentityMatrix();

	m.matrix[0][0] = std::cos(angle);
	m.matrix[0][1] = std::sin(angle);
	m.matrix[1][0] = std::sin(-angle);
	m.matrix[1][1] = std::cos(angle);
	
	return m;
}

Matrix getScalingMatrix(const Vector3& vector)
{
	Matrix m = getIdentityMatrix();

	m.matrix[0][0] = vector.x;
	m.matrix[1][1] = vector.y;
	m.matrix[2][2] = vector.z;

	return m;
}

Matrix getTranslationMatrix(const Vector3& vector)
{
	Matrix m = getIdentityMatrix();

	m.matrix[3][0] = vector.x;
	m.matrix[3][1] = vector.y;
	m.matrix[3][2] = vector.z;

	return m;
}

