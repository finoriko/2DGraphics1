#ifndef INCLUDED_GAMELIB_HASH_H
#define INCLUDED_GAMELIB_HASH_H

namespace GameLib {

	//범용 해시 계산 클래스
	template< class T > class Hash {
	public:
		unsigned value(const T&, unsigned mod) const;
		bool isEqual(const T&, const T&) const;
	};

} //namespace GameLib
#include "GameLib/Base/Impl/HashImpl.h" //속은 이 중

#endif

