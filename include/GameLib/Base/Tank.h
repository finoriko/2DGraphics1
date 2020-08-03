#ifndef INCLUDED_GAMELIB_TANK_H
#define INCLUDED_GAMELIB_TANK_H

#include "GameLib/Base/Array.h"

namespace GameLib {

	/// �� ���� ������ �������� �ʰ� �� �� ������ �� �� ���ٴ� �뵵�� ���� ��
	/*!
	���� ��� �ܹ��� �긮�� �̹����� ��ũ��� ������, �� ���� �̸��� ������ ����.
	���������δ� �뷮����(block Size)�� ���ο��� �˷�����Ʈ �Ͽ� �����Ѵ�.

	������ getCurrent()�� ���, toNext()�� ������.rewind()�� ó������ �ٽ� �׼����� �� �ִ�.
	*/
	template< class T > class Tank {
	public:
		explicit Tank(int blockSize = 16);
		~Tank();
		///��Ȯ��. ���빰�� �ִ� ���·� �θ��� assert. clear()�ض�.
		void setBlockSize(int blockSize);
		///���� ��� �� ���
		int size() const;
		///���ع�
		void clear();
		///���̿� �ϴ÷� �߰�.���� �Ϳ� ���� �����͸� ��ȯ�ϹǷ� �ʱ⼳���� �̰��� ����� ��.
		T* add();
		///���̿� �߰�
		void add(const T&);
		/// ������
		void toNext();
		///���Դϴ�.
		bool isEnd() const;
		///���� ��� ��������(��const)
		T* get();
		///���� ��� ��������(const)
		const T* get() const;
		///// ù ��° ��ҷ� ������ �ǵ�����
		void rewind();
		///�ܼ��� �迭�� ��ȯ. �̸� size()�� ũ��� new�� ������ �Ѱ��ִ� ��.
		void copyTo(T*) const;
		///Array�� ����.������ �ƴϸ� ȥ����.
		void copyTo(Array< T >*) const;
	private:
		struct Block {
			Block* mNext; //������ ���
			T* mElements; //�迭
		};
		void operator=(const Tank&); //���Ա���
		Tank(const Tank&); //���� ����Ʈ���ͱ���

		//���
		int mBlockSize; //������
		int mSize;
		Block mHead; //����

		Block* mLastBlock; //�������
		int mLastPos; //���� ��� �� ��ġ
		Block* mCurrentBlock; //���� ���ͷ�����
		int mCurrentPos; //��� �� ��ġ
	};

} //namespace GameLib
#include "GameLib/Base/Impl/TankImpl.h" //������ ����

#endif
