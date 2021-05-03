#pragma once

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

	static unsigned char ToPar(const float par){ return (unsigned char)(par / 100 * 255); }
private:
	

};
