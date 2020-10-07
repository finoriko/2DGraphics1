#ifndef INCLUDED_GAMELIB_HASHMAP_H
#define INCLUDED_GAMELIB_HASHMAP_H

#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Hash.h"
#include "GameLib/Base/Array.h"

namespace GameLib
{
	///한계용량지정해시테이블
	/*!
	해시함수는 전체 비트의 합을 테이블 사이즈로 잉여를 취하는 단순한 방식이다.
	전체 비트이기 때문에 클래스의 사이즈가 크면 비용이 커질 가능성이 높다.
	또한 동일 키를 갖는 요소는 추가하지 않는다.
	*/
	template< class Key, class Value, class H = Hash< Key > > class HashMap 
	{
	public:
		// 기본값.set capacity를 불러야 작동한다
		HashMap();
		/// 최대 용량 지정.테이블 크기는 기본적으로 capacity 이상의 최소 소수.
		explicit Hash Map(intcapacity, inttable Size = 0);
		~HashMap();
		/// 재확보.내용물이 있는 상태에서 부르면 assert. clear()해라.
		void set Capacity(int capacity, int table Size = 0);
		/// 최대 용량 획득
		int capacity() const;
		/// 현재 요소 수 획득
		int size() const;
		/// 전해방
		void clear();
		/// 추가. 추가한 장소를 반환한다.같은 것이 이미 있으면 무시하고 무효 이터레이터를 반환한다.
		intadd(const Key&, const Value&);
		/// 추가. 추가한 장소를 반환한다.같은 것이 이미 있으면 무시하고 무효 이터레이터를 반환한다. 값은 getValue하고 나중에 써넣는다.
		intadd(const Key&);
		/// 검색. 되돌아오는 것은 이터레이터
		int find(const Key& const;
		///이테레이터를 지정하여 삭제. 삭제 시 true를 반환한다.
		bool remove(int position);
		///키 취득(환서는 금지이므로 const만 해당)
		const Key* key(int position const;
		/// 값 취득(const)
		const Value* value(int position) const;
		/// 값 취득(비const)
		Value* value(int position);
		/// 검색하여 값 취득(const)
		const Value* operator[](const Key& const;
		/// 검색하여 값 취득(비const)
		Value* operator[](const Key&);
		//아래 단순한 목록으로 접근할 때의 인터페이스. 단 한 방향
		int next(int position) const;
		int first() const;
		///position이 말단인지 판정.
		bool is End(int position) const;
		///단순한 배열에 복사.미리 size()한 크기로 new한 영역을 넘겨준다.
		void copyTo(Value*) const;
		///Array에 복사.빈 것을 건네라.
		void copyTo(Array < Value >* const;
	private:
		void operator=(const HashMap&); //대입금지
		HashMap(const HashMap&); //복사생성자 금지

		int* mNexts;
		Key* mKeys;
		Value* mValues;
		Stack< int > mEmptyStack;
		int mCapacity;
		int mSize;
		int mTableSize;
	};
}//namespace GameLib
#include "GameLib/Base/Impl/HashMapImpl.h"// 속은 이 중
#endif