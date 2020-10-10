#ifndef INCLUDED_GAMELIB_HASTMAPIMPL_H
#define INCLUDED_GAMELIB_HASHMAPIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/HashMap.h"
#include "GameLib/Base/PrimeNumber.h"

/*
<주의>
첨자로 mCapacity + i (0<=i<mTableSize)가 사용되고 있다면, 그것은 더미 노드이다.
mNodes[mCapacity+ i].mNext는 mNodes[ mCapacity + i + 1 을 꽂고 있다.
*/

namespace GameLib 
{
	// 선두와 말미가 특별하게 취급되지 않도록 조금 궁리하였다.
	template< class K, class V, class H > inline HashMap< K, V, H >::HashMap() :
		mNexts(0),
		mKeys(0),
		mValues(0),
		mCapacity(0),
		mSize(0),
		mTableSize(0) {
		ASSERT(sizeof(K) != 0); // 말도 안되지?
	}

	//선두와 말미가 특별하게 취급되지 않도록 조금 궁리하였다.
	template< class K, class V, class H > inline HashMap< K, V, H >::HashMap(int capacity, int tableSize) :
		mNexts(0),
		mKeys(0),
		mValues(0),
		mCapacity(0),
		mSize(0),
		mTableSize(0) {
		ASSERT(sizeof(K) != 0); //말도 안되지?
		setCapacity(capacity, tableSize);
	}
	template< class K, class V, class H > inline HashMap< K, V, H >::~HashMap() {
		if (mNexts) {
			int pos = mNexts[mCapacity];
			while (pos < mCapacity + mTableSize) { //노드가 있다면
				if (pos < mCapacity) {
					mKeys[pos].~K();
					mValues[pos].~V();
				}
				pos = mNexts[pos];
			}
			OPERATOR_DELETE(mKeys); //생delete
			OPERATOR_DELETE(mValues); //생delete
			SAFE_DELETE_ARRAY(mNexts);
		}
	}

	template< class K, class V, class H > inline int HashMap< K, V, H >::capacity() const {
		return mCapacity;
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
		//p의 위치에 있는 요소의 해시값을 해시값을 계산하지 않고 알고 싶다.
		// 그래서 일단 헤드테이블에 나올 때까지 이태레이터를 진행해 본다.
		int h = position;
		while (h < mCapacity) {
			h = mNexts[h];
		}
		// 일단 범위를 벗어났다.여기에서 mCapacity를 빼면 다음 해시 값을 알 수 있다.
		h -= mCapacity;
		//-1 하면 이놈의 해시 값이다
		h -= 1;
		// 해시값을 알았으니 테이블의 맨 앞에서 걸어가 p를 찾는다.
		int prev = mCapacity + h; //헤드
		int p = mNexts[prev];
		while (p < mCapacity) {
			if (p == position) { //발견 얘 지운다
				mNexts[prev] = mNexts[p]; //바꿔서
				mKeys[p].~K(); //디스트렉터
				mValues[p].~V(); //디스트렉터
				mEmptyStack.push(p); //빈 스택에 푸시
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
		//디스트럭트하며 돌다.
		int prev = mCapacity; //헤드
		int p = mNexts[mCapacity]; //더미 헤드
		while (p < mCapacity + mTableSize) {  //mCapacity + mTableSize는 존재하지 않는다. 거기까지는 더미로서 존재한다.
			if (p < mCapacity) {
				mNexts[prev] = mNexts[p]; //바꿔서
				mKeys[p].~K();
				mValues[p].~V();
				mEmptyStack.push(p); //빈 스택에 푸시
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
			if (p < mCapacity) { //더미 노드 이외를 반환하다
				return p;
			}
		}
		return p;
	}
	template< class K, class V, class H > inline int HashMap< K, V, H >::first() const {
		int p = mCapacity; //최초의 더미
		while (p < mCapacity + mTableSize) {
			p = mNexts[p];
			if (p < mCapacity) { //더미 노드 이외를 반환하다
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
