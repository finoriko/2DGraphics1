#ifndef INCLUDED_GAMELIB_POOL_H
#define INCLUDED_GAMELIB_POOL_H

namespace GameLib {

	/// � ������ �δ� ��.���� new�� �谨�ϱ� ���ؼ��� �̿�ȴ�.
	/*!
	Ȯ���ϱ� ���ؼ��� ������ �� ���� ������ �ƹ��͵� �� �� ���ٴ� �Ϳ� ������ ��.
	�� �κ� �ع��� �Ұ����ϴ�.�� �� �ִ� ���� ��ü �ع���̴�.
	*/
	template< class T > class Pool {
	public:
		explicit Pool(int defaultBlockSize = 16);
		~Pool();
		///ǥ�� ��� �� ����.
		void setDefaultBlockSize(int blockSize);
		///��ü �ع�
		void clear();
		///n�� Ȯ��
		T* allocate(int n = 1);
		///n����������Ʈ��Ʈ
		T* allocate(const T*, int n = 1);
	private:
		struct Block {
			Block* mNext; //���� ���
			T* mElements; //�迭
			int mUsedNumber; //���� ��
			int mSize; //��� ũ��
		};
		void operator=(const Pool&); //���� ����
		Pool(const Pool&); //���� �ܽ�Ʈ���� ����

		//���
		int mDefaultBlockSize; //������
		Block mHead; //���� ���
		Block* mLastBlock; //���� ���
	};

} //namespace GameLib
#include "GameLib/Base/Impl/PoolImpl.h" //�ӿ� ����� ���߿�

#endif
