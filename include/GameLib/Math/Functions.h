#ifndef INCLUDED_GAMELIB_MATH_FUNCTIONS_H
#define INCLUDED_GAMELIB_MATH_FUNCTIONS_H

//수학 라이브러리.각도 단위는 도.
namespace GameLib 
{
	namespace Math 
	{
		//삼각함수
		float sin(float a);
		float cos(float a);
		float tan(float a);
		//역삼각함수
		float asin(float a);
		float acos(float a);
		float atan(float a);
		float atan2(float y, float x);

		float pow(float a, float b); //거듭제곱(a의b승)
		float log2(float a); //2를 바닥으로 하는 로그
		float log10(float a); //10을 을 바닥으로 하는 로그
		float abs(float a);//절대값
		float min(float a, float b); //최소
		float max(float a, float b); //최대
		float sqrt(float a); //제곱근

		const float EPSILON = 0.00000011920928955078125f; //floatの限界精度(1/2^23)
		const float FLOAT_MAX = 1.7014117331926442990585209174226e+38; //float의 최대값((2^24-1)/2^23 * 2^126 )

		bool isSpecial(float a); //NaN,Inf면true
	}
}
#endif