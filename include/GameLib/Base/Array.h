#ifndef INCLUDED_GAMELIB_ARRAY_H
#define INCLUDED_GAMELIB_ARRAY_H

namespace GameLib{

//������ �� �ؾ������ ���� ���� �ڵ� ���� �迭 ���ø�
template< class T > class Array{
public:
	///����Ʈ ������� 0
	Array();
	///�ʱ� �������Դϴ�.��� ��Ҵ� �⺻ �����ڷ� �ʱ�ȭ�˴ϴ�.
	explicit Array( int size );
	//�ʱ� ������ �ʱⰪ ����
	explicit Array( int size, const T& );
	~Array();
	///���߿� Ȯ������Ҵ� ������������Ȯ��� �Ұ����ϴ�clear()�Ұ͡�
	void setSize( int size );
	///������ ��������
	int size() const;
	///Ŭ����
	void clear();
	//÷��
	const T& operator[]( int i ) const;
	//��÷��
	T& operator[]( int i );
private:
	void operator=( const Array& ); //���Ա���
	Array( const Array& ); //���� ������ ����

	T* mElements;
	int mSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/ArrayImpl.h" //���� ���� ���Դϴ�.

#endif

