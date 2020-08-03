#ifndef INCLUDED_GAMELIB_TANK_H
#define INCLUDED_GAMELIB_TANK_H

#include "GameLib/Base/Array.h"

namespace GameLib {

	/// 다 더할 때까지 접속하지 않고 몇 개 더할지 알 수 없다는 용도를 위한 것
	/*!
	물을 모아 단번에 흘리는 이미지로 탱크라고 했지만, 더 좋은 이름이 있으면 모집.
	내부적으로는 용량단위(block Size)씩 내부에서 알로케이트 하여 보충한다.

	접속은 getCurrent()로 취득, toNext()로 다음에.rewind()로 처음부터 다시 액세스할 수 있다.
	*/
	template< class T > class Tank {
	public:
		explicit Tank(int blockSize = 16);
		~Tank();
		///재확보. 내용물이 있는 상태로 부르면 assert. clear()해라.
		void setBlockSize(int blockSize);
		///현재 요소 수 취득
		int size() const;
		///전해방
		void clear();
		///말미에 하늘로 추가.더한 것에 대한 포인터를 반환하므로 초기설정은 이것을 사용할 것.
		T* add();
		///말미에 추가
		void add(const T&);
		/// 다음에
		void toNext();
		///끝입니다.
		bool isEnd() const;
		///현재 요소 가져오기(비const)
		T* get();
		///현재 요소 가져오기(const)
		const T* get() const;
		///// 첫 번째 요소로 포인터 되돌리기
		void rewind();
		///단순한 배열로 변환. 미리 size()한 크기로 new한 영역을 넘겨주는 것.
		void copyTo(T*) const;
		///Array에 복사.거짓이 아니면 혼난다.
		void copyTo(Array< T >*) const;
	private:
		struct Block {
			Block* mNext; //다음의 노드
			T* mElements; //배열
		};
		void operator=(const Tank&); //대입금지
		Tank(const Tank&); //복사 컨스트럭터금지

		//상수
		int mBlockSize; //단위량
		int mSize;
		Block mHead; //더미

		Block* mLastBlock; //최종노드
		int mLastPos; //최종 노드 내 위치
		Block* mCurrentBlock; //현재 이터레이터
		int mCurrentPos; //노드 내 위치
	};

} //namespace GameLib
#include "GameLib/Base/Impl/TankImpl.h" //내용은 이중

#endif
