#ifndef INCLUDED_GAMELIB_QUEUE_H
#define INCLUDED_GAMELIB_QUEUE_H

namespace GameLib {

	/// 큐. 요소 수가 한계를 넘지 않으면 순환하기 때문에 언제까지 사용할 수있다.
	template< class T > class Queue {
	public:
		// 기본값.사이즈, 최대 용량은 0.
		Queue();
		/// 최대 용량 지정.
		explicit Queue(int capacity);
		~Queue();
		/// 재확보.내용물이 있는 상태에서 부르면 assert. clear() 해라
		void setCapacity(int size);
		/// 최대 용량 획득
		int capacity() const;
		/// 현재 요소 수 획득
		int size() const;
		/// 전해방
		void clear();
		/// 말미에 하늘로 추가.더한 것에 대한 포인터를 반환하므로 초기설정은 이것을 사용할 것.
		T* pushBack();
		/// 말미에 추가
		void pushBack(const T&);
		///선두 삭제
		void popFront();
		///선두 삭제(버림 사본 가져오기)
		void popFront(T* out);
		///선두 획득(const)
		const T* front() const;
		///선두 획득(비const)
		T* front();
	private:
		void operator=(const Queue&); //대입금지
		Queue(const Queue&); //복사 콘스트럭터 금지

		T* mElements;
		int mSize;
		int mCapacity;
		int mFront;
	};

} //namespace GameLib
#include "GameLib/Base/Impl/QueueImpl.h" //속은 이 중

#endif

