#pragma once
#include"Vector.h"

namespace MelLib
{
	/*class Easing
	{
	private:
		Easing() {}
		~Easing() {}

		static Vector3 CalcEasing(const Vector3& startPos, const Vector3& endPos, const float t);

	public:
		static Vector3 EaseIn
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);

		static Vector3 EaseOut
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);

		static Vector3 EaseInOut
		(
			const Vector3& startPos,
			const Vector3& endPos,
			const float time
		);



	};*/

	//イージングの情報、Tを持たせたクラス
	template<class V>
	class Easing
	{
	private:
		V value;
		V start;
		V end;

		//パーセント
		float par = 0.0f;

		void CalcEasing(const V startPos, const V endPos, const float par) { return startPos * (1.0f - (par / 100.0f)) + endPos * (par / 100.0f); }
	public:
		Easing() {}
		Easing(V start, V end) :start(start), end(end), par(0.0f) {}
		Easing(V start, V end, const float par) :start(start), end(end), par(par) {}


		void EaseIn()
		{
			float p = (par * par);
			return CalcEasing(start, end, p);
		}

		void EaseOut()
		{
			float p = par * (2 - par);
			return CalcEasing(start, end, p);
		}

		void EaseInOut()
		{
			float p = par * par * (3 - 2 * par);
			return CalcEasing(start, end, p);
		}

		void SetStart(const V start) { this->start = start; }
		void SetEnd(const V end) { this->end = end; }
		void SetPar(const float par) { this->par = par; }
	};
}
