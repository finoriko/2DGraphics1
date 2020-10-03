#include "GameLib/GameLib.h"
#include "GameLib/Base/Compressor.h"

#define OPTIMIZATION //이것이 ON이라고 고속화 판.OFF라고 우직 버전.

namespace GameLib
{
	namespace
	{
		//비트 할당 상수
		const int DIC_BITS = 11; // 위치 bit 수
		//이하 자동계산 상수
		const int LENGTH_BITS = 16 - 1 - DIC_BITS; //길이
		const int DIC_MASK = (1 << DIC_BITS) - 1;
		const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; //아래 8bit를 찌그러뜨린 것
		const int DIC_MASK_SHIFTED = (DIC_MASK >> 8) << LENGTH_BITS;
		const int LENGTH_MASK = (1 << LENGTH_BITS) - 1;
		const int DIC_SIZE = DIC_MASK + 1; // 사전 크기(1 빼서 저장하는 관계로 1 많이 사용됨)
		const int MAX_LENGTH = LENGTH_MASK + 3; // 최대 일치 길이(3빼서 저장하는 관계로 3개 많이 사용 가능)
		//자주 사용하는 최소값과 최대값
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