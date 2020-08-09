#ifndef INCLUDED_GAMELIB_SHAREDPTR_H
#define INCLUDED_GAMELIB_SHAREDPTR_H

namespace GameLib{

	/*참조 카운터식 공유 포인터

	쓰레드 세이프가 아님에 주의.
	ConstSharedPtr은 원래 형태의 const함수밖에 부르게 하고 싶지 않을 때를 위해
	어떤 것으로 Const Shared Ptr 어떤 곳은 Shared Ptr도 된다.

	ConstSharedPtr<int > foo(){
	return Shared Ptr<int >( 0 );
	}
	야,
	ConstSharedPtr<int > a = Shared Ptr < int >();
	가 오케이라는 것이다.
	*/
template< class T > class SharedPtr{
public:
	SharedPtr();
	SharedPtr( T* p );
	SharedPtr( const SharedPtr< T >& p );
	~SharedPtr();
	SharedPtr& operator=( T* p );
	SharedPtr& operator=( const SharedPtr< T >& p );
	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;
	operator void*() const;
private:
	struct Reference{
		Reference( T* p );
		~Reference();
		void increment();
		int decrement();

		T* mPointer;
		unsigned mCount;
	};
	Reference* mReference;
};

//const版
template< class T > class ConstSharedPtr{
public:
	ConstSharedPtr();
	ConstSharedPtr( const SharedPtr< T >& );
	~ConstSharedPtr();
	ConstSharedPtr& operator=( const SharedPtr< T >& );
	const T& operator*() const;
	const T* operator->() const;
	operator void*() const;
private:
	SharedPtr< T > mPointer;
};

} //namespace GameLib
#include "GameLib/Base/Impl/SharedPtrImpl.h"

#endif