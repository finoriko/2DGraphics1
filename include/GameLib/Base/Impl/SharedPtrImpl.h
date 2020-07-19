#ifndef INCLUDED_GAMELIB_SHAREDPTRIMPL_H
#define INCLUDED_GAMELIB_SHAREDPTRIMPL_H

namespace GameLib {

	template< class T > inline SharedPtr< T >::SharedPtr() : mReference(0) {
	}

	template< class T >	inline SharedPtr< T >::SharedPtr(T* p) {
		if (p) {
			mReference = NEW Reference(p);
		}
	}

	template< class T >	inline SharedPtr< T >::SharedPtr(const SharedPtr< T >& p) :
		mReference(p.mReference) {
		if (mReference) {
			mReference->increment();
		}
	}
	template< class T >	inline SharedPtr< T >::~SharedPtr() {
		if (mReference) {
			if (mReference->decrement() == 0) {
				delete mReference->mPointer;
				delete mReference;
			}
			mReference = 0; //어쨌든 포인터는 0으로 한다.
		}
	}

	template< class T >	inline SharedPtr< T >& SharedPtr< T >::operator=(T* p) {
		if (mReference) {
			if (mReference->decrement() == 0) {
				delete mReference->mPointer;
				if (p) { //포인터가 비어야지.
					mReference->mPointer = p; //또 아무도 안 쓰니까 이걸 바꿔서 유용.
					mReference->mCount = 1;
				}
				else { //아무도 쓰지 않기 때문에 지우다
					delete mReference;
					mReference = 0;
				}
			}
			else if (p) { //포인터가 비어 있지 않아 새로 생성. 하지만 reference는 다른 곳에서 보고 있으므로 건드리지 않는다.
				mReference = NEW Reference(p);
			}
			else { //포인터가 빈. reference는 다른 곳에서 보고 있으므로 만지지 않고, 참조 관계를 끊는다.
				mReference = 0;
			}
		}
		else if (p) {
			mReference = NEW Reference(p);
		}
		return *this;
	}

	template< class T >	inline SharedPtr< T >& SharedPtr< T >::operator=(const SharedPtr< T >& p) {
		if (mReference) {
			if (mReference->decrement() == 0) { //다를 때는 0이 되면 개방
				delete mReference->mPointer;
				delete mReference;
				mReference = 0;
			}
		}
		//교체
		mReference = p.mReference;
		if (mReference) { //물건이 있으면 인크리먼트
			mReference->increment();
		}
		return *this;
	}

	template< class T >	inline T& SharedPtr< T >::operator*() {
		T* p = (mReference) ? mReference->mPointer : 0;
		ASSERT(p);
		return *p;
	}

	template< class T >	inline const T& SharedPtr< T >::operator*() const {
		T* p = (mReference) ? mReference->mPointer : 0;
		ASSERT(p);
		return *p;
	}

	template< class T >	inline T* SharedPtr< T >::operator->() {
		return (mReference) ? mReference->mPointer : 0;
	}

	template< class T >	inline const T* SharedPtr< T >::operator->() const {
		return (mReference) ? mReference->mPointer : 0;
	}

	//포인터를 그대로 반환하면 악용될 우려가 있으므로 비제로이면 0xffffff를 반환한다.
	template< class T >	inline SharedPtr< T >::operator void* () const {
		T* p = (mReference) ? mReference->mPointer : 0;
		return (p) ? reinterpret_cast<void*>(~0) : 0;
	}

	//Reference
	template< class T > inline SharedPtr< T >::Reference::Reference(T* p) :
		mPointer(p),
		mCount(1) {
	}

	template< class T > inline SharedPtr< T >::Reference::~Reference() {
		ASSERT(mCount == 0);
		mPointer = 0;
	}

	template< class T > inline void SharedPtr< T >::Reference::increment() {
		++mCount;
	}

	template< class T > inline int SharedPtr< T >::Reference::decrement() {
		--mCount;
		return mCount;
	}


	//이하 const판
	template< class T > inline ConstSharedPtr< T >::ConstSharedPtr() {
	}

	template< class T >	inline ConstSharedPtr< T >::ConstSharedPtr(const SharedPtr< T >& p) :
		mPointer(p) {
	}

	template< class T >	inline ConstSharedPtr< T >::~ConstSharedPtr() {}

	template< class T >	inline ConstSharedPtr< T >& ConstSharedPtr< T >::operator=(const SharedPtr< T >& p) {
		mPointer = p;
		return *this;
	}

	template< class T >	inline const T& ConstSharedPtr< T >::operator*() const {
		return mPointer.operator*();
	}

	template< class T >	inline const T* ConstSharedPtr< T >::operator->() const {
		return mPointer.operator->();
	}

	template< class T >	inline ConstSharedPtr< T >::operator void* () const {
		return mPointer.operator void* ();
	}

} //namespace GameLib

#endif