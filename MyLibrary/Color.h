#pragma once

struct Color
{
	//rgba�͌^�𓝈ꂵ�Ȃ��Ƃ����Ȃ����ۂ�
	//char����a���킩��ɂ������A
	//float����rgb���킩��ɂ����B������255�Ŋ���΂������A�������̎g�p�ʂ�����
	//Map����Ƃ��AColor����Ȃ���XMFLOAT4�ɂ���΁Afloat��UCHAR�������ł���

	Color();
	Color(const unsigned char r, const unsigned char g, const unsigned char b, const float a);
	Color(const unsigned char rgb, const float a);

	//�F
	unsigned char r = 0;
	unsigned char g = 0;
	unsigned char b = 0;
	float a = 100.0f;
	
	Color operator+(const Color& color);
	Color operator-(const Color& color);
	Color operator*(const Color& color);
	Color operator/(const Color& color);

private:
	void AlphaClamp();

};
