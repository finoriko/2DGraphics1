#ifndef INCLUDED_GAMELIB_ARRAYIMPL_H
#define INCLUDED_GAMELIB_ARRAYIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/Array.h"

namespace GameLib {

	template< class T > inline Array< T >::Array() :
		mElements(0),
		mSize(0) {
	}

	template< class T > inline Array< T >::Array(int size) :
		mElements(0),
		mSize(size) {
		if (size > 0) {
			mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mSize));
			for (int i = 0; i < mSize; ++i) {
				new (&mElements[i]) T;
			}
		}
	}

	template< class T > inline Array< T >::Array(int size, const T& a) :
		mElements(0),
		mSize(size) {
		if (size > 0) {
			mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mSize));
			for (int i = 0; i < mSize; ++i) {
				new (&mElements[i]) T(a);
			}
		}
	}

	template< class T > inline Array< T >::~Array() {
		clear();
	}

	template< class T > inline void Array< T >::setSize(int size) {
		// 비어있거나 축소방향이면 접수합니다.
		if (!mElements) { //비었다면
			mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * size));
			for (int i = 0; i < size; ++i) {
				new (&mElements[i]) T;
			}
		}
		else {
			STRONG_ASSERT((size <= mSize) && "Array::setSize() : extension is not supperted.");
			//축소분 디스트럭트
			for (int i = size; i < mSize; ++i) {
				mElements[i].~T();
			}
		}
		mSize = size;
	}

	template< class T > inline int Array< T >::size() const {
		return mSize;
	}

	template< class T > inline void Array< T >::clear() {
		for (int i = 0; i < mSize; ++i) {
			mElements[i].~T();
		}
		OPERATOR_DELETE(mElements);
		mSize = 0;
	}

	template< class T > inline const T& Array< T >::operator[](int i) const {
		STRONG_ASSERT(i >= 0 && i < mSize && "range error.");
		return mElements[i];
	}

	template< class T > inline T& Array< T >::operator[](int i) {
		STRONG_ASSERT(i >= 0 && i < mSize && "range error.");
		return mElements[i];
	}

} //namespace GameLib

#endif