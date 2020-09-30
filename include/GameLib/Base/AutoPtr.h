#ifndef INCLUDED_GAMELIB_AUTOPTR_H
#define INCLUDED_GAMELIB_AUTOPTR_H

namespace GameLib{

template< class T > class AutoPtr{
public:
	AutoPtr( T* p = 0 );
	~AutoPtr();
	//���� : ���빰�� ������ ��� ���� (�̸� �̿��Ͽ� =0���� ���ϴ� Ÿ�ֿ̹� ���� �� ����)
	AutoPtr& operator=( T* p );
	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;
	operator void*() const;//�̰� ���п� if (p) {��� �� �� �ִ�.
private:
	void operator=( AutoPtr< T >& ); //���Ա��� ������ �̾��� �����ϱ� ��ƴ�.
	AutoPtr( AutoPtr< T >& ); //���� ������ ����

	T* mPointer;
};

} //namespace GameLib
#include "GameLib/Base/Impl/AutoPtrImpl.h"

#endif