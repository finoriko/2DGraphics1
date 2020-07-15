#ifndef INCLUDED_GAMELIB_MATH_FUNCTIONS_H
#define INCLUDED_GAMELIB_MATH_FUNCTIONS_H

//���� ���̺귯��.���� ������ ��.
namespace GameLib 
{
	namespace Math 
	{
		//�ﰢ�Լ�
		float sin(float);
		float cos(float);
		float tan(float);
		//���ﰢ�Լ�
		float asin(float);
		float acos(float);
		float atan(float);
		float atan2(float y, float x);

		float pow(float a, float b); //�ŵ�����(a��b��)
		float log2(float); //2�� �ٴ����� �ϴ� �α�
		float log10(float); //10�� �� �ٴ����� �ϴ� �α�
		float abs(float);//���밪
		float min(float, float); //�ּ�
		float max(float, float); //�ִ�
		float sqrt(float); //������

		const float EPSILON = 0.00000011920928955078125f; //float����ͣ����(1/2^23)
		const float FLOAT_MAX = 1.7014117331926442990585209174226e+38; //float��������((2^24-1)/2^23 * 2^126 )

		bool isSpecial(float); //NaN,Inf��true
	}
}
#endif