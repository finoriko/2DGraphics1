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
	template< class K, class V, class H > inline void HashMap< K, V, H >::setCapacity(int capacity, int tableSize) {
		ASSERT(mSize == 0 && "NOT EMPTY! call clear().");
		//우선 클리어
		if (mCapacity > 0) {
			mEmptyStack.clear();
			clear();
			OPERATOR_DELETE(mValues); //생delete
			OPERATOR_DELETE(mKeys); //생delete
			SAFE_DELETE_ARRAY(mNexts);
		}
		//재확보
		mCapacity = capacity;
		if (capacity <= 0) { //0사이즈무한
			return;
		}
		if (tableSize == 0) { //자동으로 capacity 이상의 소수 설정
			tableSize = capacity;
		}
		tableSize = PrimeNumber::next(tableSize);
		if (tableSize < 3) {
			tableSize = 3; //최저로3
		}
		mTableSize = tableSize;
		// 다음 배열 테이블 크기만큼 더미 더하기 주의
		mNexts = NEW int[mCapacity + mTableSize];
		//값 배열 확보.컨스트럭터가 불필요하므로 생new
		mKeys = static_cast<K*>(OPERATOR_NEW(sizeof(K) * mCapacity));
		mValues = static_cast<V*>(OPERATOR_NEW(sizeof(V) * mCapacity));
		//빈 번호 스택
		mEmptyStack.setCapacity(mCapacity);
		for (int i = 0; i < mCapacity; ++i) {
			mEmptyStack.push(i); //빈 번호 리스트에 채워 가다
		}
		//mCapacity+0부터 mCapacity+mTableSize-1번은 더미 요소.각 열의 선두 앞에 위치한다.
		for (int i = 0; i < mTableSize; ++i) {
			mNexts[mCapacity + i] = mCapacity + i + 1;
		}
	}

	template< class K, class V, class H > inline int HashMap< K, V, H >::capacity() const {
		return mCapacity;
	}
	template< class K, class V, class H > inline int HashMap< K, V, H >::add(const K& k, const V& v) {
		ASSERT(mSize < mCapacity);
		// 해시함수 산출
		int h = H().value(k, mTableSize);
		//검색
		int p = mNexts[mCapacity + h];
		while (p < mCapacity) {
			if (H().isEqual(mKeys[p], k)) {
				break;
			}
			p = mNexts[p];
		}
		if (p >= mCapacity) { //없으므로 더하다
			////빈자리 획득
			int newPos;
			mEmptyStack.pop(&newPos);
			// 테이블 선두에 삽입
			int head = mCapacity + h;
			mNexts[newPos] = mNexts[head]; //머리 더미 다음을 세트
			new (&mKeys[newPos]) K(k); //복사 컨스트럭터 호출
			new (&mValues[newPos]) V(v); //복사 컨스트럭터 호출
			mNexts[head] = newPos; // 앞 링크를 새 노드에 연결
			++mSize;
			return newPos;
		}
		else {
			return mCapacity + mTableSize;
		}
	}

	template< class K, class V, class H > inline int HashMap< K, V, H >::add(const K& k) {
		ASSERT(mSize < mCapacity);
		//해시 함수 산출
		int h = H().value(k, mTableSize);
		//검색
		int p = mNexts[mCapacity + h];
		bool found = false;
		while (p < mCapacity) {
			if (H().isEqual(mKeys[p], k)) {
				found = true;
			}
			p = mNexts[p];
		}
		if (!found) { //없으니 더하기
			//빈자리 획득
			int newPos;
			mEmptyStack.pop(&newPos);
			//테이블 선두에 삽입
			int head = mCapacity + h;
			mNexts[newPos] = mNexts[head]; //머리 더미 다음을 세트
			new (&mKeys[newPos]) K(k); //복사 컨스트럭터 호출
			new (&mValues[newPos]) V;
			mNexts[head] = newPos; //앞 링크를 새 노드에 연결
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
