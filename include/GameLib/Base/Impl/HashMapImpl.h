#ifndef INCLUDED_GAMELIB_HASTMAPIMPL_H
#define INCLUDED_GAMELIB_HASHMAPIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/HashMap.h"
#include "GameLib/Base/PrimeNumber.h"

/*
<����>
÷�ڷ� mCapacity + i (0<=i<mTableSize)�� ���ǰ� �ִٸ�, �װ��� ���� ����̴�.
mNodes[mCapacity+ i].mNext�� mNodes[ mCapacity + i + 1 �� �Ȱ� �ִ�.
*/

namespace GameLib 
{
	// ���ο� ���̰� Ư���ϰ� ��޵��� �ʵ��� ���� �ø��Ͽ���.
	template< class K, class V, class H > inline HashMap< K, V, H >::HashMap() :
		mNexts(0),
		mKeys(0),
		mValues(0),
		mCapacity(0),
		mSize(0),
		mTableSize(0) {
		ASSERT(sizeof(K) != 0); // ���� �ȵ���?
	}

	//���ο� ���̰� Ư���ϰ� ��޵��� �ʵ��� ���� �ø��Ͽ���.
	template< class K, class V, class H > inline HashMap< K, V, H >::HashMap(int capacity, int tableSize) :
		mNexts(0),
		mKeys(0),
		mValues(0),
		mCapacity(0),
		mSize(0),
		mTableSize(0) {
		ASSERT(sizeof(K) != 0); //���� �ȵ���?
		setCapacity(capacity, tableSize);
	}
	template< class K, class V, class H > inline HashMap< K, V, H >::~HashMap() {
		if (mNexts) {
			int pos = mNexts[mCapacity];
			while (pos < mCapacity + mTableSize) { //��尡 �ִٸ�
				if (pos < mCapacity) {
					mKeys[pos].~K();
					mValues[pos].~V();
				}
				pos = mNexts[pos];
			}
			OPERATOR_DELETE(mKeys); //��delete
			OPERATOR_DELETE(mValues); //��delete
			SAFE_DELETE_ARRAY(mNexts);
		}
	}
	template< class K, class V, class H > inline void HashMap< K, V, H >::setCapacity(int capacity, int tableSize) {
		ASSERT(mSize == 0 && "NOT EMPTY! call clear().");
		//�켱 Ŭ����
		if (mCapacity > 0) {
			mEmptyStack.clear();
			clear();
			OPERATOR_DELETE(mValues); //��delete
			OPERATOR_DELETE(mKeys); //��delete
			SAFE_DELETE_ARRAY(mNexts);
		}
		//��Ȯ��
		mCapacity = capacity;
		if (capacity <= 0) { //0�������
			return;
		}
		if (tableSize == 0) { //�ڵ����� capacity �̻��� �Ҽ� ����
			tableSize = capacity;
		}
		tableSize = PrimeNumber::next(tableSize);
		if (tableSize < 3) {
			tableSize = 3; //������3
		}
		mTableSize = tableSize;
		// ���� �迭 ���̺� ũ�⸸ŭ ���� ���ϱ� ����
		mNexts = NEW int[mCapacity + mTableSize];
		//�� �迭 Ȯ��.����Ʈ���Ͱ� ���ʿ��ϹǷ� ��new
		mKeys = static_cast<K*>(OPERATOR_NEW(sizeof(K) * mCapacity));
		mValues = static_cast<V*>(OPERATOR_NEW(sizeof(V) * mCapacity));
		//�� ��ȣ ����
		mEmptyStack.setCapacity(mCapacity);
		for (int i = 0; i < mCapacity; ++i) {
			mEmptyStack.push(i); //�� ��ȣ ����Ʈ�� ä�� ����
		}
		//mCapacity+0���� mCapacity+mTableSize-1���� ���� ���.�� ���� ���� �տ� ��ġ�Ѵ�.
		for (int i = 0; i < mTableSize; ++i) {
			mNexts[mCapacity + i] = mCapacity + i + 1;
		}
	}

	template< class K, class V, class H > inline int HashMap< K, V, H >::capacity() const {
		return mCapacity;
	}
	template< class K, class V, class H > inline int HashMap< K, V, H >::add(const K& k, const V& v) {
		ASSERT(mSize < mCapacity);
		// �ؽ��Լ� ����
		int h = H().value(k, mTableSize);
		//�˻�
		int p = mNexts[mCapacity + h];
		while (p < mCapacity) {
			if (H().isEqual(mKeys[p], k)) {
				break;
			}
			p = mNexts[p];
		}
		if (p >= mCapacity) { //�����Ƿ� ���ϴ�
			////���ڸ� ȹ��
			int newPos;
			mEmptyStack.pop(&newPos);
			// ���̺� ���ο� ����
			int head = mCapacity + h;
			mNexts[newPos] = mNexts[head]; //�Ӹ� ���� ������ ��Ʈ
			new (&mKeys[newPos]) K(k); //���� ����Ʈ���� ȣ��
			new (&mValues[newPos]) V(v); //���� ����Ʈ���� ȣ��
			mNexts[head] = newPos; // �� ��ũ�� �� ��忡 ����
			++mSize;
			return newPos;
		}
		else {
			return mCapacity + mTableSize;
		}
	}

	template< class K, class V, class H > inline int HashMap< K, V, H >::add(const K& k) {
		ASSERT(mSize < mCapacity);
		//�ؽ� �Լ� ����
		int h = H().value(k, mTableSize);
		//�˻�
		int p = mNexts[mCapacity + h];
		bool found = false;
		while (p < mCapacity) {
			if (H().isEqual(mKeys[p], k)) {
				found = true;
			}
			p = mNexts[p];
		}
		if (!found) { //������ ���ϱ�
			//���ڸ� ȹ��
			int newPos;
			mEmptyStack.pop(&newPos);
			//���̺� ���ο� ����
			int head = mCapacity + h;
			mNexts[newPos] = mNexts[head]; //�Ӹ� ���� ������ ��Ʈ
			new (&mKeys[newPos]) K(k); //���� ����Ʈ���� ȣ��
			new (&mValues[newPos]) V;
			mNexts[head] = newPos; //�� ��ũ�� �� ��忡 ����
			++mSize;
			return newPos;
		}
		else {
			return mCapacity + mTableSize;
		}
	}
	template< class K, class V, class H > inline int HashMap< K, V, H >::find(const K& k) const {
		int h = H().value(k, mTableSize);

		int p = mNexts[mCapacity + h];
		while (p < mCapacity) {
			if (H().isEqual(mKeys[p], k)) {
				return p;
			}
			p = mNexts[p];
		}
		return mCapacity + mTableSize;
	}

	template< class K, class V, class H > inline bool HashMap< K, V, H >::remove(int position) {
		//p�� ��ġ�� �ִ� ����� �ؽð��� �ؽð��� ������� �ʰ� �˰� �ʹ�.
		// �׷��� �ϴ� ������̺��� ���� ������ ���·����͸� ������ ����.
		int h = position;
		while (h < mCapacity) {
			h = mNexts[h];
		}
		// �ϴ� ������ �����.���⿡�� mCapacity�� ���� ���� �ؽ� ���� �� �� �ִ�.
		h -= mCapacity;
		//-1 �ϸ� �̳��� �ؽ� ���̴�
		h -= 1;
		// �ؽð��� �˾����� ���̺��� �� �տ��� �ɾ p�� ã�´�.
		int prev = mCapacity + h; //���
		int p = mNexts[prev];
		while (p < mCapacity) {
			if (p == position) { //�߰� �� �����
				mNexts[prev] = mNexts[p]; //�ٲ㼭
				mKeys[p].~K(); //��Ʈ����
				mValues[p].~V(); //��Ʈ����
				mEmptyStack.push(p); //�� ���ÿ� Ǫ��
				--mSize;
				return true;
			}
			prev = p;
			p = mNexts[p];
		}
		return false;
	}

	template< class K, class V, class H > inline const V* HashMap< K, V, H >::value(int p) const {
		return &mValues[p];
	}

	template< class K, class V, class H > inline V* HashMap< K, V, H >::value(int p) {
		return &mValues[p];
	}

	template< class K, class V, class H > inline V* HashMap< K, V, H >::operator[](const K& k) {
		int p = find(k);
		return (p < mCapacity) ? &mValues[p] : 0;
	}

	template< class K, class V, class H > inline const V* HashMap< K, V, H >::operator[](const K& k) const {
		int p = find(k);
		return (p < mCapacity) ? &mValues[p] : 0;
	}

	template< class K, class V, class H > inline const K* HashMap< K, V, H >::key(int p) const {
		return &mKeys[p];
	}

	template< class K, class V, class H > inline void HashMap< K, V, H >::clear() {
		//��Ʈ��Ʈ�ϸ� ����.
		int prev = mCapacity; //���
		int p = mNexts[mCapacity]; //���� ���
		while (p < mCapacity + mTableSize) {  //mCapacity + mTableSize�� �������� �ʴ´�. �ű������ ���̷μ� �����Ѵ�.
			if (p < mCapacity) {
				mNexts[prev] = mNexts[p]; //�ٲ㼭
				mKeys[p].~K();
				mValues[p].~V();
				mEmptyStack.push(p); //�� ���ÿ� Ǫ��
			}
			prev = p;
			p = mNexts[p];
		}
		mSize = 0;
	}
	template< class K, class V, class H > inline int HashMap< K, V, H >::next(int p) const {
		ASSERT(p >= 0);
		while (p < mCapacity + mTableSize) {
			p = mNexts[p];
			if (p < mCapacity) { //���� ��� �ܸ̿� ��ȯ�ϴ�
				return p;
			}
		}
		return p;
	}
	template< class K, class V, class H > inline int HashMap< K, V, H >::first() const {
		int p = mCapacity; //������ ����
		while (p < mCapacity + mTableSize) {
			p = mNexts[p];
			if (p < mCapacity) { //���� ��� �ܸ̿� ��ȯ�ϴ�
				return p;
			}
		}
		return mCapacity + mTableSize;
	}
template< class K, class V, class H > inline bool HashMap< K, V, H >::isEnd(int position) const {
	return (position == mCapacity + mTableSize);
}

template< class K, class V, class H > inline void HashMap< K, V, H >::copyTo(V* a) const {
	int p = mNexts[mCapacity];
	int idx = 0;
	while (p != mCapacity + mTableSize) {
		if (p < mCapacity) {
			a[idx] = mValues[p];
			++idx;
		}
		p = mNexts[p];
	}
}

template< class K, class V, class H > inline void HashMap< K, V, H >::copyTo(Array< V >* a) const {
	a->setSize(mSize);
	if (mSize > 0) {
		copyTo(&((*a)[0]));
	}
}


template< class K, class V, class H > inline int HashMap< K, V, H >::size() const {
	return mSize;
}

} //namespace GameLib

#endif