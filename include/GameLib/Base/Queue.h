#ifndef INCLUDED_GAMELIB_QUEUE_H
#define INCLUDED_GAMELIB_QUEUE_H

namespace GameLib {

	/// ť. ��� ���� �Ѱ踦 ���� ������ ��ȯ�ϱ� ������ �������� ����� ���ִ�.
	template< class T > class Queue {
	public:
		// �⺻��.������, �ִ� �뷮�� 0.
		Queue();
		/// �ִ� �뷮 ����.
		explicit Queue(int capacity);
		~Queue();
		/// ��Ȯ��.���빰�� �ִ� ���¿��� �θ��� assert. clear() �ض�
		void setCapacity(int size);
		/// �ִ� �뷮 ȹ��
		int capacity() const;
		/// ���� ��� �� ȹ��
		int size() const;
		/// ���ع�
		void clear();
		/// ���̿� �ϴ÷� �߰�.���� �Ϳ� ���� �����͸� ��ȯ�ϹǷ� �ʱ⼳���� �̰��� ����� ��.
		T* pushBack();
		/// ���̿� �߰�
		void pushBack(const T&);
		///���� ����
		void popFront();
		///���� ����(���� �纻 ��������)
		void popFront(T* out);
		///���� ȹ��(const)
		const T* front() const;
		///���� ȹ��(��const)
		T* front();
	private:
		void operator=(const Queue&); //���Ա���
		Queue(const Queue&); //���� �ܽ�Ʈ���� ����

		T* mElements;
		int mSize;
		int mCapacity;
		int mFront;
	};

} //namespace GameLib
#include "GameLib/Base/Impl/QueueImpl.h" //���� �� ��

#endif

