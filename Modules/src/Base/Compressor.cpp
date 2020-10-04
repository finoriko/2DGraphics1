#include "GameLib/GameLib.h"
#include "GameLib/Base/Compressor.h"

#define OPTIMIZATION //�̰��� ON�̶�� ���ȭ ��.OFF��� ���� ����.

namespace GameLib
{
	namespace
	{
		//��Ʈ �Ҵ� ���
		const int DIC_BITS = 11; // ��ġ bit ��
		//���� �ڵ���� ���
		const int LENGTH_BITS = 16 - 1 - DIC_BITS; //����
		const int DIC_MASK = (1 << DIC_BITS) - 1;
		const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; //�Ʒ� 8bit�� ��׷��߸� ��
		const int DIC_MASK_SHIFTED = (DIC_MASK >> 8) << LENGTH_BITS;
		const int LENGTH_MASK = (1 << LENGTH_BITS) - 1;
		const int DIC_SIZE = DIC_MASK + 1; // ���� ũ��(1 ���� �����ϴ� ����� 1 ���� ����)
		const int MAX_LENGTH = LENGTH_MASK + 3; // �ִ� ��ġ ����(3���� �����ϴ� ����� 3�� ���� ��� ����)
		//���� ����ϴ� �ּҰ��� �ִ밪
		inline int min(int a, int b) 
		{
			return (a < b) ? a : b;
		}

		inline int max(int a, int b) 
		{
			return (a > b) ? a : b;
		}

#ifdef OPTIMIZATION
		/*
		���ȭ ������, Ư�㿡 �ɸ� �� ���� ���� ������ �ܼ��� ������ ����������, �׷����� �� ȿ���� �ִ�.
		������ ����, ��a�� ���⡹ ��b�� ���⡹��� �ϴ� ǥ�� �����.
		0-255���� 256�� ���ڿ� ���� ���� ����� ����Ʈ�� ����� �д�.
		ã�� ���ڿ��� abc��� a���� �����ϱ� ������ a�� �ִ� ���� ����Ʈ���� ã��,
		�ű�κ��� �˻��� �����Ѵ�.a�� �ƴ� ���� �����ϱ� ������ ���ڰ� ����� ������ ����� ��������.

		�Ʒ��� Node, Index Ŭ������ �װ��� ���� ǥ�� ǥ���ϴ� Ŭ�����̸�,
		����Ʈ�� �� ��Ʈ���� Node, Node�� �����ϴ� ǥ ��ü�� Index Ŭ������ �ȴ�.
		Node�� ������ �ִ� mPos��, �� ���ڰ� �ִ� ��ġ�� ��Ÿ����. ��Ҵ� ���� ��ü �ȿ����� ��ġ�̸�,
		���� �������� ��ġ�� �ƴϴ�.
		*/

		class Node {
		public:
			int mNext;
			int mPrev;
			int mPos; //���� ���� ��ġ
		};

		class Index { //DIC_SIZE�������� ������� ���.�� �� 256���� ���� ���.DIC_SIZE+c��� �ϴ� ����, c��� �ϴ� ���� ����Ʈ�� ���, �� �ǹ�.
		public:
			Index() {
				//��������� �ʱ�ȭ. next, prev�� �ڽ��� ����Ű���� �ʱ�ȭ.
				for (int i = DIC_SIZE; i < (DIC_SIZE + 256); ++i) {
					mNodes[i].mNext = mNodes[i].mPrev = i;
				}
				//��� �ִ� ��Ҹ� ����� ������ �ʱ�ȭ.DIC_SIZE����
				for (int i = 0; i < DIC_SIZE; ++i) {
					mStack[i] = i;
				}
				mStackPos = DIC_SIZE;
			}
			int getFirst(unsigned char c) const {
				return mNodes[DIC_SIZE + c].mNext;
			}
			const Node* getNode(int i) const {
				return &mNodes[i];
			}
			//���ο� ���Ѵ�. ���ο� ���ϼ��� ���� �˻� ����� �ȴ�.
			void add(unsigned char c, int pos) {
				--mStackPos;
				int newIdx = mStack[mStackPos];
				Node* newNode = mNodes + newIdx;
				Node* head = &mNodes[DIC_SIZE + c];
				newNode->mNext = head->mNext;
				newNode->mPrev = DIC_SIZE + c;
				newNode->mPos = pos;
				mNodes[head->mNext].mPrev = newIdx;
				head->mNext = newIdx;
			}
			// ���̿��� ã�� �����.����� ���� ���� ���� add�� ���̹Ƿ�, ����� ���̿� �ִ�.
			// �� ����� ������ ���� �ʿ�� ����.
			void remove(unsigned char c, int pos) {
				int idx = mNodes[DIC_SIZE + c].mPrev;
				Node* n = &mNodes[idx];
				ASSERT(n->mPos == pos); //�ݵ�� ���⿡ ���� ���̰�, �� ������ �������� ������ ���״�.
				mStack[mStackPos] = mNodes[n->mPrev].mNext; //����
				++mStackPos;
				mNodes[n->mPrev].mNext = n->mNext;
				mNodes[n->mNext].mPrev = n->mPrev;
			}
			bool isEnd(int idx) const { //���� ������� �ƴ����� ��ȯ�Ѵ�.DIC_SIZE �̻��̸� ����.
				return (idx >= DIC_SIZE);
			}
		private:
			Node mNodes[DIC_SIZE + 256];
			int mStack[DIC_SIZE];
			int mStackPos;
		};

#endif //OPTIMIZATION

	}
}