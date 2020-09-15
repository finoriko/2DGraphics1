#ifndef INCLUDED_GAMELIB_VECTOR_H
#define INCLUDED_GAMELIB_VECTOR_H

namespace GameLib {

	///저기능판 std::vector
/*!
std ::vector를 사용하지 않고 이쪽을 사용하는 장점은 무엇입니까?
사실 별로 없다.빈도가 낮은 함수나 위험한 함수를 제외하고 있으므로
사용하기 쉬워 학습에 적합할 것이다.
나머지는 STL에서 size_t가 돌아오는 것에 질려있는 사람에게는 추천할 만하다.int면 되잖아!

기본적으로 이 클래스는 편하게 쓰기 위해 있으며, 사이즈 변경 계열의 함수를
부르면 당연히 늦었지.Array와 다르게 복사를 할 수도 있지만, 아무 생각없는 복사는 파멸적인 성능의 열화를
초대할 것이다.
*/
	template< class T > class Vector {
	public:
		///capacity를 지정하면 그 수만큼 늘어나도 이사하지 않는다.
		explicit Vector(int size = 0, int capacity = 0);
		/// 통째로 복사한다.늦은 일은 각오하라
		Vector(const Vector&); //복사 콘스트럭터를 금지하지 않음
		~Vector();
		// 대입. 통째로 복사한다.늦은 일은 각오하라
		void operator=(const Vector&); //代入も禁止しない
		/// 추가
		void push(const T&);
		/// 장소만 만들기 추가.포인터를 돌려줄 테니 여기에 넣을 것.
		T* push();
		/// 최종 요소 삭제
		void pop();
		/// 전체 삭제
		void clear();
		/// 사이즈 획득
		int size() const;
		/// 사이즈 변경. 작게 해도 이사가 일어나지 않는다.
		void setSize(int);
		// 첨자(비const)
		T& operator[](int);
		//첨자(const)
		const T& operator[](int) const;
		///내용물을 다른 벡터로 옮긴다.나는 비워진다.상대에게 들어 있던 것은 사라진다.
		void moveTo(Vector*);
	private:
		T* mElements;
		int mSize;
		int mCapacity;
	};

} //namespace GameLib
#include "GameLib/Base/Impl/VectorImpl.h" //내용은 이중

#endif

