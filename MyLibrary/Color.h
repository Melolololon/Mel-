#pragma once

struct Color
{
	//rgba�͌^�𓝈ꂵ�Ȃ��Ƃ����Ȃ����ۂ�
	//char����a���킩��ɂ������A
	//float����rgb���킩��ɂ����B������255�Ŋ���΂������A�������̎g�p�ʂ�����
	//Map����Ƃ��AColor����Ȃ���XMFLOAT4�ɂ���΁Afloat��UCHAR�������ł���
	unsigned char r, g, b, a;
	
	Color operator+(Color color);
	Color operator-(Color color);
	Color operator*(Color color);
	Color operator/(Color color);

	/// <summary>
	/// �p�[�Z���g��0�`255�̐��l�ɕϊ����܂�
	/// </summary>
	/// <param name="par">�p�[�Z���g</param>
	/// <returns></returns>
	static unsigned char alphaChangePar(float par);

	//Color operator+=(Color color);
	//Color operator-=(Color color);
	//Color operator*=(Color color);
	//Color operator/=(Color color);
};