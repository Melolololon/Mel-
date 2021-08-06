#pragma once

#include<DirectXMath.h>

//Color型とColorAlphaを分ける?

namespace MelLib
{
	struct Color
	{
		//rgbaは型を統一しないといけないっぽい
		//charだとaがわかりにくいし、
		//floatだとrgbがわかりにくい。内部で255で割ればいいが、メモリの使用量が多い
		//Mapするとき、ColorじゃなくてXMFLOAT4にすれば、floatとUCHARを混合できる

		Color();
		Color(const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);
		Color(const unsigned char rgb, const unsigned char a);

		//色
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;
		unsigned char a = 255;

		Color operator+(const Color& color);
		Color operator-(const Color& color);
		Color operator*(const Color& color);
		Color operator/(const Color& color);

		/// <summary>
		/// パーセント(0から100)を0から255に変換します。
		/// </summary>
		/// <param name="par"></param>
		/// <returns></returns>
		static unsigned char ParToUChar(const float par) { return (unsigned char)(par / 100 * 255); }

	private:


	};
}