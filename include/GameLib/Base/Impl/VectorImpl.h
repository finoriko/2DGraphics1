#ifndef INCLUDED_GAMELIB_VECTORIMPL_H
#define INCLUDED_GAMELIB_VECTORIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/Vector.h"

namespace GameLib {

	template< class T > inline Vector< T >::Vector(int size, int capacity) :
		mElements(0),
		mSize(size),
		mCapacity(capacity) {
		if (capacity <= 0) {
			mCapacity = mSize;
		}
		if (mSize > 0) {
			mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			for (int i = 0; i < mSize; ++i) {
				new (&mElements[i]) T;
			}
		}
	}

	template< class T > inline Vector< T >::Vector(const Vector& a) :
		mElements(0),
		mSize(a.mSize),
		mCapacity(a.mCapacity) {
		if (mSize > 0) {
			mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			for (int i = 0; i < mSize; ++i) {
				new (&mElements[i]) T(a.mElements[i]); //복사 콘스트럭터 호출
			}
		}
	}

	template< class T > inline Vector< T >::~Vector() {
		clear();
	}

	template< class T > inline void Vector< T >::operator=(const Vector& a) {
		//operator=()를 쓸 때는 자신을 대입하는 경우를 주의해야 한다.
		if (this == &a) {
			return;
		}
		// 일단 통째로 삭제.충분히 넓은 영역이 있어도 낭비될 가능성이 있으므로 삭제한다.
		clear();

		mSize = a.size;
		mCapacity = a.mCapacity;
		if (mSize > 0) {
			mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			for (int i = 0; i < mSize; ++i) {
				new (&mElements[i]) T(a.mElements[i]); // 복사 콘스트럭터 호출
			}
		}
	}


	template< class T > inline void Vector< T >::push(const T& a) {
		if (mSize == mCapacity) { //장소가 없음.이사를 가다.
			// 1.5배 +1 사이즈의 영역 확보.+1은 2 미만이라도 늘어나도록 하기 위해.
			mCapacity = (mCapacity * 3 / 2) + 1;
			T* newElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			// 전체 복사 및 오래된 요소 파기
			for (int i = 0; i < mSize; ++i) {
				new (&newElements[i]) T(mElements[i]);
				mElements[i].~T();
			}
			// 구영역 파기
			OPERATOR_DELETE(mElements);
			// 교체
			mElements = newElements;
		}
		// 신요소 추가
		new (&mElements[mSize]) T(a);
		// 사이즈 갱신
		++mSize;
	}

	template< class T > inline T* Vector< T >::push() {
		if (mSize == mCapacity) { //장소가 없다. 이사를 가다.
			//1.5배+1사이즈의 영역을 확보.+1은 2 미만이라도 늘어나도록 하기 위해.
			mCapacity = (mCapacity * 3 / 2) + 1;
			T* newElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			//전체 복사본 및 오래된 요소 파기
			for (int i = 0; i < mSize; ++i) {
				new (&newElements[i]) T(mElements[i]);
				mElements[i].~T();
			}
			//예전영역의 파기
			OPERATOR_DELETE(mElements);
			//바꿔 꽂음
			mElements = newElements;
			newElements = 0;
		}
		// 신요소 추가
		new (&mElements[mSize]) T;
		// 사이즈 갱신
		++mSize;
		// 새로운 요소의 포인터 반환
		return &mElements[mSize - 1];
	}

	template< class T > inline void Vector< T >::clear() {
		//디스트럭터 호출
		for (int i = 0; i < mSize; ++i) {
			mElements[i].~T();
		}
		//영역 개방
		OPERATOR_DELETE(mElements);
		mSize = mCapacity = 0;
	}

	template< class T > inline int Vector< T >::size() const {
		return mSize;
	}

	template< class T > inline void Vector< T >::setSize(int n) {
		//지정 크기가 지금의 capacity 이하이면 이사하지 않는다
		if (n < mCapacity) {
			for (int i = n; i < mSize; ++i) { //단, 뒤쪽의 석방은 필요.다섯 개가 있고 셋으로 하려면 첨자 서너 개는 풀어 줘야 한다
				mElements[i].~T();
			}
		}
		else if (n > mCapacity) { //안 되겠다. 이전
			mCapacity = n;
			T* newElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			//전체 복사본 및 오래된 요소 파기
			for (int i = 0; i < mSize; ++i) {
				new (&newElements[i]) T(mElements[i]);
				mElements[i].~T();
			}
			//예전영역 없애기
			OPERATOR_DELETE(mElements);
			//바꿔넣기
			mElements = newElements;
			newElements = 0;
			// 아무것도 없는 영역에 기본 컨스트럭터 호출
			for (int i = mSize; i < n; ++i) {
				new (&mElements[i]) T;
			}
		}
		mSize = n;
	}

	template< class T > inline T& Vector< T >::operator[](int i) {
		ASSERT(i >= 0 && i < mSize && "Range Error!");
		return mElements[i];
	}

	template< class T > inline const T& Vector< T >::operator[](int i) const {
		ASSERT(i >= 0 && i < mSize && "Range Error!");
		return mElements[i];
	}

	template< class T > inline void Vector< T >::moveTo(Vector* a) {
		a->clear();
		a->mElelents = mElements;
		a->mSize = mSize;
		a->mCapacity = mCapacity;
		mElements = 0;
		mSize = mCapacity = 0;
	}

} //namespace GameLib

#endif
