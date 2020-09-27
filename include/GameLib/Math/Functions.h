#ifndef INCLUDED_GAMELIB_MATH_FUNCTIONS_H
#define INCLUDED_GAMELIB_MATH_FUNCTIONS_H

//���� ���̺귯��.���� ������ ��.
namespace GameLib 
{
	namespace Math 
	{
		//�ﰢ�Լ�
		float sin(float a);
		float cos(float a);
		float tan(float a);
		//���ﰢ�Լ�
		float asin(float a);
		float acos(float a);
		float atan(float a);
		float atan2(float y, float x);

		float pow(float a, float b); //�ŵ�����(a��b��)
		float log2(float a); //2�� �ٴ����� �ϴ� �α�
		float log10(float a); //10�� �� �ٴ����� �ϴ� �α�
		float abs(float a);//���밪
		float min(float a, float b); //�ּ�
		float max(float a, float b); //�ִ�
		float sqrt(float a); //������

		const float EPSILON = 0.00000011920928955078125f; //float����ͣ����(1/2^23)
		const float FLOAT_MAX = 1.7014117331926442990585209174226e+38; //float�� �ִ밪((2^24-1)/2^23 * 2^126 )

		bool isSpecial(float a); //NaN,Inf��true
	}
}
#endif