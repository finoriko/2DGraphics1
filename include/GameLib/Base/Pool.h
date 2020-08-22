#ifndef INCLUDED_GAMELIB_POOL_H
#define INCLUDED_GAMELIB_POOL_H

namespace GameLib {

	/// 어떤 형태의 두는 곳.오직 new를 삭감하기 위해서만 이용된다.
	/*!
	확보하기 위해서만 있으며 그 후의 조작은 아무것도 할 수 없다는 것에 주의할 것.
	또 부분 해방이 불가능하다.할 수 있는 것은 전체 해방뿐이다.
	*/
	template< class T > class Pool {
	public:
		explicit Pool(int defaultBlockSize = 16);
		~Pool();
		///표준 블록 수 변경.
		void setDefaultBlockSize(int blockSize);
		///전체 해방
		void clear();
		///n개 확보
		T* allocate(int n = 1);
		///n개복사컨스트럭트
		T* allocate(const T*, int n = 1);
	private:
		struct Block {
			Block* mNext; //다음 블록
			T* mElements; //배열
			int mUsedNumber; //사용된 수
			int mSize; //블록 크기
		};
		void operator=(const Pool&); //대입 금지
		Pool(const Pool&); //복사 콘스트럭터 금지

		//상수
		int mDefaultBlockSize; //단위량
		Block mHead; //더미 노드
		Block* mLastBlock; //최종 노드
	};

} //namespace GameLib
#include "GameLib/Base/Impl/PoolImpl.h" //속에 든것은 이중에

#endif
