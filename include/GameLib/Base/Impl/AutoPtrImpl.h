#ifndef INCLUDED_GAMELIB_AUTOPTRIMPL_H
#define INCLUDED_GAMELIB_AUTOPTRIMPL_H

namespace GameLib 
{

	template< class T > inline AutoPtr< T >::AutoPtr(T* p = 0) :
		mPointer(p) {
	}

	template< class T > inline AutoPtr< T >::~AutoPtr() {
		delete mPointer;
		mPointer = 0;
	}

	template< class T > inline AutoPtr< T >& AutoPtr< T >::operator=(T* p) {
		delete mPointer;
		mPointer = p;
		return *this;
	}

	template< class T > inline T& AutoPtr< T >::operator*() {
		return *mPointer;
	}

	template< class T > inline const T& AutoPtr< T >::operator*() const {
		return *mPointer;
	}

	template< class T > inline T* AutoPtr< T >::operator->() {
		return mPointer;
	}

	template< class T > inline const T* AutoPtr< T >::operator->() const {
		return mPointer;
	}
	//포인터를 그대로 돌려놓으면 악용될 우려가 있기 때문에 비제로이면 0xffffff를 반환한다.
	template< class T > inline AutoPtr< T >::operator void* () const {
		return (mPointer) ? reinterpret_cast<void*>(~0) : 0;
	}

} //namespace GameLib

#endif