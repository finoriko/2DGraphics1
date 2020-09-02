#ifndef INCLUDED_GAMELIB_STACK_H
#define INCLUDED_GAMELIB_STACK_H

namespace GameLib {

	/// ������ ���� ������ ����
	template< class T > class Stack {
	public:
		/// �⺻��.������, �ִ� �뷮�� 0.
		Stack();
		///�ִ� �뷮 ����.
		explicit Stack(int capacity);
		~Stack();
		///��Ȯ��.���빰�� �ִ� ���¿��� �θ��� assert. clear()�ض�.
		void setCapacity(int size);
		///�ִ� �뷮 ȹ��
		int capacity() const;
		/// ���� ��� �� ȹ��
		int size() const;
		///���ع�
		void clear();
		///���̿� �߰�
		void push(const T&);
		///���̿� �߰�(�⺻�� ����Ʈ���� ȣ��)
		void push();
		///���� ����
		void pop();
		///���� ����(������ ��� ���� �޾ƺ���)
		void pop(T* out);
		///���� ���(const)
		const T* get() const;
		///���� ���(��const)
		T* get();
	private:
		void operator=(const Stack&); //���� ����
		Stack(const Stack&); //���� �ܽ�Ʈ���� ����

		T* mElements;
		int mSize;
		int mCapacity;
		int mBack;
	};

} //namespace GameLib
#include "GameLib/Base/Impl/StackImpl.h" //���� �� �ȿ� �ִ�

#endif

