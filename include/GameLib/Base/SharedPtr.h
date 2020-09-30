#ifndef INCLUDED_GAMELIB_SHAREDPTR_H
#define INCLUDED_GAMELIB_SHAREDPTR_H

namespace GameLib{

	/*���� ī���ͽ� ���� ������

	������ �������� �ƴԿ� ����.
	ConstSharedPtr�� ���� ������ const�Լ��ۿ� �θ��� �ϰ� ���� ���� ���� ����
	� ������ Const Shared Ptr � ���� Shared Ptr�� �ȴ�.

	ConstSharedPtr<int > foo(){
	return Shared Ptr<int >( 0 );
	}
	��,
	ConstSharedPtr<int > a = Shared Ptr < int >();
	�� �����̶�� ���̴�.
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

//const��
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