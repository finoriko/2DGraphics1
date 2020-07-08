#ifndef INCLUDED_GAMELIB_AUTOPTR_H
#define INCLUDED_GAMELIB_AUTOPTR_H

namespace GameLib{

template< class T > class AutoPtr{
public:
	AutoPtr( T* p = 0 );
	~AutoPtr();
	//대입 : 내용물이 있으면 즉시 삭제 (이를 이용하여 =0으로 원하는 타이밍에 지울 수 있음)
	AutoPtr& operator=( T* p );
	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;
	operator void*() const; //이것 덕분에 if (p) {라고 쓸 수 있다.
private:
	void operator=( AutoPtr< T >& ); //대입금지 관리의 이양은 이해하기 어렵다.
	AutoPtr( AutoPtr< T >& ); //복사 생성자 금지

	T* mPointer;
};

} //namespace GameLib
#include "GameLib/Base/Impl/AutoPtrImpl.h"

#endif