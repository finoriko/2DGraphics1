#include "GameLib/GameLib.h"
#include "GameLib/Base/Compressor.h"

#define OPTIMIZATION //이것이 ON이라고 고속화 판.OFF라고 우직 버전.

namespace GameLib
{
	namespace
	{
		//비트 할당 상수
		const int DIC_BITS = 11; // 위치 bit 수
		//이하 자동계산 상수
		const int LENGTH_BITS = 16 - 1 - DIC_BITS; //길이
		const int DIC_MASK = (1 << DIC_BITS) - 1;
		const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; //아래 8bit를 찌그러뜨린 것
		const int DIC_MASK_SHIFTED = (DIC_MASK >> 8) << LENGTH_BITS;
		const int LENGTH_MASK = (1 << LENGTH_BITS) - 1;
		const int DIC_SIZE = DIC_MASK + 1; // 사전 크기(1 빼서 저장하는 관계로 1 많이 사용됨)
		const int MAX_LENGTH = LENGTH_MASK + 3; // 최대 일치 길이(3빼서 저장하는 관계로 3개 많이 사용 가능)
		//자주 사용하는 최소값과 최대값
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
		고속화 수법은, 특허에 걸릴 것 같지 않을 정도로 단순한 구조에 억제했지만, 그런데도 꽤 효과가 있다.
		사전에 대해, 「a는 여기」 「b는 여기」라고 하는 표를 만든다.
		0-255까지 256개 문자에 대해 각각 양방향 리스트를 만들어 둔다.
		찾는 문자열이 abc라면 a부터 시작하기 때문에 a가 있는 곳을 리스트에서 찾고,
		거기로부터 검색을 시작한다.a가 아닌 곳은 무시하기 때문에 글자가 흩어져 있으면 상당히 빨라진다.

		아래의 Node, Index 클래스는 그것을 위한 표를 표현하는 클래스이며,
		리스트의 각 엔트리가 Node, Node를 관리하는 표 전체가 Index 클래스가 된다.
		Node가 가지고 있는 mPos가, 그 문자가 있는 위치를 나타낸다. 장소는 파일 전체 안에서의 위치이며,
		사전 내에서의 위치가 아니다.
		*/

		class Node {
		public:
			int mNext;
			int mPrev;
			int mPos; //파일 내의 위치
		};

		class Index { //DIC_SIZE번까지가 사용중인 노드.그 뒤 256개는 더미 헤드.DIC_SIZE+c라고 하는 것은, c라고 하는 문자 리스트의 헤드, 의 의미.
		public:
			Index() {
				//더미헤드의 초기화. next, prev는 자신을 가리키듯이 초기화.
				for (int i = DIC_SIZE; i < (DIC_SIZE + 256); ++i) {
					mNodes[i].mNext = mNodes[i].mPrev = i;
				}
				//비어 있는 장소를 기록한 스택의 초기화.DIC_SIZE까지
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
			//선두에 더한다. 새로운 것일수록 빨리 검색 대상이 된다.
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
			// 말미에서 찾아 지운다.지우는 것은 제일 먼저 add한 것이므로, 절대로 말미에 있다.
			// 즉 목록을 일일이 돌릴 필요는 없다.
			void remove(unsigned char c, int pos) {
				int idx = mNodes[DIC_SIZE + c].mPrev;
				Node* n = &mNodes[idx];
				ASSERT(n->mPos == pos); //반드시 여기에 있을 것이고, 이 조건이 충족되지 않으면 버그다.
				mStack[mStackPos] = mNodes[n->mPrev].mNext; //삭제
				++mStackPos;
				mNodes[n->mPrev].mNext = n->mNext;
				mNodes[n->mNext].mPrev = n->mPrev;
			}
			bool isEnd(int idx) const { //더미 헤드인지 아닌지를 반환한다.DIC_SIZE 이상이면 더미.
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