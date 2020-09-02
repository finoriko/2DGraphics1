#ifndef INCLUDED_GAMELIB_STACK_H
#define INCLUDED_GAMELIB_STACK_H

namespace GameLib {

	/// 솔직한 고정 사이즈 스택
	template< class T > class Stack {
	public:
		/// 기본값.사이즈, 최대 용량은 0.
		Stack();
		///최대 용량 지정.
		explicit Stack(int capacity);
		~Stack();
		///재확보.내용물이 있는 상태에서 부르면 assert. clear()해라.
		void setCapacity(int size);
		///최대 용량 획득
		int capacity() const;
		/// 현재 요소 수 획득
		int size() const;
		///전해방
		void clear();
		///말미에 추가
		void push(const T&);
		///말미에 추가(기본값 컨스트럭터 호출)
		void push();
		///말미 삭제
		void pop();
		///말미 삭제(버리는 요소 복사 받아보기)
		void pop(T* out);
		///말미 취득(const)
		const T* get() const;
		///말미 취득(비const)
		T* get();
	private:
		void operator=(const Stack&); //대입 금지
		Stack(const Stack&); //복사 콘스트럭터 금지

		T* mElements;
		int mSize;
		int mCapacity;
		int mBack;
	};

} //namespace GameLib
#include "GameLib/Base/Impl/StackImpl.h" //속은 이 안에 있다

#endif

