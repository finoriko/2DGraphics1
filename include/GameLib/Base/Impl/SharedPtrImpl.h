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
			mReference = 0; //��·�� �����ʹ� 0���� �Ѵ�.
		}
	}

	template< class T >	inline SharedPtr< T >& SharedPtr< T >::operator=(T* p) {
		if (mReference) {
			if (mReference->decrement() == 0) {
				delete mReference->mPointer;
				if (p) { //�����Ͱ� ������.
					mReference->mPointer = p; //�� �ƹ��� �� ���ϱ� �̰� �ٲ㼭 ����.
					mReference->mCount = 1;
				}
				else { //�ƹ��� ���� �ʱ� ������ �����
					delete mReference;
					mReference = 0;
				}
			}
			else if (p) { //�����Ͱ� ��� ���� �ʾ� ���� ����. ������ reference�� �ٸ� ������ ���� �����Ƿ� �ǵ帮�� �ʴ´�.
				mReference = NEW Reference(p);
			}
			else { //�����Ͱ� ��. reference�� �ٸ� ������ ���� �����Ƿ� ������ �ʰ�, ���� ���踦 ���´�.
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
			if (mReference->decrement() == 0) { //�ٸ� ���� 0�� �Ǹ� ����
				delete mReference->mPointer;
				delete mReference;
				mReference = 0;
			}
		}
		//��ü
		mReference = p.mReference;
		if (mReference) { //������ ������ ��ũ����Ʈ
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

	//�����͸� �״�� ��ȯ�ϸ� �ǿ�� ����� �����Ƿ� �������̸� 0xffffff�� ��ȯ�Ѵ�.
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


	//���� const��
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