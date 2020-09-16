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
				new (&mElements[i]) T(a.mElements[i]); //���� �ܽ�Ʈ���� ȣ��
			}
		}
	}

	template< class T > inline Vector< T >::~Vector() {
		clear();
	}

	template< class T > inline void Vector< T >::operator=(const Vector& a) {
		//operator=()�� �� ���� �ڽ��� �����ϴ� ��츦 �����ؾ� �Ѵ�.
		if (this == &a) {
			return;
		}
		// �ϴ� ��°�� ����.����� ���� ������ �־ ����� ���ɼ��� �����Ƿ� �����Ѵ�.
		clear();

		mSize = a.size;
		mCapacity = a.mCapacity;
		if (mSize > 0) {
			mElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			for (int i = 0; i < mSize; ++i) {
				new (&mElements[i]) T(a.mElements[i]); // ���� �ܽ�Ʈ���� ȣ��
			}
		}
	}


	template< class T > inline void Vector< T >::push(const T& a) {
		if (mSize == mCapacity) { //��Ұ� ����.�̻縦 ����.
			// 1.5�� +1 �������� ���� Ȯ��.+1�� 2 �̸��̶� �þ���� �ϱ� ����.
			mCapacity = (mCapacity * 3 / 2) + 1;
			T* newElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			// ��ü ���� �� ������ ��� �ı�
			for (int i = 0; i < mSize; ++i) {
				new (&newElements[i]) T(mElements[i]);
				mElements[i].~T();
			}
			// ������ �ı�
			OPERATOR_DELETE(mElements);
			// ��ü
			mElements = newElements;
		}
		// �ſ�� �߰�
		new (&mElements[mSize]) T(a);
		// ������ ����
		++mSize;
	}

	template< class T > inline T* Vector< T >::push() {
		if (mSize == mCapacity) { //��Ұ� ����. �̻縦 ����.
			//1.5��+1�������� ������ Ȯ��.+1�� 2 �̸��̶� �þ���� �ϱ� ����.
			mCapacity = (mCapacity * 3 / 2) + 1;
			T* newElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			//��ü ���纻 �� ������ ��� �ı�
			for (int i = 0; i < mSize; ++i) {
				new (&newElements[i]) T(mElements[i]);
				mElements[i].~T();
			}
			//���������� �ı�
			OPERATOR_DELETE(mElements);
			//�ٲ� ����
			mElements = newElements;
			newElements = 0;
		}
		// �ſ�� �߰�
		new (&mElements[mSize]) T;
		// ������ ����
		++mSize;
		// ���ο� ����� ������ ��ȯ
		return &mElements[mSize - 1];
	}

	template< class T > inline void Vector< T >::clear() {
		//��Ʈ���� ȣ��
		for (int i = 0; i < mSize; ++i) {
			mElements[i].~T();
		}
		//���� ����
		OPERATOR_DELETE(mElements);
		mSize = mCapacity = 0;
	}

	template< class T > inline int Vector< T >::size() const {
		return mSize;
	}

	template< class T > inline void Vector< T >::setSize(int n) {
		//���� ũ�Ⱑ ������ capacity �����̸� �̻����� �ʴ´�
		if (n < mCapacity) {
			for (int i = n; i < mSize; ++i) { //��, ������ ������ �ʿ�.�ټ� ���� �ְ� ������ �Ϸ��� ÷�� ���� ���� Ǯ�� ��� �Ѵ�
				mElements[i].~T();
			}
		}
		else if (n > mCapacity) { //�� �ǰڴ�. ����
			mCapacity = n;
			T* newElements = static_cast<T*>(OPERATOR_NEW(sizeof(T) * mCapacity));
			//��ü ���纻 �� ������ ��� �ı�
			for (int i = 0; i < mSize; ++i) {
				new (&newElements[i]) T(mElements[i]);
				mElements[i].~T();
			}
			//�������� ���ֱ�
			OPERATOR_DELETE(mElements);
			//�ٲ�ֱ�
			mElements = newElements;
			newElements = 0;
			// �ƹ��͵� ���� ������ �⺻ ����Ʈ���� ȣ��
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
