#pragma once

#include<DirectXMath.h>

//Color�^��ColorAlpha�𕪂���?

struct Color
{
	//rgba�͌^�𓝈ꂵ�Ȃ��Ƃ����Ȃ����ۂ�
	//char����a���킩��ɂ������A
	//float����rgb���킩��ɂ����B������255�Ŋ���΂������A�������̎g�p�ʂ�����
	//Map����Ƃ��AColor����Ȃ���XMFLOAT4�ɂ���΁Afloat��UCHAR�������ł���

	Color();
	Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
	Color(const unsigned char rgb, const unsigned char a);

	//�F
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	unsigned char a = 255;
	
	Color operator+(const Color& color);
	Color operator-(const Color& color);
	Color operator*(const Color& color);
	Color operator/(const Color& color);

	/// <summary>
	/// �p�[�Z���g(0����100)��0����255�ɕϊ����܂��B
	/// </summary>
	/// <param name="par"></param>
	/// <returns></returns>
	static unsigned char ParToUChar(const float par){ return (unsigned char)(par / 100 * 255); }

private:
	

};
