#pragma once

struct Color
{
	//rgbaは型を統一しないといけないっぽい
	//charだとaがわかりにくいし、
	//floatだとrgbがわかりにくい。内部で255で割ればいいが、メモリの使用量が多い
	//Mapするとき、ColorじゃなくてXMFLOAT4にすれば、floatとUCHARを混合できる

	Color();
	Color(const unsigned char r, const unsigned char g, const unsigned char b, const float a);
	Color(const unsigned char rgb, const float a);

	//色
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
