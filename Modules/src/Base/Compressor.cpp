#include "GameLib/GameLib.h"
#include "GameLib/Base/Compressor.h"

#define OPTIMIZATION //�̰��� ON�̶�� ���ȭ ��.OFF��� ���� ����.

namespace GameLib
{
	namespace
	{
		//��Ʈ �Ҵ� ���
		const int DIC_BITS = 11; // ��ġ bit ��
		//���� �ڵ���� ���
		const int LENGTH_BITS = 16 - 1 - DIC_BITS; //����
		const int DIC_MASK = (1 << DIC_BITS) - 1;
		const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; //�Ʒ� 8bit�� ��׷��߸� ��
		const int DIC_MASK_SHIFTED = (DIC_MASK >> 8) << LENGTH_BITS;
		const int LENGTH_MASK = (1 << LENGTH_BITS) - 1;
		const int DIC_SIZE = DIC_MASK + 1; // ���� ũ��(1 ���� �����ϴ� ����� 1 ���� ����)
		const int MAX_LENGTH = LENGTH_MASK + 3; // �ִ� ��ġ ����(3���� �����ϴ� ����� 3�� ���� ��� ����)
		//���� ����ϴ� �ּҰ��� �ִ밪
		inline int min(int a, int b) 
		{
			return (a < b) ? a : b;
		}

		inline int max(int a, int b) 
		{
			return (a > b) ? a : b;
		}

	}
}