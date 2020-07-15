#ifndef INCLUDED_GAMELIB_MATH_FUNCTIONS_H
#define INCLUDED_GAMELIB_MATH_FUNCTIONS_H

//수학 라이브러리.각도 단위는 도.
namespace GameLib 
{
	namespace Math 
	{
		//삼각함수
		float sin(float);
		float cos(float);
		float tan(float);
		//역삼각함수
		float asin(float);
		float acos(float);
		float atan(float);
		float atan2(float y, float x);

		float pow(float a, float b); //거듭제곱(a의b승)
		float log2(float); //2를 바닥으로 하는 로그
		float log10(float); //10을 을 바닥으로 하는 로그
		float abs(float);//절대값
		float min(float, float); //최소
		float max(float, float); //최대
		float sqrt(float); //제곱근

		const float EPSILON = 0.00000011920928955078125f; //floatの限界精度(1/2^23)
		const float FLOAT_MAX = 1.7014117331926442990585209174226e+38; //floatの最大値((2^24-1)/2^23 * 2^126 )

		bool isSpecial(float); //NaN,Inf면true
	}
}
#endif